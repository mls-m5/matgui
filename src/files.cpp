#include "matgui/files.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

namespace matgui {

namespace {

struct PathHasher {
    std::size_t operator()(const std::filesystem::path &path) const {
        return std::hash<std::string>()(path.string());
    }
};

std::unordered_map<std::filesystem::path, std::string, PathHasher> builtinFiles;

} // namespace

std::unique_ptr<std::istream> openFile(std::filesystem::path path) {
    auto createStream = [](std::string str) {
        return std::make_unique<std::istringstream>(str);
    };

    if (auto f = builtinFiles.find(path); f != builtinFiles.end()) {
        return createStream(f->second);
    }

    return std::make_unique<std::ifstream>(path);
}

void registerFile(std::filesystem::path path, std::string_view content) {
    builtinFiles[path] = std::string(content);
}

FileRegister::FileRegister(std::filesystem::path path,
                           std::string_view content) {
    registerFile(path, content);
}

std::string loadFile(std::filesystem::path path) {
    if (auto f = builtinFiles.find(path); f != builtinFiles.end()) {
        return std::string{f->second};
    }

    size_t size = std::filesystem::file_size(path);
    std::string content;
    content.resize(size);

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file.");
    }

    file.read(&content[0], size);
    return content;
}

} // namespace matgui
