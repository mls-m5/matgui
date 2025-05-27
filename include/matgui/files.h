#pragma once

#include <filesystem>
#include <istream>
#include <memory>

namespace matgui {

/// A wrapper file to open streams that represents files
/// The returned value could also return built in data
std::unique_ptr<std::istream> openFile(std::filesystem::path);

/// Loads a file into memory
std::string loadFile(std::filesystem::path);

/// Set up a file in memory
void registerFile(std::filesystem::path path, std::string_view content);

// Check if a cached or regular file exists
bool doesFileExist(std::filesystem::path path);

struct FileRegister {
    FileRegister(std::filesystem::path, std::string_view content);
};

} // namespace matgui
