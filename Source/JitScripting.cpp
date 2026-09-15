#include "stdafx.hpp"
#include "JitScripting.hpp"
#include "Utils/duktape.h"
#include <stdexcept>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#define USE_EDGEMODE_JSRT
#include <jsrt.h>

namespace {
std::wstring wide(const std::string& text) {
    if(text.empty()) return {};
    int count = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.data(), (int)text.size(), nullptr, 0);
    if(!count) throw std::runtime_error("Invalid UTF-8 script text");
    std::wstring result(count, L'\0');
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, text.data(), (int)text.size(), result.data(), count);
    return result;
}
std::string narrow(const wchar_t* text, size_t count) {
    if(!count) return {};
    int bytes = WideCharToMultiByte(CP_UTF8, 0, text, (int)count, nullptr, 0, nullptr, nullptr);
    std::string result(bytes, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text, (int)count, result.data(), bytes, nullptr, nullptr);
    return result;
}
void check(JsErrorCode code) {
    if(code != JsNoError) throw std::runtime_error("JIT API error " + std::to_string((int)code));
}
const char* classes[] = {"FileIO", "cPosition", "sSprite", "cRandomMap", "cPhase", "cMission", "cCampaign", "sGameData", "cEngine"};
}

struct cJitScripting::Impl {
    HMODULE library = nullptr;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef context = JS_INVALID_REFERENCE;
    JsValueRef global = JS_INVALID_REFERENCE;
    duk_context* duk;
    JsSourceContext sourceId = 0;
    struct NativeRef {
        Impl* owner;
        void* pointer;
        unsigned id;
        JsValueRef wrapper = JS_INVALID_REFERENCE; // Weak: cleared by the GC callback.
    };
    std::unordered_map<void*, std::unique_ptr<NativeRef>> handles;
    std::vector<void*> collected;
    struct Method { Impl* owner; std::string name; };
    std::vector<std::unique_ptr<Method>> methods;
    unsigned nextHandle = 0;
    bool ready = false;

#define JIT_API_LIST(X) \
    X(JsCreateRuntime) X(JsCreateContext) X(JsGetCurrentContext) X(JsSetCurrentContext) X(JsDisposeRuntime) \
    X(JsAddRef) X(JsRelease) X(JsSetObjectBeforeCollectCallback) \
    X(JsGetGlobalObject) X(JsCreateFunction) X(JsGetPropertyIdFromName) X(JsSetProperty) X(JsGetProperty) \
    X(JsPointerToString) X(JsStringToPointer) X(JsRunScript) X(JsConvertValueToString) \
    X(JsGetValueType) X(JsGetUndefinedValue) X(JsGetNullValue) X(JsBoolToBoolean) X(JsBooleanToBool) \
    X(JsDoubleToNumber) X(JsNumberToDouble) X(JsIntToNumber) X(JsCreateObject) X(JsCreateArray) \
    X(JsGetIndexedProperty) X(JsSetIndexedProperty) X(JsGetOwnPropertyNames) X(JsCallFunction) \
    X(JsGetAndClearException) X(JsCreateError) X(JsSetException)
#define DECLARE_API(name) decltype(&::name) name = nullptr;
    JIT_API_LIST(DECLARE_API)
#undef DECLARE_API

    explicit Impl(duk_context* bindings) : duk(bindings) {}
    struct CurrentContext {
        Impl& owner;
        JsContextRef previous = JS_INVALID_REFERENCE;
        explicit CurrentContext(Impl& impl) : owner(impl) {
            check(owner.JsGetCurrentContext(&previous));
            check(owner.JsSetCurrentContext(owner.context));
        }
        ~CurrentContext() { owner.JsSetCurrentContext(previous); }
    };
    ~Impl() {
        if(runtime != JS_INVALID_RUNTIME_HANDLE) {
            JsContextRef previous = JS_INVALID_REFERENCE;
            JsGetCurrentContext(&previous);
            if(previous == context) JsSetCurrentContext(JS_INVALID_REFERENCE);
            if(context != JS_INVALID_REFERENCE) JsRelease(context, nullptr);
            JsDisposeRuntime(runtime);
        }
        if(library) FreeLibrary(library);
    }
    JsPropertyIdRef property(const wchar_t* name) {
        JsPropertyIdRef id;
        check(JsGetPropertyIdFromName(name, &id));
        return id;
    }
    JsValueRef get(JsValueRef object, const wchar_t* name) {
        JsValueRef value;
        check(JsGetProperty(object, property(name), &value));
        return value;
    }
    void set(JsValueRef object, const wchar_t* name, JsValueRef value) {
        check(JsSetProperty(object, property(name), value, true));
    }
    JsValueRef number(double value) {
        JsValueRef result;
        check(JsDoubleToNumber(value, &result));
        return result;
    }
    JsValueRef string(const std::string& value) {
        auto text = wide(value);
        JsValueRef result;
        check(JsPointerToString(text.data(), text.size(), &result));
        return result;
    }
    std::string text(JsValueRef value) {
        JsValueRef converted;
        check(JsConvertValueToString(value, &converted));
        const wchar_t* ptr;
        size_t length;
        check(JsStringToPointer(converted, &ptr, &length));
        return narrow(ptr, length);
    }
    JsValueRef indexed(JsValueRef object, unsigned index) {
        JsValueRef value;
        check(JsGetIndexedProperty(object, number(index), &value));
        return value;
    }
    unsigned length(JsValueRef object) {
        double size;
        check(JsNumberToDouble(get(object, L"length"), &size));
        if(size < 0 || size > 16000000) throw std::runtime_error("Invalid native array length");
        return (unsigned)size;
    }

    static void CALLBACK beforeCollect(JsRef, void* state) {
        auto& ref = *static_cast<NativeRef*>(state);
        ref.wrapper = JS_INVALID_REFERENCE;
        ref.owner->collected.push_back(ref.pointer);
    }
    void drainCollected() {
        // Release binding roots outside Chakra's GC callback. Native destructors
        // can run here, including destructors belonging to another script engine.
        auto pending = std::move(collected);
        collected.clear();
        for(void* pointer : pending) {
            auto found = handles.find(pointer);
            if(found == handles.end() || found->second->wrapper != JS_INVALID_REFERENCE) continue;
            duk_get_global_string(duk, "__jitNativeRefs");
            duk_del_prop_index(duk, -1, found->second->id);
            duk_pop(duk);
            handles.erase(found);
        }
    }

    // Keep the native object alive only while its JS proxy is reachable, and
    // preserve identity when a getter returns that object again.
    JsValueRef fromDuk(duk_idx_t index, unsigned depth = 0) {
        if(depth > 64) throw std::runtime_error("Native result nesting exceeds 64");
        index = duk_normalize_index(duk, index);
        JsValueRef result;
        if(duk_is_undefined(duk, index)) { check(JsGetUndefinedValue(&result)); return result; }
        if(duk_is_null(duk, index)) { check(JsGetNullValue(&result)); return result; }
        if(duk_is_boolean(duk, index)) { check(JsBoolToBoolean(duk_get_boolean(duk, index) != 0, &result)); return result; }
        if(duk_is_number(duk, index)) return number(duk_get_number(duk, index));
        if(duk_is_string(duk, index)) {
            duk_size_t count;
            const char* value = duk_get_lstring(duk, index, &count);
            return string(std::string(value, count));
        }
        if(!duk_is_object(duk, index)) throw std::runtime_error("Unsupported native return value");
        for(const char* name : classes) {
            duk_get_global_string(duk, name);
            bool instance = duk_instanceof(duk, index, -1) != 0;
            duk_pop(duk);
            if(!instance) continue;
            void* pointer = duk_get_heapptr(duk, index);
            auto found = handles.find(pointer);
            if(found != handles.end() && found->second->wrapper != JS_INVALID_REFERENCE)
                return found->second->wrapper;
            NativeRef* ref;
            if(found != handles.end()) ref = found->second.get();
            else {
                auto owned = std::make_unique<NativeRef>();
                ref = owned.get();
                ref->owner = this;
                ref->pointer = pointer;
                ref->id = nextHandle++;
                handles[pointer] = std::move(owned);
                duk_get_global_string(duk, "__jitNativeRefs");
                duk_dup(duk, index);
                duk_put_prop_index(duk, -2, ref->id);
                duk_pop(duk);
            }
            JsValueRef args[] = { global, number(ref->id), string(name) };
            check(JsCallFunction(get(global, L"__jitWrap"), args, 3, &result));
            check(JsSetObjectBeforeCollectCallback(result, ref, beforeCollect));
            ref->wrapper = result;
            return result;
        }
        if(duk_is_array(duk, index)) {
            unsigned count = (unsigned)duk_get_length(duk, index);
            check(JsCreateArray(count, &result));
            for(unsigned i = 0; i < count; ++i) {
                duk_get_prop_index(duk, index, i);
                auto value = fromDuk(-1, depth + 1);
                duk_pop(duk);
                check(JsSetIndexedProperty(result, number(i), value));
            }
        } else {
            check(JsCreateObject(&result));
            duk_enum(duk, index, DUK_ENUM_OWN_PROPERTIES_ONLY);
            while(duk_next(duk, -1, 1)) {
                auto key = wide(duk_get_string(duk, -2));
                auto value = fromDuk(-1, depth + 1);
                set(result, key.c_str(), value);
                duk_pop_2(duk);
            }
            duk_pop(duk);
        }
        return result;
    }

    void toDuk(JsValueRef value, unsigned depth = 0) {
        if(depth > 64) throw std::runtime_error("Native argument nesting exceeds 64");
        JsValueType type;
        check(JsGetValueType(value, &type));
        if(type == JsUndefined) { duk_push_undefined(duk); return; }
        if(type == JsNull) { duk_push_null(duk); return; }
        if(type == JsNumber) { double v; check(JsNumberToDouble(value, &v)); duk_push_number(duk, v); return; }
        if(type == JsBoolean) { bool v; check(JsBooleanToBool(value, &v)); duk_push_boolean(duk, v); return; }
        if(type == JsString) { auto v = text(value); duk_push_lstring(duk, v.data(), v.size()); return; }
        if(type != JsObject && type != JsArray && type != JsTypedArray)
            throw std::runtime_error("Unsupported native argument type");
        auto idValue = get(value, L"__jitNativeId");
        JsValueType idType;
        check(JsGetValueType(idValue, &idType));
        if(idType == JsNumber) {
            double id;
            check(JsNumberToDouble(idValue, &id));
            if(id < 0 || id >= nextHandle || id != (unsigned)id) throw std::runtime_error("Invalid native object handle");
            duk_get_global_string(duk, "__jitNativeRefs");
            duk_get_prop_index(duk, -1, (unsigned)id);
            duk_remove(duk, -2);
            return;
        }
        if(type == JsArray || type == JsTypedArray) {
            unsigned count = length(value);
            duk_push_array(duk);
            for(unsigned i = 0; i < count; ++i) {
                toDuk(indexed(value, i), depth + 1);
                duk_put_prop_index(duk, -2, i);
            }
        } else {
            JsValueRef keys;
            check(JsGetOwnPropertyNames(value, &keys));
            unsigned count = length(keys);
            duk_push_object(duk);
            for(unsigned i = 0; i < count; ++i) {
                auto key = text(indexed(keys, i));
                toDuk(get(value, wide(key).c_str()), depth + 1);
                duk_put_prop_lstring(duk, -2, key.data(), key.size());
            }
        }
    }

    JsValueRef raise(const std::exception& error) noexcept {
        try {
            JsValueRef exception;
            if(JsCreateError(string(error.what()), &exception) == JsNoError)
                JsSetException(exception);
        } catch(...) {}
        return JS_INVALID_REFERENCE;
    }

    static JsValueRef CALLBACK callMethod(JsValueRef, bool, JsValueRef* args, unsigned short count, void* state) {
        auto& method = *static_cast<Method*>(state);
        auto& self = *method.owner;
        duk_idx_t top = duk_get_top(self.duk);
        try {
            self.drainCollected();
            self.toDuk(args[0]);
            if(!duk_is_object(self.duk, -1)) throw std::runtime_error("Native method requires an object receiver");
            duk_get_prop_string(self.duk, -1, method.name.c_str());
            duk_dup(self.duk, -2);
            duk_remove(self.duk, -3);
            for(unsigned i = 1; i < count; ++i) self.toDuk(args[i]);
            if(duk_pcall_method(self.duk, count - 1) != DUK_EXEC_SUCCESS) {
                duk_get_prop_string(self.duk, -1, "stack");
                throw std::runtime_error(duk_safe_to_string(self.duk, -1));
            }
            auto result = self.fromDuk(-1);
            duk_set_top(self.duk, top);
            return result;
        } catch(const std::exception& error) {
            duk_set_top(self.duk, top);
            return self.raise(error);
        }
    }

    static JsValueRef CALLBACK bindMethod(JsValueRef, bool, JsValueRef* args, unsigned short count, void* state) {
        auto& self = *static_cast<Impl*>(state);
        try {
            if(count != 2) throw std::runtime_error("Invalid native method binding");
            auto method = std::make_unique<Method>();
            method->owner = &self;
            method->name = self.text(args[1]);
            JsValueRef result;
            check(self.JsCreateFunction(callMethod, method.get(), &result));
            self.methods.push_back(std::move(method));
            return result;
        } catch(const std::exception& error) { return self.raise(error); }
    }

    static JsValueRef CALLBACK bridge(JsValueRef, bool, JsValueRef* args, unsigned short count, void* state) {
        auto& self = *static_cast<Impl*>(state);
        duk_idx_t top = duk_get_top(self.duk);
        try {
            self.drainCollected();
            if(count != 5) throw std::runtime_error("Invalid native bridge call");
            duk_get_global_string(self.duk, "__jitDispatch");
            for(unsigned i = 1; i < count; ++i) self.toDuk(args[i]);
            if(duk_pcall(self.duk, 4) != DUK_EXEC_SUCCESS) {
                duk_get_prop_string(self.duk, -1, "stack");
                throw std::runtime_error(duk_safe_to_string(self.duk, -1));
            }
            auto result = self.fromDuk(-1);
            duk_set_top(self.duk, top);
            return result;
        } catch(const std::exception& error) {
            duk_set_top(self.duk, top);
            return self.raise(error);
        }
    }

    void initialize() {
        library = LoadLibraryExW(L"chakra.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if(!library) return;
#define LOAD_API(name) name = reinterpret_cast<decltype(name)>(GetProcAddress(library, #name)); if(!name) return;
        JIT_API_LIST(LOAD_API)
#undef LOAD_API
        check(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
        check(JsCreateContext(runtime, &context));
        check(JsAddRef(context, nullptr));
        CurrentContext current(*this);
        check(JsGetGlobalObject(&global));
        JsValueRef fn;
        check(JsCreateFunction(bridge, this, &fn));
        set(global, L"__jitBridge", fn);
        check(JsCreateFunction(bindMethod, this, &fn));
        set(global, L"__jitBindMethod", fn);

        const char* dispatch = R"JS(
var __jitNativeRefs = {};
function __jitDispatch(op, target, name, args) {
    var global = (function() { return this; })();
    var object = target === null ? global : __jitNativeRefs[target];
    if(op === 'get') return object[name];
    if(op === 'set') { object[name] = args[0]; return args[0]; }
    if(op === 'call') return object[name].apply(object, args);
    if(op === 'new') {
        var C = global[name];
        switch(args.length) {
        case 0: return new C();
        case 1: return new C(args[0]);
        case 2: return new C(args[0], args[1]);
        case 3: return new C(args[0], args[1], args[2]);
        default: throw new Error('Unsupported native constructor arity');
        }
    }
    throw new Error('Unknown native operation');
}
JSON.stringify(['FileIO','cPosition','sSprite','cRandomMap','cPhase','cMission','cCampaign','sGameData','cEngine'].map(function(name) {
    var proto = this[name].prototype, seen = {}, methods = [], properties = [];
    while(proto && proto !== Object.prototype) {
        Object.getOwnPropertyNames(proto).forEach(function(key) {
            if(key === 'constructor' || seen[key] || key.charCodeAt(0) === 255) return;
            seen[key] = true;
            var d = Object.getOwnPropertyDescriptor(proto, key);
            if(d.get || d.set) properties.push({name:key, read:!!d.get, write:!!d.set});
            else if(typeof d.value === 'function') methods.push(key);
        });
        proto = Object.getPrototypeOf(proto);
    }
    return {name:name, methods:methods, properties:properties};
}))
)JS";
        if(duk_peval_string(duk, dispatch) != DUK_EXEC_SUCCESS) {
            std::string error = duk_safe_to_string(duk, -1);
            duk_pop(duk);
            throw std::runtime_error(error);
        }
        std::string schema = duk_get_string(duk, -1);
        duk_pop(duk);
        std::string bootstrap = "var __jitSchema = " + schema + ";\n" + R"JS(
(function(global) {
    global.__jitWrap = function(id, name) {
        var value = Object.create(global[name].prototype);
        Object.defineProperty(value, '__jitNativeId', {value:id});
        return value;
    };
    __jitSchema.forEach(function(spec) {
        var C = function() {
            if(!(this instanceof C)) throw new TypeError('Native constructor requires new');
            return __jitBridge('new', null, spec.name, Array.prototype.slice.call(arguments));
        };
        spec.methods.forEach(function(name) {
            C.prototype[name] = __jitBindMethod(name);
        });
        spec.properties.forEach(function(p) {
            var d = {enumerable:true};
            if(p.read) d.get = function() { return __jitBridge('get', this.__jitNativeId, p.name, []); };
            if(p.write) d.set = function(v) { __jitBridge('set', this.__jitNativeId, p.name, [v]); };
            Object.defineProperty(C.prototype, p.name, d);
        });
        global[spec.name] = C;
    });
    ['print','consoleLog','consoleClear'].forEach(function(name) {
        global[name] = function() { return __jitBridge('call', null, name, Array.prototype.slice.call(arguments)); };
    });
})(this);
)JS";
        if(!execute(bootstrap, "native-jit-bindings")) throw std::runtime_error("JIT binding bootstrap failed");
        ready = true;
    }

    bool execute(const std::string& source, const std::string& filename) {
        CurrentContext current(*this);
        drainCollected();
        JsValueRef result;
        auto script = wide(source);
        auto file = wide(filename);
        auto error = JsRunScript(script.c_str(), sourceId++, file.c_str(), &result);
        if(error == JsNoError) return true;
        JsValueRef exception;
        if(JsGetAndClearException(&exception) == JsNoError) {
            auto stack = get(exception, L"stack");
            g_Debugger->Error("JIT " + filename + ": " + text(stack));
        } else g_Debugger->Error("JIT " + filename + ": error " + std::to_string((int)error));
        return false;
    }
};
#else
struct cJitScripting::Impl {};
#endif

cJitScripting::cJitScripting(duk_hthread* bindings) {
#ifdef _WIN32
    mImpl = std::make_unique<Impl>(bindings);
    try { mImpl->initialize(); }
    catch(const std::exception& error) { g_Debugger->Error(std::string("JIT initialization failed: ") + error.what()); mImpl.reset(); }
#endif
}
cJitScripting::~cJitScripting() = default;
bool cJitScripting::available() const {
#ifdef _WIN32
    return mImpl && mImpl->ready;
#else
    return false;
#endif
}
bool cJitScripting::run(const std::string& source, const std::string& filename) {
#ifdef _WIN32
    try { return mImpl->execute(source, filename); }
    catch(const std::exception& error) { g_Debugger->Error(error.what()); }
#endif
    return false;
}
std::string cJitScripting::libraryPath() const {
#ifdef _WIN32
    wchar_t path[32768];
    DWORD count = GetModuleFileNameW(mImpl->library, path, 32768);
    return count && count < 32768 ? narrow(path, count) : std::string();
#else
    return {};
#endif
}
void cJitScripting::syncEngine() {
#ifdef _WIN32
    Impl::CurrentContext current(*mImpl);
    duk_get_global_string(mImpl->duk, "Engine");
    auto value = mImpl->fromDuk(-1);
    duk_pop(mImpl->duk);
    mImpl->set(mImpl->global, L"Engine", value);
#endif
}
