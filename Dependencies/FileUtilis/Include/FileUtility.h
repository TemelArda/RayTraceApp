#pragma once
#include <unordered_map>
#include <string>

namespace FileUtility
{

[[nodiscard]] const std::unordered_multimap<std::string, std::string> ParseFile(
    const std::string& fileName);
}// namespace FileUtility