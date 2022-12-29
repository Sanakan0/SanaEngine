#pragma once
#include "SRender/Resources/GLShader.h"
#include <map>
#include <stdint.h>
#include <string>
#include <any>
namespace SRender::Resources{

class STexture{
friend class STextureLoader;
public:
    STexture(uint32_t pid,uint32_t pwidth,uint32_t pheight,const std::string& ppath,bool mipmap_generate);
    ~STexture();
    void Bind(uint32_t slot);
    void Unbind();
    uint32_t id;
    uint32_t width;
    uint32_t height;
    uint32_t bitsperpixel;
    std::string path;
    bool enablemipmap;
};



}