#include <SRender/Passes/ActorOutlineRenderPass.h>
#include "ECS/Actor.h"
#include "ECS/Component/TransformComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"

namespace SRender::Passes{


ActorOutlineRenderPass::ActorOutlineRenderPass():
renderer_(SANASERVICE(Core::EntityRenderer)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    std::string pth = ResourceManager::Util::GetFullPath(":shaders\\unlit.glsl");
    shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
}
void ActorOutlineRenderPass::Draw(ECS::Actor& actor){
    shaderp_->Bind();

    glm::mat4 tmpmodel = glm::mat4(1);
    for (auto& meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
        auto transcomp =static_cast<ECS::Components::TransformComponent*>(
            meshcomp->parentactor_.GetComponent("TransformComponent"));
        
        if (transcomp) {
            shaderp_->SetUniMat4("ModelMat", transcomp->GetMat());
        }else{
            shaderp_->SetUniMat4("ModelMat", tmpmodel);
        }
        renderer_.DrawModel(*meshcomp->GetModel());
    }
    shaderp_->Unbind();
}





}