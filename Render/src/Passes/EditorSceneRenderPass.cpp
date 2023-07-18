#include <SRender/Passes/EditorSceneRenderPass.h>
#include "ECS/Component/MaterialComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"

namespace SRender::Passes{


EditorSceneRenderPass::EditorSceneRenderPass():
renderer_(SANASERVICE(Core::EntityRenderer)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    std::string pth = ResourceManager::PathManager::GetFullPath(":shaders\\unlit.glsl");
    unlit_shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
    pth = ResourceManager::PathManager::GetFullPath(":shaders\\standard.glsl");
    standard_shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
    DisableNormal();
}

void EditorSceneRenderPass::EnableNormal(){
    shaderp_ = standard_shaderp_.get();
}

void EditorSceneRenderPass::DisableNormal(){
    shaderp_ = unlit_shaderp_.get();
}

void EditorSceneRenderPass::Draw(){
    shaderp_->Bind();
    shaderp_->SetUniFloat("k", k);
    glm::mat4 tmpmodel = glm::mat4(1);
    for (auto& meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
        auto transcomp =meshcomp->parentactor_.GetTransformComponent();

        if (transcomp) {
            shaderp_->SetUniMat4("ModelMat", transcomp->GetMat());
        }else{
            shaderp_->SetUniMat4("ModelMat", tmpmodel);
        }
        renderer_.DrawModel(*meshcomp->GetModel());
    }
    renderer_.GetShapeDrawer()->DrawGrid();
    for (auto& camcomp:scenemanager_.GetScene()->GetBasicRenderComponent().camcomps){
        auto transcomp =camcomp->parentactor_.GetTransformComponent();
        bool is_active = (&camcomp->parentactor_ == scenemanager_.GetActiveCamera()) ? true:false;
        renderer_.GetShapeDrawer()->DrawCamFrame(transcomp->GetMat(), camcomp->cam_.Getfocal_length()/camcomp->cam_.Getsensor_size_h(), camcomp->cam_.Getaspect_ratio(), {0,0,0,1},is_active);
    }
    
    //renderer_.GetShapeDrawer()->DrawArrow(glm::translate(glm::mat4(1),scenemanager_.cursor_pos_));
    shaderp_->Unbind();
   
}





}