#include "SRender/Resources/STextureLoader.h"
#include "SRender/Resources/STexture.h"
#include <stb_image/stb_image.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdint.h>
namespace SRender::Resources{

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
    stbi_image_free(rawdata);
    return res;
}
STexture* STextureLoader::LoadFromMemory(void* data,uint32_t width,uint32_t height,const std::string& pth,GLenum minfilter,GLenum magfilter,bool mipmap){
    uint32_t id;
    glGenTextures(1,&id);
    
    glBindTexture(GL_TEXTURE_2D,id);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    if(mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);

    glBindTexture(GL_TEXTURE_2D,0);
    return new STexture(id,width,height,pth,mipmap);
}

}