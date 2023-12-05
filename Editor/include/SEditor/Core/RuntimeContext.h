#pragma once
#include "SEditor/Core/AssetLoader.h"
#include "SRender/Buffers/GLShaderStorageBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/ShaderManager.h"
#include "SResourceManager/TextureManager.h"
#include "SWnd/Input/InputManager.h"
#include "SceneSys/SceneManager.h"
#include <memory>
#include <SWnd/context/context.h>
#include <SGUI/Core/UImanager.h>
#include <SRender/Buffers/GLUniformBuffer.h>
#include <SRender/Core/GLRenderer.h>
#include <string>
namespace SEditor::Core{

class RuntimeContext{
public:
    RuntimeContext();
    ~RuntimeContext();



    std::string engine_asset_pth_;
    std::string project_pth_;

    std::unique_ptr<SWnd::Context> wndcontext_;
    std::unique_ptr<SGUI::Core::UImanager> uimanager_;
    
    //std::unique_ptr<SRender::Core::GLRenderer> core_renderer_;
    std::unique_ptr<SRender::Core::EntityRenderer> core_renderer_;
    SRender::Core::GLShapeDrawer* shape_drawer_;
    std::unique_ptr<SRender::Buffers::GLUniformBuffer> editor_ubo_;
    std::unique_ptr<SRender::Buffers::GLShaderStorageBuffer> anima_ssbo_,light_ssbo_;
    std::unique_ptr<ResourceManager::ShaderManager> shader_manager_;
    std::unique_ptr<ResourceManager::ModelManager> model_manager_;
    std::unique_ptr<ResourceManager::TextureManager> texture_manager_;
    std::unique_ptr<SceneSys::SceneManager> scene_manager_;
    std::unique_ptr<SWnd::Input::InputManager> input_manager_;
    std::unique_ptr<AssetLoader> asset_loader_;
};

}