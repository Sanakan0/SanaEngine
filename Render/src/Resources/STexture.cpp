#include"SRender/Resources/STexture.h"

namespace SRender::Resources{

STexture::STexture(uint32_t pid,uint32_t pwidth,uint32_t pheight,const std::string& ppath,GLenum pminfilter,GLenum pmagfilter,bool mipmap_generate,unsigned char* prawdata):
id(pid),width(pwidth),height(pheight),path(ppath),minfilter(pminfilter),magfilter(pmagfilter),mipmap(mipmap_generate),rawdata(prawdata){}

STexture::~STexture(){
    glDeleteTextures(1,&id);
    if (rawdata){
        free(rawdata);
    }
}

void STexture::Bind(uint32_t slot){
    glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void STexture::Unbind(){
    glBindTexture(GL_TEXTURE_2D,0);
}

void STexture::UploadTexture(){
    glGenTextures(1,&id);
    
    glBindTexture(GL_TEXTURE_2D,id);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,rawdata);
    if(mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);

    glBindTexture(GL_TEXTURE_2D,0);
    free(rawdata);
}

}