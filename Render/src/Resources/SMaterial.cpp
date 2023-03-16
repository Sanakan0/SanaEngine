#include "SRender/Resources/SMaterial.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Resources/STexture.h"
#include <stdint.h>
namespace SRender::Resources{

SMaterial::SMaterial(std::vector<TextureStack>& texs):
texturelist_(std::move(texs)){

}
SMaterial::SMaterial(){

}

void SMaterial::BindTexturebyidx(uint32_t idx,STexture* empty_tex){
    if (texturelist_.size()==0){
        for (int i=0;i<TEXTURESTACK_NUM;++i){
            empty_tex->Bind(i);
        }
    }else{
        texturelist_[idx].DiffuseTex?texturelist_[idx].DiffuseTex->Bind(0):empty_tex->Bind(0);
        texturelist_[idx].SpecularTex?texturelist_[idx].SpecularTex->Bind(1):empty_tex->Bind(1);
        texturelist_[idx].NormalsTex?texturelist_[idx].NormalsTex->Bind(2):empty_tex->Bind(2);
    }
    
}


void SMaterial::UnBindTexture(){
    glBindTexture(GL_TEXTURE_2D,0);
}


}


