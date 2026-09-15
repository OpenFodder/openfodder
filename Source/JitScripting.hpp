#pragma once

#include <memory>
#include <string>

struct duk_hthread;

// Optional JIT execution of game scripts. Existing Duktape/dukglue bindings
// continue to own native objects and enforce the C++ argument contracts.
class cJitScripting {
    struct Impl;
    std::unique_ptr<Impl> mImpl;
public:
    explicit cJitScripting(duk_hthread* bindings);
    ~cJitScripting();
    bool available() const;
    std::string libraryPath() const;
    bool run(const std::string& source, const std::string& filename);
    void syncEngine();
};
