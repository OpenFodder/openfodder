#include "Setup/InstallTransaction.hpp"

#include <stdexcept>
#include <system_error>

namespace Setup {
namespace fs = std::filesystem;

InstallTransaction::InstallTransaction(const fs::path& target) {
    if (target.empty())
        throw std::runtime_error("installation target is empty");
    mTarget = fs::absolute(target).lexically_normal();
    if (mTarget.filename().empty() || mTarget == mTarget.root_path())
        throw std::runtime_error("installation target must be a named directory");
    mWork = mTarget.parent_path() / ("." + mTarget.filename().string() + ".openfodder-update");
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
        fs::copy(mTarget, mStaging, fs::copy_options::recursive | fs::copy_options::copy_symlinks);
    } else {
        fs::create_directory(mStaging);
    }
}

void InstallTransaction::Commit() {
    if (!mOwnWork)
        throw std::runtime_error("installation has not been prepared");
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
