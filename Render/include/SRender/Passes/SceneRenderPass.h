#pragma once
#include "SRender/Core/EntityRenderer.h"
#include "SRender/LowRenderer/Camera.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SModel.h"
#include "SRender/Resources/SShader.h"
#include "SceneSys/SceneManager.h"
#include <algorithm>
#include <memory>


namespace SEditor::Core{class RuntimeContext;}

namespace SRender::Passes{

class SceneRenderPass{
public:
    SceneRenderPass();
    ~SceneRenderPass()=default;
    void Draw();
    void BindDistortionInfo(LowRenderer::RadialDistortion* distinfo);
    void EnableDistortion(){shaderp_ = distortion_shaderp_.get();}
    void DisableDistortion(){shaderp_ = unlit_shaderp_.get();}
private: 
    std::unique_ptr<Resources::GLShader> unlit_shaderp_;
    std::unique_ptr<Resources::GLShader> standard_shaderp_;
    std::unique_ptr<Resources::GLShader> distortion_shaderp_;
    Resources::GLShader* shaderp_;
    Core::EntityRenderer& renderer_;
    SceneSys::SceneManager& scenemanager_;
    const LowRenderer::RadialDistortion* distortioninfo_ = nullptr;
    bool enabledistortion_=false;
};


}