// Exercise the production scanner with small, deliberately overlapping manifests.
// Uses real temporary files and real MD5 checksums without requiring game assets.
#include "stdafx.hpp"
#include "FileSystem.hpp"
#include "Setup/Validation.hpp"
#include "Setup/AutoScan.hpp"
#include "Amiga/dernc.hpp"
#include <chrono>
#include <filesystem>
#include <stdexcept>
namespace fs = std::filesystem;
std::shared_ptr<cResourceMan> g_ResourceMan;
static fs::path testRoot;
static std::vector<const sGameVersion*> available;
std::vector<const sGameVersion*> cResourceMan::GetAvailable() const { return available; }
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
{ "CF1", eGame::CF1, ePlatform::Amiga, eRelease::Retail, "Amiga", "CF1", {{"CFTITLE.RAW", "F97C5D29941BFB1B2FDAB0874906AB82"}, {"1.RAW", "F97C5D29941BFB1B2FDAB0874906AB82"}} },
{ "CF2", eGame::CF2, ePlatform::Amiga, eRelease::Retail, "Amiga2", "CF2", {{"CFTITLE.RAW", "B8A9F715DBB64FD5C56E7783C6820A61"}, {"1.RAW", "B8A9F715DBB64FD5C56E7783C6820A61"}} },
{ "CF1 CD32", eGame::CF1, ePlatform::Amiga, eRelease::Retail, "Amiga_CD", "CD32", {{"CFTITLE.RAW", "657CE4B58B26FD92B13FAA4F4098C8E8"}, {"1.RAW", "657CE4B58B26FD92B13FAA4F4098C8E8"}} },
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
static void packedFile(const fs::path& path, const std::string& text) {
    require(text.size() == 3, "literal RNC fixture length");
    std::vector<uint8> payload = {12,128,0,32,0,64};
    payload.insert(payload.end(), text.begin(), text.end());
    std::vector<uint8> packed(18);
    packed[0] = 'R'; packed[1] = 'N'; packed[2] = 'C'; packed[3] = 1;
    packed[7] = 3; packed[11] = (uint8)payload.size();
    const auto unpackedCrc = rnc_crc(const_cast<char*>(text.data()), (long)text.size());
    const auto packedCrc = rnc_crc(payload.data(), (long)payload.size());
    packed[12] = uint8(unpackedCrc >> 8); packed[13] = uint8(unpackedCrc);
    packed[14] = uint8(packedCrc >> 8); packed[15] = uint8(packedCrc);
    packed[17] = 1;
    packed.insert(packed.end(), payload.begin(), payload.end());
    std::ofstream output(path, std::ios::binary);
    output.write(reinterpret_cast<const char*>(packed.data()), packed.size());
}
static void onlyCF1(const fs::path& folder, bool raw) {
    const auto result = Setup::ValidateFolder(folder.u8string());
    require(result.mMatches.size() == 1, "selected folder detects only CF1, no sibling CF2");
    const auto& match = result.mMatches.front();
    require(match.mVersion == &KnownGameVersions[0] && match.IsComplete(), "CF1 complete");
    require(match.mIsRawFolder == raw, "correct import vs search-root mode");
    require(match.mResolvedPath.find('\\') == std::string::npos,
            "release paths use consistent separators for collection deduplication");
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
        for (const auto& selected : {testRoot / "installed", testRoot / "installed/Data"}) {
            const auto choices = Setup::FolderCandidates(Setup::ValidateFolder(selected.u8string()), "Selected");
            require(choices.size() == 2, "one selectable entry for each game in a folder");
            for (size_t i = 0; i < choices.size(); ++i) {
                const auto& choice = choices[i];
                require(choice.mResult.mMatches.size() == 1, "candidate describes exactly one game");
                require(choice.mResult.mMatches.front().mVersion == &KnownGameVersions[i], "distinct releases retained");
                const auto again = Setup::ValidateFolder(choice.mPath);
                require(again.mMatches.size() == 1 && again.mMatches.front().mVersion == &KnownGameVersions[i],
                        "choosing a game revalidates only its own release folder");
                require(fs::equivalent(fs::u8path(choice.mPath),
                        testRoot / "installed/Data" / KnownGameVersions[i].mDataPath), "candidate shows actual release path");
            }
        }
        auto editions = Setup::ValidateFolder((testRoot / "installed/Data/Amiga").u8string());
        auto weakerEdition = editions.mMatches.front();
        weakerEdition.mVersion = &KnownGameVersions[1];
        weakerEdition.mFilesMd5Checked = 4;
        weakerEdition.mFilesMd5Match = 3;
        editions.mMatches.push_back(weakerEdition);
        // The fixture's second version is a different game, so it must not
        // be dropped merely because another game has a better checksum ratio.
        require(Setup::FolderCandidates(editions, "Editions").size() == 2,
                "checksum ranking does not hide a different game in the same folder");
        auto sameGameEditions = Setup::ValidateFolder((testRoot / "installed/Data/Amiga").u8string());
        auto cd32 = sameGameEditions.mMatches.front();
        cd32.mVersion = &KnownGameVersions[2];
        cd32.mFilesMd5Checked = 4;
        cd32.mFilesMd5Match = 3;
        sameGameEditions.mMatches.push_back(cd32);
        require(Setup::FolderCandidates(sameGameEditions, "Editions").size() == 1,
                "floppy/CD32 editions are distinguished by their checksum evidence");
        Setup::ExcludeAvailable(sameGameEditions, {&KnownGameVersions[0]});
        require(sameGameEditions.mMatches.empty() && sameGameEditions.mAlreadyAvailable,
                "installed stronger edition does not expose a weaker false-positive edition");
        auto overlappingDemo = Setup::ValidateFolder((testRoot / "installed/Data/Amiga").u8string());
        auto partialDemo = overlappingDemo.mMatches.front();
        partialDemo.mVersion = &KnownGameVersions[1];
        ++partialDemo.mFilesExpected;
        overlappingDemo.mMatches.push_back(partialDemo);
        const auto demoChoices = Setup::FolderCandidates(overlappingDemo, "Selected");
        require(demoChoices.size() == 1 && demoChoices.front().mResult.mMatches.front().IsComplete(),
                "shared files do not create a second partial demo beside a complete release");
        game(testRoot / "extracted/Data", "one");
        onlyCF1(testRoot / "extracted", true);
        game(testRoot / "single/Data/Amiga", "one");
        onlyCF1(testRoot / "single", false);
        game(testRoot / "raw", "one");
        onlyCF1(testRoot / "raw", true);
        game(testRoot / "compressed", "one");
        packedFile(testRoot / "compressed/cftitle.raw", "one");
        packedFile(testRoot / "compressed/1.raw", "one");
        onlyCF1(testRoot / "compressed", true);
        {
            std::ifstream input(testRoot / "compressed/1.raw", std::ios::binary);
            std::vector<uint8> bytes((std::istreambuf_iterator<char>(input)), {});
            uint8 output[] = {0xA5, 0, 0xA5};
            bytes[7] = 1; // Declared output smaller than its valid literal run.
            require(rnc_unpack_checked(bytes.data(), bytes.size(), output + 1, 1) < 0 &&
                    output[0] == 0xA5 && output[2] == 0xA5, "RNC probe enforces output bounds");
            bytes[7] = 3;
            bytes.pop_back();
            require(rnc_unpack_checked(bytes.data(), bytes.size(), output, sizeof(output)) < 0,
                    "RNC probe rejects truncated packed streams");
        }
        {
            uint32 random = 0x12345678;
            for (int sample = 0; sample < 1000; ++sample) {
                const size_t payloadSize = 1 + sample % 64;
                const size_t outputSize = 1 + sample % 63;
                std::vector<uint8> bytes(18 + payloadSize);
                bytes[0] = 'R'; bytes[1] = 'N'; bytes[2] = 'C'; bytes[3] = 1;
                bytes[7] = uint8(outputSize); bytes[11] = uint8(payloadSize); bytes[17] = 1;
                for (size_t n = 18; n < bytes.size(); ++n) {
                    random = random * 1664525u + 1013904223u;
                    bytes[n] = uint8(random >> 24);
                }
                const auto crc = rnc_crc(bytes.data() + 18, (long)payloadSize);
                bytes[14] = uint8(crc >> 8); bytes[15] = uint8(crc);
                std::vector<uint8> output(outputSize + 2, 0xA5);
                rnc_unpack_checked(bytes.data(), bytes.size(), output.data() + 1, outputSize);
                require(output.front() == 0xA5 && output.back() == 0xA5,
                        "malformed CRC-valid RNC streams stay within output bounds");
            }
        }
        fs::create_directory(testRoot / "installed/empty");
        require(Setup::ValidateFolder((testRoot / "installed/empty").u8string()).mMatches.empty(),
                "empty selection never finds parent's games");
        game(testRoot / "Data/Amiga", "one");
        game(testRoot / "Data/Amiga2", "two");
        fs::create_directories(testRoot / "Data/Amiga_CD");
        std::ofstream(testRoot / "Data/Amiga_CD/cftitle.raw") << "cd32";
        std::ofstream(testRoot / "unrelated.iso") << "unrelated";
        std::ofstream(testRoot / "run/unrelated.adf") << "unrelated";
        const auto hits = Setup::AutoScan();
        require(std::count_if(hits.begin(), hits.end(), [](const Setup::sScanCandidate& hit) {
            return fs::equivalent(fs::u8path(hit.mPath), testRoot) && hit.mResult.mMatches.size() == 2;
        }) == 1, "automatic scan groups both games under one deduplicated base folder");
        for (const auto& hit : hits) {
            require(!hit.mIsImage, "automatic candidates never include unverified images");
            require(!hit.mResult.mMatches.empty(), "automatic suggestions are validated games");
        }
        available = {&KnownGameVersions[0]};
        const auto mixed = Setup::AutoScan();
        require(mixed.size() == 1 && mixed.front().mResult.mMatches.size() == 1 &&
                mixed.front().mResult.mMatches.front().mVersion == &KnownGameVersions[1],
                "installed releases are omitted from automatic suggestions and collection counts");
        auto ownFolder = Setup::ValidateFolder((testRoot / "Data/Amiga").u8string());
        Setup::ExcludeAvailable(ownFolder, available);
        require(ownFolder.mMatches.empty() && ownFolder.mAlreadyAvailable, "explicit installed folder is already added");
        require(ownFolder.mAvailableMatches.size() == 1 &&
                fs::equivalent(fs::u8path(ownFolder.mAvailableMatches.front().mResolvedPath), testRoot / "Data/Amiga"),
                "already-added feedback retains the actual discovered location");
        Setup::ExcludeAvailable(ownFolder, available);
        require(ownFolder.mAlreadyAvailable && ownFolder.mAvailableMatches.size() == 1,
                "repeated filtering preserves already-added feedback without duplicates");
        available.push_back(&KnownGameVersions[1]);
        require(Setup::AutoScan().empty(), "own data folder is not offered when all games are installed");
        available.clear();
        require(Setup::ValidateFolderCollection(testRoot.u8string(), [] { return true; }).mMatches.empty(),
                "cancelled directory check stops without publishing partial matches");
        game(testRoot / "collection/First game", "one");
        game(testRoot / "collection/Second game/Data", "two");
        game(testRoot / "collection/Partial files", "one");
        fs::remove(testRoot / "collection/Partial files/1.raw");
        fs::create_directories(testRoot / "collection/Unrelated");
        std::ofstream(testRoot / "collection/Unrelated/random.iso") << "not a game";
        const auto collection = Setup::FolderCandidates(
            Setup::ValidateFolderCollection((testRoot / "collection").u8string()), "Selected");
        require(collection.size() == 2, "picked collection detects complete games without promoting partial file overlaps");
        for (const auto& choice : collection)
            require(choice.mResult.mMatches.front().IsComplete(), "only validated child games in collection");
        require(Setup::FolderCandidates(Setup::ValidateFolderCollection((testRoot / "Data").u8string()), "Data").size() == 2,
                "collection scanning does not duplicate canonical release subfolders");
        game(testRoot / "deep/outer/inner", "one");
        game(testRoot / "deep/other", "two");
        game(testRoot / "deep/too/far/away", "one");
        const auto nested = Setup::ValidateFolderCollection((testRoot / "deep").u8string());
        require(nested.mMatches.size() == 2,
                "collection discovery finds children and grandchildren, but stops before the third level");
        require(std::any_of(nested.mMatches.begin(), nested.mMatches.end(), [&](const Setup::sVersionMatch& match) {
            return fs::equivalent(fs::u8path(match.mResolvedPath), testRoot / "deep/outer/inner");
        }), "grandchild release is found at its actual path");
        require(Setup::ValidateFolderCollection((testRoot / "deep/too").u8string()).mMatches.size() == 1,
                "depth is measured from any selected folder");
        game(testRoot / "release-with-support", "one");
        game(testRoot / "release-with-support/support/other", "two");
        require(Setup::ValidateFolderCollection((testRoot / "release-with-support").u8string()).mMatches.size() == 1,
                "a specific release does not scan its support folders as more games");
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
