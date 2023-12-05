#pragma once
#include "SEditor/Core/CameraCtrl.h"
#include "SMath/Transform.h"
#include "SRender/Buffers/GLFrameBuffer.h"
#include "SRender/LowRenderer/Camera.h"
#include "SRender/Passes/SimpleRenderPipeline.h"
#include "SceneSys/SceneManager.h"
#include "ThreadPool.h"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <ECS/Component/Component.h>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
namespace ECS::Components {

class DatasetGenComponent:public Component{

public:
    DatasetGenComponent(Actor& parentactor);
    ~DatasetGenComponent();
    virtual void Tick(float delta_t) override;

    virtual void DrawInspector() override;

    virtual void OnDrawGizmo(float delta_t) override;

    virtual void OnUpdate(float delta_t) override;

    virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

    virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

private:

    void RenderImg(SRender::LowRenderer::Camera& intrinsic,sm::Transform& extrinsic,const std::string& name="default");
    void SaveFbo(const std::string& name);
    void LoadLensData();
    
    int cnt_=0;
    int totalImgCnt_=0;
    bool startGen=0;
    std::vector<std::tuple<float,float>>lens_;

    glm::vec3 extends_{10,10,10};
    float step_=5;
    float fovy_=1;
    float aspect_=1;
    std::string save_pth_;
    std::string lensdata_pth_;
    int resolution_=1080;
    SRender::SimpleRenderPipeline renderpipeline_;
    SRender::Buffers::GLFrameBuffer dist_fbo_;
    SRender::Buffers::GLFrameBuffer undist_fbo_;

    SRender::LowRenderer::Camera defaultCam_;
    sm::Transform defaultTran_;
    SEditor::Core::ACamera acam_;

    SceneSys::SceneManager& scenemanager_;

    ThreadPool threadpool_{16};
};


}