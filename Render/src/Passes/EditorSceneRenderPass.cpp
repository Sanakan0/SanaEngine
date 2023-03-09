#include <SRender/Passes/EditorSceneRenderPass.h>
#include "ECS/Component/TransformComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"

namespace SRender::Passes{


EditorSceneRenderPass::EditorSceneRenderPass():
renderer_(SANASERVICE(Core::EntityRenderer)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    std::string pth = ResourceManager::Util::GetFullPath(":shaders\\standard.glsl");
    shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
}
void EditorSceneRenderPass::Draw(){
    shaderp_->Bind();
    shaderp_->SetUniFloat("k", k);
    glm::mat4 tmpmodel = glm::mat4(1);
    for (auto& meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
        auto transcomp =static_cast<ECS::Components::TransformComponent*>(
            meshcomp->parentactor_.GetComponent("TransformComponent"));
        
        if (transcomp) {
            shaderp_->SetUniMat4("ModelMat", transcomp->GetMat());
        }else{
            shaderp_->SetUniMat4("ModelMat", tmpmodel);
        }
        DrawModel(meshcomp->GetModel());
    }
    shaderp_->Unbind();
}

void EditorSceneRenderPass::DrawModel(Resources::SModel* model){
    auto& meshes = model->GetMeshes();
    auto& materials=model->GetMaterials();
    for (int i=0;i<meshes.size();++i){
        //bind texture
        materials[i].data[0]->Bind(0);
        //draw sth
        renderer_.Draw(*meshes[i], Setting::SPrimitive::TRIANGLES);
        materials[i].data[0]->Unbind();
    }
}



}