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
    STexture(uint32_t pid,int pwidth,int pheight,const std::string& ppath,GLenum pminfilter,GLenum pmagfilter,bool mipmap_generate,unsigned char* prawdata=nullptr);
    ~STexture();
    void Bind(uint32_t slot);
    void Unbind();
    void LoadFromDisk();
    void UploadTexture();
    uint32_t id;
    int width;
    int height;
    int bitsperpixel;
    std::string path;
    GLenum minfilter,magfilter;
    bool mipmap;
    unsigned char* rawdata;
};



}