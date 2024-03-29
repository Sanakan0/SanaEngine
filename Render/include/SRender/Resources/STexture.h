#pragma once
#include "SRender/Resources/GLShader.h"
#include <map>
#include <stdint.h>
#include <string>
#include <any>
#include "SRender/Resources/Settings.h"
namespace SRender::Resources{



class STexture{
friend class STextureLoader;
public:
    STexture(uint32_t pid,int pwidth,int pheight,const std::string& ppath,GLenum pminfilter,GLenum pmagfilter,bool mipmap_generate,unsigned char* prawdata=nullptr,TexInternalFormat pinternal=TexInternalFormat::Default);
    ~STexture();
    void Bind(uint32_t slot) const;
    void Unbind()const;
    bool LoadFromDisk();
    void UploadTexture();
    void FreeRawData();
    uint32_t id;
    int width;
    int height;
    int bitsperpixel;
    std::string path;
    GLenum minfilter,magfilter;
    TexInternalFormat internalformat;
    bool mipmap;
    unsigned char* rawdata;
};



}