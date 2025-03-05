/*
 * texture.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace matgui {

// The texture class is supposed to be used by value
// Because the textureId is kept in a shared_ptr the background texture is freed
// when needed
class Texture {
public:
    enum Interpolation { Linear = 0, Nearest = 1 };

    struct Pixel {
        unsigned char r = 0, g = 0, b = 0, a = 255;
        Pixel(unsigned char r,
              unsigned char g,
              unsigned char b,
              unsigned char a = 255)
            : r(r), g(g), b(b), a(a) {
        }
        Pixel(unsigned int c, unsigned char a)
            : Pixel(c & 0xFF, (c >> 8) & 0xFF, (c >> 8 * 2) & 0xFF, a) {
        }
        Pixel(unsigned int c)
            : Pixel(c & 0xFF, (c >> 8) & 0xFF, (c >> 8 * 2) & 0xFF) {
        }
        Pixel() = default;
    };

    Texture();
    Texture(const std::string filename, bool addToLibrary = true);
    Texture(const Texture &texture) = default;
    Texture(Texture &&texture) = default;

    virtual ~Texture();

    // Load a texture by filename.
    // If addToLibrary is true the texture is buffered and only loaded
    // from disk once
    void load(const std::string path, bool addToLibrary = true);
    void load(const std::filesystem::path path, bool addToLibrary = true) {
        load(path.string(), addToLibrary);
    }
    void load(const char *path, bool addToLibrary = true) {
        load(std::string{path}, addToLibrary);
    }
    void load(std::string_view path, bool addToLibrary = true) {
        load(std::string{path}, addToLibrary);
    }

    void createBitmap(const std::vector<Pixel> &pixels,
                      int width = 1,
                      int height = 1,
                      std::string name = "");
    void createGrayscale(const std::vector<float> &grayScale,
                         int width = 1,
                         int height = 1,
                         std::string name = "");
    void clear() {
        _texturePtr.reset();
    }

    void bind() const;
    static void unbind();

    unsigned int texture() const {
        return (unsigned long)_texturePtr.get();
    }

    // Transfer ownership of raw reference to this object
    // the reference is handled by the texture object
    void texture(unsigned int texture);

    operator unsigned int() const {
        return texture();
    }

    // Check if this is the only instance pointing at the current texture
    bool unique() const {
        return _texturePtr.unique();
    }

    Texture &operator=(const Texture &) = default;
    Texture &operator=(Texture &&) = default;

    void interpolation(Interpolation);

    void name(std::string name) {
        _name = std::move(name);
    }

    std::string_view name() const {
        return _name;
    }

    template <typename Arch>
    void save(Arch &arch) {
        arch("name", _name);
    }

    template <typename Arch>
    void load(Arch &arch) {
        arch("name", _name);
        load(_name);
    }

private:
    std::shared_ptr<void> _texturePtr;
    std::string _name;
};

} /* namespace matgui */
