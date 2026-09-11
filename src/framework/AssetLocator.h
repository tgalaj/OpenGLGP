#pragma once

#include <filesystem>
#include <optional>
#include <vector>

namespace openglgp::framework
{

class AssetLocator
{
public:
    explicit AssetLocator(const std::filesystem::path& executablePath);

    [[nodiscard]] std::optional<std::filesystem::path> locate(const std::filesystem::path& relativePath) const;
    [[nodiscard]] const std::vector<std::filesystem::path>& searchRoots() const noexcept;

private:
    std::vector<std::filesystem::path> roots_;
};

} // namespace openglgp::framework
