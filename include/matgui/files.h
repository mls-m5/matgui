#pragma once

#include <filesystem>
#include <istream>
#include <memory>

namespace matgui {

/// A wrapper file to open streams that represents files
/// The returned value could also return built in data
std::unique_ptr<std::istream> openFile(std::filesystem::path);

/// Set up a file in memory
void registerFile(std::filesystem::path path, std::string_view content);

struct FileRegister {
    FileRegister(std::filesystem::path, std::string_view content);
};


} // namespace engine
