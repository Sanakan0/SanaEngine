#pragma once
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SModel.h"
#include "SRender/Resources/SShader.h"
#include <algorithm>
#include <memory>
namespace SRender::Passes{

class SimpleRenderPass{
public:
    SimpleRenderPass();
    ~SimpleRenderPass()=default;
    void Draw();
    std::vector<Resources::SModel*> render_resources_;
private:
    std::unique_ptr<Resources::GLShader> shaderp_;
    Core::EntityRenderer& renderer_;
};


}