#pragma once
#include "SRender/Buffers/GLShaderStorageBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include <memory>
#include <SWnd/context/context.h>
#include <SGUI/Core/UImanager.h>
#include <SRender/Buffers/GLUniformBuffer.h>
#include <SRender/Core/GLRenderer.h>
namespace SEditor::Core{

class RuntimeContext{
public:
    RuntimeContext();
    ~RuntimeContext();


    std::unique_ptr<SWnd::Context> wndcontext_;
    std::unique_ptr<SGUI::Core::UImanager> uimanager_;
    
    //std::unique_ptr<SRender::Core::GLRenderer> core_renderer_;
    std::unique_ptr<SRender::Core::EntityRenderer> core_renderer_;
    std::unique_ptr<SRender::Core::GLShapeDrawer> shape_drawer_;
    std::unique_ptr<SRender::Buffers::GLUniformBuffer> editor_ubo_;
    std::unique_ptr<SRender::Buffers::GLShaderStorageBuffer> anima_ssbo_;
};

}