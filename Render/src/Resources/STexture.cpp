#include"SRender/Resources/STexture.h"
#include"stb_image/stb_image.h"
#include <stdint.h>
namespace SRender::Resources{

STexture::STexture(uint32_t pid,int pwidth,int pheight,const std::string& ppath,GLenum pminfilter,GLenum pmagfilter,bool mipmap_generate,unsigned char* prawdata):
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

void STexture::LoadFromDisk(){
    int bitsPerPixel;
    stbi_set_flip_vertically_on_load(true);
    rawdata = stbi_load(path.c_str(),&width,&height,&bitsPerPixel,4);
}

void STexture::UploadTexture(){
    //if (rawdata==nullptr) return;
    


    glGenTextures(1,&id);
    
    glBindTexture(GL_TEXTURE_2D,id);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,rawdata);
    if(mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);

    glBindTexture(GL_TEXTURE_2D,0);
    free(rawdata);
    rawdata=nullptr;
}

}