#include "framework/AssetLocator.h"

#include <algorithm>
#include <system_error>

namespace openglgp::framework
{
namespace
{

void addUniqueRoot(
    std::vector<std::filesystem::path>& roots,
    const std::filesystem::path& path)
{
    if (path.empty())
    {
        return;
    }

    const auto normalized = path.lexically_normal();
    if (std::find(roots.begin(), roots.end(), normalized) == roots.end())
    {
        roots.push_back(normalized);
    }
}

} // namespace

AssetLocator::AssetLocator(const std::filesystem::path& executablePath)
{
    std::error_code error;
    if (!executablePath.empty())
    {
        auto executable = executablePath;
        if (executable.is_relative())
        {
            executable = std::filesystem::absolute(executable, error);
        }
        if (!error)
        {
            addUniqueRoot(roots_, executable.parent_path() / "res");
        }
    }

    error.clear();
    const auto currentPath = std::filesystem::current_path(error);
    if (!error)
    {
        addUniqueRoot(roots_, currentPath / "res");
    }

#ifdef OPENGLGP_SOURCE_ASSET_DIR
    addUniqueRoot(roots_, std::filesystem::path(OPENGLGP_SOURCE_ASSET_DIR));
#endif
}

std::optional<std::filesystem::path> AssetLocator::locate(
    const std::filesystem::path& relativePath) const
{
    std::error_code error;
    if (relativePath.is_absolute() && std::filesystem::exists(relativePath, error))
    {
        return relativePath.lexically_normal();
    }

    for (const auto& root : roots_)
    {
        const auto candidate = (root / relativePath).lexically_normal();
        error.clear();
        if (std::filesystem::exists(candidate, error))
        {
            return candidate;
        }
    }
    return std::nullopt;
}

const std::vector<std::filesystem::path>& AssetLocator::searchRoots() const noexcept
{
    return roots_;
}

} // namespace openglgp::framework
