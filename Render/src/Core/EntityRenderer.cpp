#include "ECS/Component/MeshComponent.h"
#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SShader.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include <SRender/Core/EntityRenderer.h>
#include <memory>
#include <stdint.h>

namespace SRender::Core{


EntityRenderer::EntityRenderer():
shapedrawer_(*static_cast<GLRenderer*>(this)),
emptytexture_(Resources::STextureLoader::CreateColor((255<<24)|(255<<16)|(255<<8)|255)),
outline_colored_texture_(Resources::STextureLoader::CreateColor((255<<24)|(128<<16)|(57<<8)|237)),
unlitshader_(Resources::GLShaderLoader::LoadFromFile(ResourceManager::Util::GetFullPath(":shaders\\unlit.glsl"))){
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glStencilFunc(GL_ALWAYS,1,0xFF);
};

EntityRenderer::~EntityRenderer(){}

void EntityRenderer::DrawSkeleton(Resources::SModel& model){
    ApplyGLstate(Skeleton_Default_GLstate);
    glm::mat4 scale=  glm::scale(glm::mat4(1),glm::vec3(0.1));
    for (int i=0;i<model.GetJoints().size();++i){
        shapedrawer_.DrawArrow(model.palette_[i]*glm::inverse(model.GetJoints()[i].inverse_bind_mat)*scale);
    }
    ApplyPreviousGLstate();
}

void EntityRenderer::DrawModel(Resources::SModel& model){
    auto& meshes = model.GetMeshes();
    auto material=model.GetMaterial();
    for (int i=0;i<meshes.size();++i){
        //bind texture
        material->BindTexturebyidx(meshes[i]->material_idx_,emptytexture_.get());
        //draw sth
        Draw(*meshes[i], Setting::SPrimitive::TRIANGLES);
        material->UnBindTexture();
    }
}

void EntityRenderer::DrawModelwithEmptyTex(Resources::SModel &model,Resources::STexture* empty_tex){
    auto& meshes = model.GetMeshes();
    if (empty_tex==nullptr)empty_tex=emptytexture_.get();
    for (int i=0;i<meshes.size();++i){
        //bind texture
        empty_mat_.BindTexturebyidx(meshes[i]->material_idx_,empty_tex);
        //draw sth
        Draw(*meshes[i], Setting::SPrimitive::TRIANGLES);
        empty_mat_.UnBindTexture();
    }
}

void EntityRenderer::DrawActorOutline(ECS::Actor& actor,float linewidth){
    
    auto model = static_cast<ECS::Components::MeshComponent*>(
        actor.GetComponent("MeshComponent"))->GetModel();
    
    auto transcomp = actor.GetTransformComponent();
    
    
    
    unlitshader_->Bind();

    if (transcomp) {
        unlitshader_->SetUniMat4("ModelMat", transcomp->GetMat());
    }else{
        unlitshader_->SetUniMat4("ModelMat", glm::mat4(1));
    }

    
    //draw to stencil
    ApplyGLstate(Stencil_GLstate);
    glStencilMask(0xFF);
    DrawModel(*model);
    glStencilMask(0x00);
    ApplyPreviousGLstate();
    //draw outline
    ApplyGLstate(Outline_GLstate);
    SetRasterizationMode(Setting::SRasterization::LINE);
    SetRasterizationLineWdith(linewidth);
    glStencilFunc(GL_NOTEQUAL,1,0XFF);
    
    
    DrawModelwithEmptyTex(*model, outline_colored_texture_.get());


    glStencilFunc(GL_ALWAYS,1,0xFF);
    SetRasterizationLineWdith(1.0);
    SetRasterizationMode(Setting::SRasterization::FILL);
    ApplyPreviousGLstate();
    unlitshader_->Unbind();
    
}



}