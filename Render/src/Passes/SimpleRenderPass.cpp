#include "SRender/Passes/SimpleRenderPass.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SResourceManager/Util.h"


namespace SRender::Passes{

SimpleRenderPass::SimpleRenderPass():renderer_(SANASERVICE(Core::EntityRenderer)){
    std::string pth = ResourceManager::Util::GetFullPath(":shaders\\standard.glsl");
    shaderp_ = std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(pth));
}

void SimpleRenderPass::Draw(){
    shaderp_->Bind();
    shaderp_->SetUniFloat("k", k);
    glm::mat4 tmpmodel = glm::mat4(1);
    tmpmodel[3]=glm::vec4(0,0,0,1);
    shaderp_->SetUniMat4("ModelMat", tmpmodel);
    for (auto model:render_resources_){
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
    shaderp_->Unbind();
}

}