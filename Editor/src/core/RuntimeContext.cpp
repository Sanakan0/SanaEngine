#include "SEditor/Core/RuntimeContext.h"
#include "SEditor/Core/AssetLoader.h"
#include "SRender/Buffers/GLShaderStorageBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/SModelLoader.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/ShaderManager.h"
#include "SResourceManager/TextureManager.h"
#include "SResourceManager/Util.h"
#include "SWnd/Input/InputManager.h"
#include "SceneSys/SceneManager.h"
#include <SCore/Global/ServiceLocator.h>
#include <filesystem>
#include <memory>
namespace SEditor::Core{
using namespace SCore::Global;
RuntimeContext::RuntimeContext(){
    SWnd::contextSETTING wndset{800,800,true};
    //set engine assetpath
    engine_asset_pth_ = std::filesystem::canonical("assets").string()+"\\";
    ResourceManager::PathManager::SetEngineAssetPath(engine_asset_pth_);

    wndcontext_ = std::make_unique<SWnd::Context>(wndset);
    wndcontext_->setup_GLAD();

    uimanager_ = std::make_unique<SGUI::Core::UImanager>(*wndcontext_);

    //core_renderer_ = std::make_unique<SRender::Core::GLRenderer>();
    core_renderer_ = std::make_unique<SRender::Core::EntityRenderer>();
    shape_drawer_ = core_renderer_->GetShapeDrawer();
    
    //engine ubo setup
    editor_ubo_ = std::make_unique<SRender::Buffers::GLUniformBuffer>(
        //sizeof(glm::mat4)+ //model
        sizeof(glm::mat4)+
        sizeof(glm::mat4)+
        sizeof(glm::mat4)+ //prjview
        sizeof(glm::vec4)+ //pos
        sizeof(glm::mat4)+ 
        sizeof(int), 
        0,
        GL_STREAM_DRAW
    );

    anima_ssbo_ = std::make_unique<SRender::Buffers::GLShaderStorageBuffer>();
    light_ssbo_ = std::make_unique<SRender::Buffers::GLShaderStorageBuffer>();
    anima_ssbo_->Bind(0);
    light_ssbo_->Bind(1);

    
    //setup resource manager
    texture_manager_ = std::make_unique<ResourceManager::TextureManager>();
    shader_manager_ = std::make_unique<ResourceManager::ShaderManager>();
    model_manager_ = std::make_unique<ResourceManager::ModelManager>();
    //init model loader
    SRender::Resources::SModelLoader::Initialize(texture_manager_.get());
    //setup scene manager
    scene_manager_ = std::make_unique<SceneSys::SceneManager>();
    scene_manager_->CreateScene();

    input_manager_ = std::make_unique<SWnd::Input::InputManager>(wndcontext_->Wnd_p);

    //upload service to servicelocator
    ServiceLocator::Provide<SRender::Core::EntityRenderer>(*core_renderer_);
    ServiceLocator::Provide<SWnd::Context>(*wndcontext_); 
    ServiceLocator::Provide<SGUI::Core::UImanager>(*uimanager_); 
    ServiceLocator::Provide<SRender::Buffers::GLUniformBuffer>(*editor_ubo_);
    ServiceLocator::Provide<ResourceManager::TextureManager>(*texture_manager_);
    ServiceLocator::Provide<ResourceManager::ShaderManager>(*shader_manager_);
    ServiceLocator::Provide<ResourceManager::ModelManager>(*model_manager_);
    ServiceLocator::Provide<SceneSys::SceneManager>(*scene_manager_);
    ServiceLocator::Provide<SWnd::Input::InputManager>(*input_manager_);
    ServiceLocator::Provide<SEditor::Core::RuntimeContext>(*this);
    
    asset_loader_ = std::make_unique<AssetLoader>();
    ServiceLocator::Provide<AssetLoader>(*asset_loader_);
}

RuntimeContext::~RuntimeContext(){}


}