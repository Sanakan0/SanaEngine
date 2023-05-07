#include "SRender/Resources/STextureLoader.h"
#include "SRender/Resources/STexture.h"
#include <memory>
#include <stb_image/stb_image.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdint.h>
namespace SRender::Resources{

STexture* STextureLoader::LoadFromFile_cached(const std::string &pth,GLenum minfilter,GLenum magfilter,bool mipmap){
	int width;
	int height;
	int bitsPerPixel;
    
    STexture* res=new STexture(0,0,0,pth,minfilter,magfilter,mipmap,nullptr);
    return res;
    return nullptr;
}

STexture* STextureLoader::LoadFromFile(const std::string &pth,GLenum minfilter,GLenum magfilter,bool mipmap){
	int width;
	int height;
	int bitsPerPixel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* rawdata = stbi_load(pth.c_str(),&width,&height,&bitsPerPixel,4);
    STexture* res=nullptr;
    if(rawdata!=nullptr){
        res = LoadFromMemory(static_cast<void*>(rawdata),width, height,pth,minfilter, magfilter, mipmap);
    }
    return res;
}
STexture* STextureLoader::LoadFromMemory(void* data,uint32_t width,uint32_t height,const std::string& pth,GLenum minfilter,GLenum magfilter,bool mipmap){
    auto tmpuniq = std::make_unique<STexture>(0,width,height,pth,minfilter,magfilter,mipmap,(unsigned char*)data);
    tmpuniq->UploadTexture();
    return tmpuniq.release();
}
STexture* STextureLoader::CreateColor(uint32_t color){
    auto tmpp = (int*)malloc(sizeof(uint32_t));
    *tmpp = color;
    return LoadFromMemory(tmpp, 1, 1, "color");
}


}