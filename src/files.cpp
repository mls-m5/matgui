#include "matgui/files.h"
#include <filesystem>
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

auto &builtinFiles() {
    static std::unordered_map<std::filesystem::path, std::string, PathHasher>
        builtinFiles;
    return builtinFiles;
}

} // namespace

std::unique_ptr<std::istream> openFile(std::filesystem::path path) {
    auto createStream = [](std::string str) {
        return std::make_unique<std::istringstream>(str);
    };

    auto &files = builtinFiles();

    if (auto f = files.find(path); f != files.end()) {
        return createStream(f->second);
    }

    auto file = std::make_unique<std::ifstream>(path);
    if (!file->is_open()) {
        return nullptr;
    }

    return file;
}

void registerFile(std::filesystem::path path, std::string_view content) {
    builtinFiles()[path] = std::string(content);
}

FileRegister::FileRegister(std::filesystem::path path,
                           std::string_view content) {
    registerFile(path, content);
}

std::string loadFile(std::filesystem::path path) {
    auto &files = builtinFiles();
    if (auto f = files.find(path); f != files.end()) {
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

bool doesFileExist(std::filesystem::path path) {
    auto &files = builtinFiles();
    if (auto f = files.find(path); f != files.end()) {
        return true;
    }

    return std::filesystem::exists(path);
}

} // namespace matgui
