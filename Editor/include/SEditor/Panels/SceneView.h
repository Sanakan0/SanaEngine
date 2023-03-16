#pragma once
#include "SRender/Passes/ActorPickerMaskRenderPass.h"
#include "SRender/Passes/EditorSceneRenderPass.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Passes/SimpleRenderPass.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/TextureManager.h"
#include "SWnd/Input/InputManager.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
#include <SResourceManager/ShaderManager.h>
#include <memory>
namespace SEditor::Panels{
class SceneView:public SView{
public:
    SceneView(Core::RuntimeContext& rtcontext);
    void LogicTick(float deltat);
    void RenderTick(float deltat);
private:   
    void ActorPickerTick(float deltat);
    Core::RuntimeContext& rtcontext_;
    std::unique_ptr<SRender::Resources::SMesh> trimeshp;
    ResourceManager::ShaderManager& shadermanager;
    ResourceManager::TextureManager& texturemanager;
    ResourceManager::ModelManager& modelmanager;
    //SWnd::Input::InputManager& inputmanager;
    SRender::Resources::SModel* model;
    SRender::Passes::SimpleRenderPass simplerenderpass;
    SRender::Passes::EditorSceneRenderPass scenerenderpass;
    SRender::Passes::ActorPickerMaskRenderPass actorpickerrenderpass;
    SRender::Buffers::GLFrameBuffer actor_picker_fbo_;
    void task1(std::string pth,int idx);
    std::mutex test_m;
};


}