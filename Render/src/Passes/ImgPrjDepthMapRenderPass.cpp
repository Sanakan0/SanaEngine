#include "SRender/Passes/ImgPrjDepthMapRenderPass.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SResourceManager/Util.h"

namespace SRender::Passes{

ImgPrjDepthMapRenderPass::ImgPrjDepthMapRenderPass():
renderer_(SANASERVICE(Core::EntityRenderer)),
scenemanager_(SANASERVICE(SceneSys::SceneManager))
{
    std::string pth = ResourceManager::PathManager::GetFullPath(":shaders\\img_prj_depthmap.glsl");
    shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
}
void ImgPrjDepthMapRenderPass::Draw(){
    shaderp_->Bind();
    glm::mat4 tmpmodel = glm::mat4(1);
    for (auto& meshcomp:scenemanager_.GetScene()->GetBasicRenderComponent().meshcomps){
        auto& transcomp =meshcomp->parentactor_.GetTransformComponent();

        
        shaderp_->SetUniMat4("ModelMat", transcomp.GetMat());
        
        renderer_.DrawModel(*meshcomp->GetModel());
    }
    shaderp_->Unbind();
   
}





}