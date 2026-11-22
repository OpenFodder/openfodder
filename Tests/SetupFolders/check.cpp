// Exercise the production scanner with small, deliberately overlapping manifests.
// Resource-manager file operations use real temporary files; checksum tokens
// are their contents so the fixtures remain small and readable.
#include "stdafx.hpp"
#include "FileSystem.hpp"
#include "Setup/Validation.hpp"
#include "Setup/AutoScan.hpp"
#include <chrono>
#include <filesystem>
#include <stdexcept>
namespace fs = std::filesystem;
std::shared_ptr<cResourceMan> g_ResourceMan;
static fs::path testRoot;
cResourceMan::cResourceMan() = default;
bool cResourceMan::DirExists(const std::string& p) const {
    std::error_code ec; return fs::is_directory(FileSystem::Resolve(p), ec);
}
bool cResourceMan::FileExists(const std::string& p) const {
    std::error_code ec; return fs::is_regular_file(FileSystem::Resolve(p), ec);
}
std::vector<std::string> cResourceMan::DirectoryList(const std::string& p, const std::string& ext) {
    return FileSystem::List(p, ext, false);
}
std::string cResourceMan::FileMD5(const std::string& p) {
    std::ifstream in(FileSystem::Resolve(p)); std::string token; in >> token; return token;
}
std::vector<std::string> cResourceMan::getAllPaths() const { return {testRoot.u8string()}; }
std::string cResourceMan::getcwd() { return (testRoot / "run").u8string(); }
const sGameVersion KnownGameVersions[20] = {
{ "CF1", eGame::CF1, ePlatform::Amiga, eRelease::Retail, "Amiga", "CF1", {{"CFTITLE.RAW", "one"}, {"1.RAW", "one"}} },
{ "CF2", eGame::CF2, ePlatform::Amiga, eRelease::Retail, "Amiga2", "CF2", {{"CFTITLE.RAW", "two"}, {"1.RAW", "two"}} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} },
{ "Unused", eGame::CF1, ePlatform::Amiga, eRelease::Custom, "Custom", "", {} }
};
static void require(bool ok, const char* message) {
    if (!ok) throw std::runtime_error(message);
}
static void game(const fs::path& folder, const char* token) {
    fs::create_directories(folder);
    std::ofstream(folder / "cftitle.raw") << token;
    std::ofstream(folder / "1.raw") << token;
}
static void onlyCF1(const fs::path& folder, bool raw) {
    const auto result = Setup::ValidateFolder(folder.u8string());
    require(result.mMatches.size() == 1, "selected folder detects only CF1, no sibling CF2");
    const auto& match = result.mMatches.front();
    require(match.mVersion == &KnownGameVersions[0] && match.IsComplete(), "CF1 complete");
    require(match.mIsRawFolder == raw, "correct import vs search-root mode");
    require(fs::equivalent(fs::u8path(result.mResolvedRoot), folder), "selection never ascends");
}
int main() {
    testRoot = fs::temp_directory_path() / ("openfodder-folders-" +
        std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    try {
        g_ResourceMan = std::make_shared<cResourceMan>();
        fs::create_directories(testRoot / "run");
        game(testRoot / "installed/Data/Amiga", "one");
        game(testRoot / "installed/Data/Amiga2", "two");
        onlyCF1(testRoot / "installed/Data/Amiga", true);
        require(Setup::ValidateFolder((testRoot / "installed").u8string()).mMatches.size() == 2,
                "explicit installation root detects both contained games");
        require(Setup::ValidateFolder((testRoot / "installed/Data").u8string()).mMatches.size() == 2,
                "explicit Data folder detects its release children");
        game(testRoot / "extracted/Data", "one");
        onlyCF1(testRoot / "extracted", true);
        game(testRoot / "single/Data/Amiga", "one");
        onlyCF1(testRoot / "single", false);
        game(testRoot / "raw", "one");
        onlyCF1(testRoot / "raw", true);
        fs::create_directory(testRoot / "installed/empty");
        require(Setup::ValidateFolder((testRoot / "installed/empty").u8string()).mMatches.empty(),
                "empty selection never finds parent's games");
        game(testRoot / "Data/Amiga", "one");
        std::ofstream(testRoot / "unrelated.iso") << "unrelated";
        std::ofstream(testRoot / "run/unrelated.adf") << "unrelated";
        const auto hits = Setup::AutoScan();
        require(std::any_of(hits.begin(), hits.end(), [](const Setup::sScanCandidate& hit) {
            return fs::equivalent(fs::u8path(hit.mPath), testRoot);
        }), "automatic scan retains validated game-folder suggestions");
        for (const auto& hit : hits) {
            require(!hit.mIsImage, "automatic candidates never include unverified images");
            require(!hit.mResult.mMatches.empty(), "automatic suggestions are validated games");
        }
        std::ofstream(testRoot / "raw/cftitle.raw") << "wrong";
        std::ofstream(testRoot / "raw/1.raw") << "wrong";
        require(Setup::ValidateFolder((testRoot / "raw").u8string()).mMatches.empty(),
                "overlapping filenames without matching bytes are rejected");
        require(Setup::ValidateFolder("").mMatches.empty(), "empty selection is not cwd");
        require(Setup::ValidateFolder((testRoot / "missing").u8string()).mMatches.empty(),
                "missing selection is not cwd");
        fs::remove_all(testRoot);
        std::cout << "Folder scope, extraction layouts, release identity and image suggestions passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        fs::remove_all(testRoot);
        return 1;
    }
}
