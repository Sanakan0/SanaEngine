#pragma once
#include <SRender/Core/GLRenderer.h>
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include <SRender/Resources/SModel.h>
#include <SRender/Settings/GLSet.h>
#include <cstddef>
#include <memory>
namespace SRender::Core{

class EntityRenderer:public GLRenderer{
public:
    EntityRenderer();
    ~EntityRenderer();
    void DrawSkeleton(Resources::SModel& model);
    void Draw2Stencil(Resources::SModel& model);
    void DrawModel(Resources::SModel& model);
    GLShapeDrawer* GetShapeDrawer(){return &shapedrawer_;}
private:
    GLShapeDrawer shapedrawer_;
};

}