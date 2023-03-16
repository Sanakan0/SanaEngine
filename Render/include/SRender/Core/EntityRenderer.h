#pragma once
#include <SRender/Core/GLRenderer.h>
#include "ECS/Component/Component.h"
#include "ECS/Actor.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Resources/SMaterial.h"
#include "SRender/Resources/STexture.h"
#include <SRender/Resources/SModel.h>
#include <SRender/Settings/GLSet.h>
#include <cstddef>
#include <memory>
#include <stdint.h>
namespace SRender::Core{

class EntityRenderer:public GLRenderer{
public:
    EntityRenderer();
    ~EntityRenderer();
    void DrawSkeleton(Resources::SModel& model);
    void DrawActorOutline(ECS::Actor& actor,float linewidth=5.0f);
    void DrawModelwithEmptyTex(Resources::SModel &model,Resources::STexture* empty_tex=nullptr);
    void DrawModel(Resources::SModel& model);
    GLShapeDrawer* GetShapeDrawer(){return &shapedrawer_;}

    
private:
    std::unique_ptr<Resources::STexture> emptytexture_;
    std::unique_ptr<Resources::STexture> outline_colored_texture_;
    GLShapeDrawer shapedrawer_;
    std::unique_ptr<Resources::GLShader> unlitshader_;
    Resources::SMaterial empty_mat_;
};

}