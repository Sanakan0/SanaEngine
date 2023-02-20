#pragma once
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Passes/SimpleRenderPass.h"
#include "SRender/Resources/SModel.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/TextureManager.h"
#include<SRender/Buffers/GLFrameBuffer.h>
#include<SEditor/Panels/SView.h>
#include<SEditor/Core/RuntimeContext.h>
#include <SResourceManager/ShaderManager.h>
#include <memory>
namespace SEditor::Panels{
class TestView:public SView{
public:
    TestView(Core::RuntimeContext& rtcontext);
    ~TestView();
    void LogicTick(float deltat);
    void RenderTick(float deltat);
private:   
    Core::RuntimeContext& rtcontext_;
    std::unique_ptr<SRender::Resources::SMesh> trimeshp;
    ResourceManager::ShaderManager& shadermanager;
    ResourceManager::TextureManager& texturemanager;
    ResourceManager::ModelManager& modelmanager;
    SRender::Resources::SModel* model;
    SRender::Passes::SimpleRenderPass renderpass;
    void task1(std::string pth,int idx);
    std::mutex test_m;
};


}