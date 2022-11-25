#include "SEditor/Core/RuntimeContext.h"
#include "SRender/Buffers/GLShaderStorageBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Core/GLRenderer.h"
#include <SCore/Global/ServiceLocator.h>
#include <memory>
namespace SEditor::Core{
using namespace SCore::Global;
RuntimeContext::RuntimeContext(){
    SWnd::contextSETTING wndset{800,800,true};
    wndcontext_ = std::make_unique<SWnd::Context>(wndset);
    wndcontext_->setup_GLAD();

    uimanager_ = std::make_unique<SGUI::Core::UImanager>(wndcontext_->Wnd_p);

    //core_renderer_ = std::make_unique<SRender::Core::GLRenderer>();
    core_renderer_ = std::make_unique<SRender::Core::EntityRenderer>();
    shape_drawer_ = std::make_unique<SRender::Core::GLShapeDrawer>(*core_renderer_);

    editor_ubo_ = std::make_unique<SRender::Buffers::GLUniformBuffer>(
        //sizeof(glm::mat4)+ //model
        sizeof(glm::mat4)+
        sizeof(glm::mat4)+
        sizeof(glm::mat4)+ //prjview
        sizeof(glm::vec4), //pos
        0,
        GL_STREAM_DRAW
    );

    anima_ssbo_ = std::make_unique<SRender::Buffers::GLShaderStorageBuffer>();
    anima_ssbo_->Bind(0);
    //upload service to servicelocator
    ServiceLocator::Provide<SWnd::Context>(*wndcontext_); 
    ServiceLocator::Provide<SGUI::Core::UImanager>(*uimanager_); 
    ServiceLocator::Provide<SRender::Buffers::GLUniformBuffer>(*editor_ubo_);
}

RuntimeContext::~RuntimeContext(){}


}