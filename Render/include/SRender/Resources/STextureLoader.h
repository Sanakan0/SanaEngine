#pragma once
#include "SRender/Resources/STexture.h"
#include <stdint.h>
namespace SRender::Resources{

class STextureLoader{
public:
    STextureLoader() = delete;
    /*
    LoadFromFile_cached()
    Load Texture to memory but wont upload to gpu memory (for multithread loading)
    */
    static STexture* LoadFromFile_cached(const std::string& pth,const TextureInfo& texinfo=TextureInfo());
    
    //Load 2 memory and upload 2 gpu
    static STexture* LoadFromFile(const std::string& pth,const TextureInfo& texinfo=TextureInfo());
    static STexture* LoadFromMemory(void* data,uint32_t width,uint32_t height,const std::string& pth,const TextureInfo& texinfo=TextureInfo());
    static STexture* CreateColor(uint32_t color);
    static TexInternalFormat internalformat;
};

}