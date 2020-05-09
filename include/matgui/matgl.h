/*
 * matgl.h
 * A wrapper around some openGL-objects to improve productivity
 * and increase joy of programming
 *
 *  Created on: 6 maj 2019
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "common-gl.h"
#include "gl-error-handling.h"
#include "shaderprogram.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Vec;

namespace GL {

template <typename T>
GLenum getType();

#ifdef GL_DOUBLE
template <>
inline GLenum getType<double>() {
    return GL_DOUBLE;
}
#endif

template <>
inline GLenum getType<float>() {
    return GL_FLOAT;
}

template <>
inline GLenum getType<GLuint>() {
    return GL_UNSIGNED_INT;
}

template <>
inline GLenum getType<GLint>() {
    return GL_INT;
}

template <>
inline GLenum getType<GLubyte>() {
    return GL_UNSIGNED_BYTE;
}

template <>
inline GLenum getType<GLbyte>() {
    return GL_BYTE;
}

template <>
inline GLenum getType<::Vec>() {
    return GL_DOUBLE;
}

//! Like unique_ptr but for unsigned ints
//! Used to be able to easier move objects
template <typename Deleter>
class UniqueGlPointer {

public:
    UniqueGlPointer() = default;

    UniqueGlPointer(GLuint ptr) : ptr(ptr) {
    }

    UniqueGlPointer(UniqueGlPointer &) = delete;

    UniqueGlPointer(UniqueGlPointer &&other) : ptr(other.ptr) {
        other.ptr = 0;
    }

    UniqueGlPointer &operator=(GLuint ptr) {
        reset();
        this->ptr = ptr;
    }

    UniqueGlPointer &operator=(const UniqueGlPointer &other) = delete;

    UniqueGlPointer &operator=(UniqueGlPointer &&other) {
        reset();
        ptr = other.ptr;
        other.ptr = 0;
        return *this;
    }

    ~UniqueGlPointer() {
        reset();
    }

    // Delete content if set and reset pointer
    void reset() {
        if (ptr) {
            glCall(Deleter(ptr));
        }
        ptr = 0;
    }

    GLuint get() const {
        return ptr;
    }

    GLuint &get() {
        return ptr;
    }

    operator GLuint() const {
        return ptr;
    }

    operator GLuint &() {
        return ptr;
    }

    GLuint ptr = 0;
};

class VertexArrayObject {
public:
    // Create a VAO _and_ bind it
    VertexArrayObject() {
        init();
    }

    // For creating the object later
    VertexArrayObject(std::nullptr_t) {
    }

    VertexArrayObject(const VertexArrayObject &) = delete;
    VertexArrayObject(VertexArrayObject &&other) = default;
    ~VertexArrayObject() = default;

    void init() {
        glCall(glGenVertexArrays(1, &ptr.get()));
        bind();
    }

    void bind() {
        glCall(glBindVertexArray(ptr));
    }

    void unbind() {
        glCall(glBindVertexArray(0));
    }

    void clear() {
        ptr.reset();
    }

    operator GLuint() {
        return ptr;
    }

private:
    struct DeleteVertexArray {
        void operator()(GLuint p) {
            glCall(glDeleteVertexArrays(1, &p));
        }
    };

    UniqueGlPointer<DeleteVertexArray> ptr;
};

class VertexBufferObject {
public:
    // GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER is the most common
    // More info on
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
    VertexBufferObject(GLenum target = GL_ARRAY_BUFFER) : target(target) {
        glCall(glGenBuffers(1, &ptr.get()));
        bind();
    }

    // Shorthand version
    // if size == 0 it will be calculated from the size of the vector
    // index is the index in the shader program
    // elementSize is the size of each element eg 3 for 3d-position
    // stride is how far it is between each element in bytes
    //   can for example be calculated by sizeof(T) * element size
    // start is the starting position in bytes
    template <class T>
    VertexBufferObject(const std::vector<T> &data,
                       GLuint index,
                       GLuint elementSize = 3,
                       GLuint stride = 0,
                       size_t start = 0,
                       GLenum target = GL_ARRAY_BUFFER,
                       GLenum usage = GL_STATIC_DRAW)
        : VertexBufferObject(target) {
        setData(data, usage);
        attribPointer(
            index, elementSize, getType<T>(), false, stride, (void *)start);
    }

    // The same as above but for a standard array
    // dataSize: The number of values in the data
    template <class T>
    VertexBufferObject(const T *data,
                       size_t dataSize,
                       GLuint index,
                       GLuint elementSize = 3,
                       GLuint stride = 0,
                       size_t start = 0,
                       GLenum target = GL_ARRAY_BUFFER,
                       GLenum usage = GL_STATIC_DRAW)
        : VertexBufferObject(target) {
        setData(data, dataSize, usage);
        attribPointer(
            index, elementSize, getType<T>(), false, stride, (void *)start);
    }

    // A specialized version for element buffers
    VertexBufferObject(const std::vector<GLuint> &indices)
        : VertexBufferObject(GL_ELEMENT_ARRAY_BUFFER) {
        setData(indices);
    }

    VertexBufferObject(VertexBufferObject &&other) = default;
    VertexBufferObject(const VertexBufferObject &) = delete;
    VertexBufferObject &operator=(VertexBufferObject &other) = delete;
    VertexBufferObject &operator=(VertexBufferObject &&other) = default;

    ~VertexBufferObject() = default;

    void bind() {
        glCall(glBindBuffer(target, ptr));
    }

    void unbind() {
        glCall(glBindBuffer(target, 0));
    }

    void clear() {
        ptr.reset();
    }

    // Set attribute pointer
    // and enable it if it is not GL_ELEMENT_ARRAY_BUFFER
    // Index is the index in the shader program
    // size is the total number of variables for each element eg. 3 for a 3d
    // position type is the data type eg. GL_FLOAT or GL_DOUBLE stride is if you
    // have some other format like and needs the size of each element to be
    // bigger pointer is the offset of the first element, probably used in
    // combination with stride
    void attribPointer(GLuint index,
                       GLint size,
                       GLenum type = GL_FLOAT,
                       GLboolean normalized = false,
                       GLsizei stride = 0,
                       const void *pointer = nullptr) {
        glCall(glVertexAttribPointer(
            index, size, type, normalized, stride, pointer));
        if (type != GL_ELEMENT_ARRAY_BUFFER) {
            glCall(glEnableVertexAttribArray(index));
        }
    }

    // Binds and set data
    template <class T>
    void setData(const std::vector<T> &data, GLenum usage = GL_STATIC_DRAW) {
        bind();
        glCall(glBufferData(
            target, sizeof(T) * data.size(), &data.front(), usage));
    }

    // Binds and set data
    template <class T>
    void setData(const T *data, size_t size, GLenum usage = GL_STATIC_DRAW) {
        if (!data || !size) {
            return;
        }
        bind();
        glCall(glBufferData(target, sizeof(T) * size, data, usage));
    }

    operator GLuint() const {
        return ptr.get();
    }

private:
    struct Deleter {
        void operator()(GLuint p) {
            glDeleteBuffers(1, &p);
        }
    };

    UniqueGlPointer<Deleter> ptr;
    GLenum target;
};

#if MAT_GL_VERSION >= 3

class FrameBufferObject {
public:
    FrameBufferObject(int width, int height) : width(width), height(height) {
        glCall(glGenFramebuffers(1, &ptr.get()));
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, ptr));
        glCall(glDrawBuffer(GL_COLOR_ATTACHMENT0));
    }

    // Setup opengl to render to this framebuffer
    void bind() {
        glBindTexture(GL_TEXTURE_2D, 0); // Make sure to unbind any textures
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, ptr));
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Framebuffer is not complete");
        }
        glCall(glViewport(0, 0, width, height));
    }

    // Sets opengl to render to the default framebuffer (the screen)
    static void unBind() {
        glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    static void unBind(int w, int h) {
        unBind();
        glCall(glViewport(0, 0, w, h));
    }

    ~FrameBufferObject() = default;

    operator GLuint() const {
        return ptr;
    }

private:
    struct Deleter {
        void operator()(GLuint p) {
            glDeleteFramebuffers(1, &p);
        }
    };
    UniqueGlPointer<Deleter> ptr;
    int width, height;
};

class TextureAttachment {
public:
    TextureAttachment(int width,
                      int height,
                      GLenum attachment = GL_COLOR_ATTACHMENT0) {
        glCall(glGenTextures(1, &ptr.get()));
        bind();
        glCall(glTexImage2D(GL_TEXTURE_2D,
                            0,
                            GL_RGB,
                            width,
                            height,
                            0,
                            GL_RGB,
                            GL_UNSIGNED_BYTE,
                            nullptr));
        glCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCall(glFramebufferTexture(GL_FRAMEBUFFER, attachment, ptr, 0));
        unbind();
    }

    TextureAttachment(const TextureAttachment &) = delete;
    TextureAttachment(TextureAttachment &&) = default;
    TextureAttachment &operator=(const TextureAttachment &) = delete;
    TextureAttachment &operator=(TextureAttachment &&) = default;

    ~TextureAttachment() = default;

    void bind() {
        glCall(glBindTexture(GL_TEXTURE_2D, ptr.get()));
    }

    void unbind() {
        glCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    operator GLuint() const {
        return ptr;
    }

private:
    struct Deleter {
        void operator()(GLuint p) {
            glCall(glDeleteTextures(1, &p));
        }
    };

    UniqueGlPointer<Deleter> ptr;
};

// A depth buffer that may be used to render somewhere else
class DepthTextureAttachment {
public:
    DepthTextureAttachment(int width, int height) {
        glCall(glGenTextures(1, &ptr.get()));
        bind();
        glCall(glTexImage2D(GL_TEXTURE_2D,
                            0,
                            GL_DEPTH_COMPONENT32,
                            width,
                            height,
                            0,
                            GL_DEPTH_COMPONENT,
                            GL_UNSIGNED_INT,
                            nullptr));
        glCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCall(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCall(
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ptr, 0));
        unbind();
    }

    DepthTextureAttachment(const DepthTextureAttachment &) = delete;
    DepthTextureAttachment(DepthTextureAttachment &&) = default;
    DepthTextureAttachment &operator=(const DepthTextureAttachment &) = delete;
    DepthTextureAttachment &operator=(DepthTextureAttachment &&) = default;
    ~DepthTextureAttachment() = default;

    void bind() {
        glCall(glBindTexture(GL_TEXTURE_2D, ptr));
    }

    void unbind() {
        glCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    operator GLuint() const {
        return ptr;
    }

private:
    struct Deleter {
        void operator()(GLuint p) {
            glCall(glDeleteTextures(1, &p));
        }
    };
    UniqueGlPointer<Deleter> ptr;
};

// A depth buffer that is not used to render to anywhere else
class DepthBufferAttachment {
public:
    DepthBufferAttachment(int width, int height) {
        glGenRenderbuffers(1, &ptr.get());
        bind();
        glCall(glRenderbufferStorage(
            GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
        glCall(glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ptr));
        unbind();
    }

    DepthBufferAttachment(const DepthBufferAttachment &) = delete;
    DepthBufferAttachment(DepthBufferAttachment &&) = default;
    DepthBufferAttachment &operator=(const DepthBufferAttachment &) = delete;
    DepthBufferAttachment &operator=(DepthBufferAttachment &&) = default;
    ~DepthBufferAttachment() = default;

    void bind() {
        glCall(glBindRenderbuffer(GL_RENDERBUFFER, ptr));
    }

    void unbind() {
        glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }

    operator GLuint() const {
        return ptr;
    }

private:
    struct Deleter {
        void operator()(GLuint p) {
            glCall(glDeleteRenderbuffers(1, &p));
        }
    };

    UniqueGlPointer<Deleter> ptr;
};

#endif

class Texture {
public:
    Texture() {
        glGenTextures(1, &ptr.get());
    }

    // Shorthand function
    // Possible formats: GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and
    // GL_LUMINANCE_ALPHA
    template <typename T>
    Texture(const std::vector<T> &data,
            int width,
            int height,
            GLenum format = GL_RGB,
            GLenum interpolationType = GL_LINEAR,
            bool generateMipmap = true)
        : Texture() {
        setData(data, width, height, format, interpolationType);
        if (generateMipmap) {
            this->generateMipmap();
        }
        setWrap();
    }

    // Shorthand function
    // Possible formats: GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and
    // GL_LUMINANCE_ALPHA
    template <typename T>
    Texture(const T *data,
            int width,
            int height,
            GLenum format = GL_RGB,
            GLenum interpolationType = GL_LINEAR,
            bool generateMipmap = true)
        : Texture() {
        setData(data, width, height, format, interpolationType);
        if (generateMipmap) {
            this->generateMipmap();
        }
        setWrap();
    }

    // Possible formats: GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and
    // GL_LUMINANCE_ALPHA
    template <typename T>
    void setData(const std::vector<T> &data,
                 int width,
                 int height,
                 GLenum format = GL_RGB,
                 GLenum interpolationType = GL_LINEAR) {
        setData(&data.front(), width, height, format, interpolationType);
    }

    // Possible formats: GL_ALPHA, GL_RGB, GL_RGBA, GL_LUMINANCE, and
    // GL_LUMINANCE_ALPHA
    template <typename T>
    void setData(const T *data,
                 int width,
                 int height,
                 GLenum format = GL_RGB,
                 GLenum interpolationType = GL_LINEAR) {
        bind();
        glCall(glTexImage2D(GL_TEXTURE_2D,
                            0,
                            format,
                            width,
                            height,
                            0,
                            format,
                            getType<T>(),
                            data));
        setInterpolationType(interpolationType);
    }

    // Calls glTexSubImage
    template <typename T>
    void updateData(const T *data,
                    int xoffset,
                    int yoffset,
                    int width,
                    int height,
                    GLenum format = GL_RGB) {
        bind();
        glCall(glTexSubImage2D(GL_TEXTURE_2D,
                               0,
                               xoffset,
                               yoffset,
                               width,
                               height,
                               format,
                               getType<T>(),
                               data));
    }

    template <typename T>
    void updateData(const std::vector<T> &data,
                    int xoffset,
                    int yoffset,
                    int width,
                    int height,
                    GLenum format = GL_RGB) {
        updateData(&data.front(), xoffset, yoffset, width, height, format);
    }

    void generateMipmap() {
        bind();
        glCall(glGenerateMipmap(GL_TEXTURE_2D));
    }

    Texture(const Texture &other) = delete;
    Texture(Texture &&other) = default;
    Texture &operator=(const Texture &other) = delete;
    Texture &operator=(Texture &&other) = default;
    ~Texture() = default;

    void bind() {
        glBindTexture(GL_TEXTURE_2D, ptr);
    }

    // Note that you need to bind before using these functions
    static void setParameteri(GLenum paramName, GLint value) {
        glTexParameteri(GL_TEXTURE_2D, paramName, value);
    }

    static void setWrap() {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    // GL_LINEAR GL_NEAREST_MIPMAP_NEAREST GL_LINEAR_MIPMAP_NEAREST
    // GL_NEAREST_MIPMAP_LINEAR GL_LINEAR_MIPMAP_LINEAR
    static void setInterpolationType(GLenum type) {
        setParameteri(GL_TEXTURE_MIN_FILTER, type);
        setParameteri(GL_TEXTURE_MAG_FILTER, type);
    }

    operator GLuint() {
        return ptr;
    }

private:
    struct Deleter {
        void operator()(GLuint p) {
            glDeleteTextures(1, &p);
        }
    };
    UniqueGlPointer<Deleter> ptr;
};

} // namespace GL
