#pragma once
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SModel.h"
#include "SRender/Resources/SShader.h"
#include "SceneSys/SceneManager.h"
#include <algorithm>
#include <memory>


namespace SEditor::Core{class RuntimeContext;}

namespace SRender::Passes{

class EditorSceneRenderPass{
public:
    EditorSceneRenderPass();
    ~EditorSceneRenderPass()=default;
    void Draw();
    std::vector<Resources::SModel*> render_resources_;
    float k=0;
private:
    std::unique_ptr<Resources::GLShader> shaderp_;
    Core::EntityRenderer& renderer_;
    SceneSys::SceneManager& scenemanager_;
};


}