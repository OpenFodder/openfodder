#pragma once

#include <filesystem>
#include <string>

namespace Setup {

// Builds a replacement beside the target (same filesystem), retaining the old
// tree until publication succeeds. A failed rollback keeps its backup for repair.
class InstallTransaction {
public:
    explicit InstallTransaction(const std::filesystem::path& target);
    ~InstallTransaction();
    InstallTransaction(const InstallTransaction&) = delete;
    InstallTransaction& operator=(const InstallTransaction&) = delete;

    void Prepare();
    void Commit();
    const std::filesystem::path& Staging() const { return mStaging; }
    const std::filesystem::path& Archive() const { return mArchive; }

private:
    std::filesystem::path mTarget, mWork, mStaging, mArchive, mBackup;
    bool mOwnWork = false;
    bool mKeepBackup = false;
};

} // namespace Setup
