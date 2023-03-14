#pragma once
#include "SRender/Resources/GLShader.h"
#include <vector>
#include <string>
#include <SRender/Resources/SBaseRenderResources.h>
namespace SRender::Resources{



class SMaterial{
public:
    void BindTextures();
    GLShader* GetShader(){return shader_;}
    void SetShader(GLShader* shader){shader_=shader;}
    TextureStack& GetTextureStack(){return textures;}
private:
    TextureStack textures;
    GLShader* shader_;


};


}