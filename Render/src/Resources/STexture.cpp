#include"SRender/Resources/STexture.h"

namespace SRender::Resources{

STexture::STexture(uint32_t pid,uint32_t pwidth,uint32_t pheight,const std::string& ppath,bool mipmap_generate):
id(pid),width(pwidth),height(pheight),path(ppath),enablemipmap(mipmap_generate){}

STexture::~STexture(){
    glDeleteTextures(1,&id);
}

void STexture::Bind(uint32_t slot){
    glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
}

void STexture::Unbind(){
    glBindTexture(GL_TEXTURE_2D,0);
}

}