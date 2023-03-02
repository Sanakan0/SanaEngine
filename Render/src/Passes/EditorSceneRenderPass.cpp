#include <SRender/Passes/EditorSceneRenderPass.h>
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShaderLoader.h"
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
    for (auto& actorp:scenemanager_.GetScene()->GetActors()){
        
    }
}




}