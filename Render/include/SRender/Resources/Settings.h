#pragma once
#include <glad/glad.h>
namespace SRender::Resources{


enum class TexInternalFormat:GLint{
    Default = GL_RGBA8,
    DXT5 = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
};


struct TextureInfo{
    GLenum minfilter {GL_LINEAR};
    GLenum magfilter {GL_LINEAR};
    bool mipmap_generate {false};
    TexInternalFormat internal {TexInternalFormat::Default};
};


struct ModelLoadSetting{
    SRender::Resources::TextureInfo texinfo;
};



}