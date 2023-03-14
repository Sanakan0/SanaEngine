#include "SRender/Resources/SMaterial.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Resources/STexture.h"
namespace SRender::Resources{




void SMaterial::BindTextures(){
    for (int i=0;i<TEXTURESTACK_NUM;++i){
        auto tmp = reinterpret_cast<STexture*>(&textures+sizeof(STexture*)*i); //test me
        if (tmp!=nullptr) tmp->Bind(i);
    }
}


}


