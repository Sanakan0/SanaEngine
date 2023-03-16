#pragma once
#include "ECS/Actor.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SModel.h"
#include "SRender/Resources/SShader.h"
#include "SRender/Resources/STexture.h"
#include "SceneSys/SceneManager.h"
#include <algorithm>
#include <memory>


namespace SEditor::Core{class RuntimeContext;}

namespace SRender::Passes{

class ActorPickerMaskRenderPass{
public:
    ActorPickerMaskRenderPass();
    ~ActorPickerMaskRenderPass()=default;
    void Draw();
private:
    std::unique_ptr<Resources::GLShader> shaderp_;
    Core::EntityRenderer& renderer_;
    SceneSys::SceneManager& scenemanager_;

};


}