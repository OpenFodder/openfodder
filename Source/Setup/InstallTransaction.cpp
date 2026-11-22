#include "Setup/InstallTransaction.hpp"

#include <stdexcept>
#include <system_error>
#include <vector>

namespace Setup {
namespace fs = std::filesystem;

InstallTransaction::InstallTransaction(const fs::path& target, bool bundle) : mBundle(bundle) {
    if (target.empty())
        throw std::runtime_error("installation target is empty");
    mTarget = fs::absolute(target).lexically_normal();
    if (mTarget.filename().empty() || mTarget == mTarget.root_path())
        throw std::runtime_error("installation target must be a named directory");
    mWork = bundle ? mTarget / ".openfodder-update" :
        mTarget.parent_path() / ("." + mTarget.filename().string() + ".openfodder-update");
    mStaging = mWork / "staging";
    mArchive = mWork / "release.zip";
    mBackup = mWork / "backup";
}

InstallTransaction::~InstallTransaction() {
    if (!mOwnWork)
        return;
    // Only remove the fixed work directory that this transaction created.
    // Never delete a backup if restoring the live target failed.
    std::error_code ignored;
    if (!mKeepBackup)
        fs::remove_all(mWork, ignored);
}

void InstallTransaction::Prepare() {
    fs::create_directories(mTarget.parent_path());
    if (mBundle) fs::create_directories(mTarget);
    // create_directory is exclusive across processes. A work directory left
    // after a killed process is retained for recovery, never silently erased.
    if (!fs::create_directory(mWork))
        throw std::runtime_error("another update is running or an interrupted update needs recovery at " + mWork.string());
    mOwnWork = true;
    if (fs::is_symlink(fs::symlink_status(mTarget)))
        throw std::runtime_error("cannot replace a symbolic-link installation: " + mTarget.string());
    if (fs::exists(mTarget)) {
        if (!fs::is_directory(mTarget))
            throw std::runtime_error("installation target is not a directory: " + mTarget.string());
        // Preserve local retail data and user files. Links are copied as links,
        // not followed; ZipArchive checks each destination before overwriting.
        if (mBundle) {
            fs::create_directory(mStaging);
            for (const auto* name : {"Data", "Scripts", "Campaigns", "about.png", "installed.json"}) {
                const auto source = mTarget / name;
                if (fs::is_symlink(fs::symlink_status(source)))
                    throw std::runtime_error("cannot replace symbolic-link bundle member: " + source.string());
                if (fs::exists(source))
                    fs::copy(source, mStaging / name, fs::copy_options::recursive | fs::copy_options::copy_symlinks);
            }
        } else {
            fs::copy(mTarget, mStaging, fs::copy_options::recursive | fs::copy_options::copy_symlinks);
        }
    } else {
        fs::create_directory(mStaging);
    }
}

void InstallTransaction::Commit() {
    if (!mOwnWork)
        throw std::runtime_error("installation has not been prepared");
    if (mBundle) {
        // Never rename the runtime root: it may contain the running executable
        // or be the current working directory. Publish only managed resources.
        fs::create_directory(mBackup);
        std::vector<std::string> backedUp, published;
        try {
            for (const auto* name : {"Data", "Scripts", "Campaigns", "about.png", "installed.json"}) {
                if (fs::exists(mTarget / name)) {
                    fs::rename(mTarget / name, mBackup / name);
                    backedUp.emplace_back(name);
                }
                fs::rename(mStaging / name, mTarget / name);
                published.emplace_back(name);
            }
        } catch (const std::exception& ex) {
            const std::string failure = ex.what();
            std::error_code error;
            // Move new members back to staging rather than deleting live paths.
            for (auto it = published.rbegin(); it != published.rend(); ++it) {
                fs::rename(mTarget / *it, mStaging / *it, error);
                if (error) { mKeepBackup = true; break; }
            }
            if (!mKeepBackup) {
                for (auto it = backedUp.rbegin(); it != backedUp.rend(); ++it) {
                    fs::rename(mBackup / *it, mTarget / *it, error);
                    if (error) { mKeepBackup = true; break; }
                }
            }
            throw std::runtime_error(failure + (mKeepBackup ?
                "; rollback incomplete; recovery retained at " + mWork.string() : "; previous bundle restored"));
        }
        return;
    }
    if (fs::exists(mTarget)) {
        fs::rename(mTarget, mBackup);
        mKeepBackup = true;
    }
    std::error_code error;
    fs::rename(mStaging, mTarget, error);
    if (error) {
        const std::string failure = "could not publish installation: " + error.message();
        if (mKeepBackup) {
            std::error_code rollbackError;
            fs::rename(mBackup, mTarget, rollbackError);
            if (rollbackError)
                throw std::runtime_error(failure + "; could not restore previous installation: " +
                                         rollbackError.message() + "; backup retained at " + mBackup.string());
            mKeepBackup = false;
        }
        throw std::runtime_error(failure);
    }
    // Both the contents and installed.json are now from the completed stage.
    mKeepBackup = false;
}

} // namespace Setup
