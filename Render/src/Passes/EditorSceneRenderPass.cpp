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
    std::string pth = ResourceManager::Util::GetFullPath(":shaders\\unlit.glsl");
    shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
}
void EditorSceneRenderPass::Draw(){
    shaderp_->Bind();
    shaderp_->SetUniFloat("k", k);
    glm::mat4 tmpmodel = glm::mat4(1);
    for (auto& meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
        auto transcomp =meshcomp->parentactor_.GetTransformComponent();
        auto material = static_cast<ECS::Components::MaterialComponent*>(
            meshcomp->parentactor_.GetComponent("MaterialComponent"))->GetMaterial();
        Resources::GLShader* shaderp = material->GetShader()?material->GetShader():shaderp_.get();
        if (transcomp) {
            shaderp->SetUniMat4("ModelMat", transcomp->GetMat());
        }else{
            shaderp->SetUniMat4("ModelMat", tmpmodel);
        }
        renderer_.DrawModel(*meshcomp->GetModel());
    }
    shaderp_->Unbind();
}





}