#include "matgui/files.h"
//#include "engine/legacy/modelobjectshader.h"
//#include "engine/legacy/modelobjectshader.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace matgui {

namespace {

std::unordered_map<std::filesystem::path, std::string> builtinFiles;

}

std::unique_ptr<std::istream> openFile(std::filesystem::path path) {
    auto createStream = [](std::string str) {
        return std::make_unique<std::istringstream>(str);
    };

    if(auto f = builtinFiles.find(path); f != builtinFiles.end()) {
        return createStream(f->second);
    }

    return std::make_unique<std::ifstream>(path);
}

void registerFile(std::filesystem::path path, std::string_view content) {
    builtinFiles[path] = std::string(content);
}

FileRegister::FileRegister(std::filesystem::path path, std::string_view content)
{
    registerFile(path, content);
}

} // namespace engine
