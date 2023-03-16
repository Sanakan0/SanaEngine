#pragma once
#include "SRender/Resources/GLShader.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <SRender/Resources/SBaseRenderResources.h>
namespace SRender::Resources{



class SMaterial{
public:
    SMaterial(std::vector<TextureStack>& texs);
    SMaterial();
    void BindTexturebyidx(uint32_t idx,STexture* empty_tex);
    void UnBindTexture();
    // TextureStack& GetTextureStack(){return textures;}

    std::vector<TextureStack> texturelist_;
    std::string shaderfile_;
};


}