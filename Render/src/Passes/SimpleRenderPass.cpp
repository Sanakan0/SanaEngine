#include "SRender/Passes/SimpleRenderPass.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SResourceManager/Util.h"


namespace SRender::Passes{

SimpleRenderPass::SimpleRenderPass(SEditor::Core::RuntimeContext& rtcontext):
renderer_(SANASERVICE(Core::EntityRenderer)),rtcontext_(rtcontext){
    std::string pth = ResourceManager::PathManager::GetFullPath(":shaders\\standard.glsl");
    shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
}

void SimpleRenderPass::Draw(){
    shaderp_->Bind();
    shaderp_->SetUniFloat("k", k);
    glm::mat4 tmpmodel = glm::mat4(1);
    tmpmodel[3]=glm::vec4(0,0,0,1);
    shaderp_->SetUniMat4("ModelMat", tmpmodel);
    for (auto model:render_resources_){
        renderer_.DrawModel(*model);
    }
    shaderp_->Unbind();
}

}