#include <SRender/Passes/ActorPickerMaskRenderPass.h>
#include <cstddef>
#include <stdint.h>
#include "ECS/Actor.h"
#include "ECS/Component/TransformComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"

namespace SRender::Passes{


ActorPickerMaskRenderPass::ActorPickerMaskRenderPass():
renderer_(SANASERVICE(Core::EntityRenderer)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    std::string pth = ResourceManager::Util::GetFullPath(":shaders\\unlit.glsl");
    shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
}
void ActorPickerMaskRenderPass::Draw(){
    shaderp_->Bind();
    glm::mat4 tmpmodel = glm::mat4(1);
    for (auto& meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
        auto transcomp =meshcomp->parentactor_.GetTransformComponent();

        if (transcomp) {
            shaderp_->SetUniMat4("ModelMat", transcomp->GetMat());
        }else{
            shaderp_->SetUniMat4("ModelMat", tmpmodel);
        }
        uint32_t actorid = static_cast<uint32_t>( meshcomp->parentactor_.GetID());
        auto bytep = reinterpret_cast<uint8_t*>(&actorid);
        shaderp_->SetUniVec4("diffuse_color", glm::vec4(bytep[0]/255.0f,bytep[1]/255.0f,bytep[2]/255.0f,1.0f));
        renderer_.DrawModelwithEmptyTex(*meshcomp->GetModel());
    }
    shaderp_->Unbind();
}





}