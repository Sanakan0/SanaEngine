#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include <SRender/Core/EntityRenderer.h>
#include <memory>

namespace SRender::Core{


EntityRenderer::EntityRenderer():shapedrawer_(*static_cast<GLRenderer*>(this)){
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glStencilFunc(GL_ALWAYS,1,0x00);
};

EntityRenderer::~EntityRenderer(){}

void EntityRenderer::DrawSkeleton(Resources::SModel& model){
    glm::mat4 scale=  glm::scale(glm::mat4(1),glm::vec3(0.1));
    for (int i=0;i<model.GetJoints().size();++i){
        shapedrawer_.DrawArrow(model.palette_[i]*glm::inverse(model.GetJoints()[i].inverse_bind_mat)*scale);
    }
}

void EntityRenderer::DrawModel(Resources::SModel& model){
    auto& meshes = model.GetMeshes();
    auto& materials=model.GetMaterials();
    for (int i=0;i<meshes.size();++i){
        //bind texture
        materials[i].data[0]->Bind(0);
        //draw sth
        Draw(*meshes[i], Setting::SPrimitive::TRIANGLES);
        materials[i].data[0]->Unbind();
    }
}

void EntityRenderer::Draw2Stencil(Resources::SModel& model){
    glStencilMask(0xFF);
    DrawModel(model);
    glStencilMask(0x00);
}



}