#include"SRender/Resources/STexture.h"
#include"stb_image/stb_image.h"
#include"stb_image/stb_image_resize.h"
#include <stdint.h>
#include <stdlib.h>
namespace SRender::Resources{

STexture::STexture(uint32_t pid,int pwidth,int pheight,const std::string& ppath,GLenum pminfilter,GLenum pmagfilter,bool mipmap_generate,unsigned char* prawdata,TexInternalFormat pinternal):
id(pid),width(pwidth),height(pheight),path(ppath),minfilter(pminfilter),magfilter(pmagfilter),internalformat(pinternal),mipmap(mipmap_generate),rawdata(prawdata){}

STexture::~STexture(){
    glDeleteTextures(1,&id);
    if (rawdata){
        free(rawdata);
    }
}

void STexture::Bind(uint32_t slot)const{
    glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void STexture::Unbind()const{
    glBindTexture(GL_TEXTURE_2D,0);
}

bool STexture::LoadFromDisk(){
    FreeRawData();
    stbi_set_flip_vertically_on_load(true);
    int filechannel;
    rawdata = stbi_load(path.c_str(),&width,&height,&filechannel,4);
    if (rawdata == nullptr) return false;
    if (height>2048){
        int clampedheight = 2048;
        int clampedwidth = (int)(width*(2048.0/height));
        unsigned char* resizedata = (unsigned char*)malloc(4*clampedheight*clampedwidth);
        stbir_resize_uint8_srgb(rawdata, width, height, 0, resizedata,clampedwidth , clampedheight,0,4, 3, 0);
        free(rawdata);
        rawdata = resizedata;
        width=clampedwidth;
        height=clampedheight;
        resizedata=nullptr;
    }
    return true;
}

void STexture::UploadTexture(){
    //if (rawdata==nullptr) return;
    


    glGenTextures(1,&id);
    
    glBindTexture(GL_TEXTURE_2D,id);
    
    glTexImage2D(GL_TEXTURE_2D,0,static_cast<GLint>(internalformat),width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,rawdata);
    if(mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);

    glBindTexture(GL_TEXTURE_2D,0);
    
}

void STexture::FreeRawData(){
    free(rawdata);
    rawdata=nullptr;
}

}