#include <SRender/Passes/SceneRenderPass.h>
#include "ECS/Component/MaterialComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/LowRenderer/Camera.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"

namespace SRender::Passes{


SceneRenderPass::SceneRenderPass():
renderer_(SANASERVICE(Core::EntityRenderer)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    std::string pth = ResourceManager::PathManager::GetFullPath(":shaders\\unlit.glsl");
    unlit_shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
    pth = ResourceManager::PathManager::GetFullPath(":shaders\\standard.glsl");
    standard_shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
    pth = ResourceManager::PathManager::GetFullPath(":shaders\\distortion.glsl");
    distortion_shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
    shaderp_ = unlit_shaderp_.get();
}
void SceneRenderPass::Draw(){
    
    
    if (distortioninfo_&&distortioninfo_->dist_type!=LowRenderer::DistortionModel::NONE){
        shaderp_=distortion_shaderp_.get();
        shaderp_->SetUniFloatV("DistInfo.dist_para",const_cast<float*>(distortioninfo_->dist_para),3);
        shaderp_->SetUniInt("DistInfo.dist_type",(int)distortioninfo_->dist_type);
    }else{
        shaderp_=unlit_shaderp_.get();

    }
    shaderp_->Bind();

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
    shaderp_->Unbind();
   
}

void SceneRenderPass::BindDistortionInfo(const LowRenderer::Camera& cam){
    distortioninfo_ = &cam.distortion_;
}


}