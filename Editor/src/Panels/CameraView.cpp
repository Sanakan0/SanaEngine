#include "SEditor/Panels/CameraView.h"
#include "ECS/Component/CameraComponent.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SAnimation.h"
#include "SRender/Settings/GLSet.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"
#include <SRender/Resources/SModel.h>
#include <SRender/Resources/SModelLoader.h>
#include <SEditor/Core/SimpleJoint.h>
#include <iostream>
#include <memory>

namespace SEditor::Panels{

CameraView::CameraView(Core::RuntimeContext& rtcontext):rtcontext_(rtcontext){
    name_="Camera View";
}



void CameraView::LogicTick(float deltat){
    
    //update camctl if changed
    if (auto curcam = rtcontext_.scene_manager_->GetActiveCamera();curcam!=nullptr&&curcam!=active_camera_actor_){
        active_camera_actor_=curcam;
        auto camcomp = static_cast<ECS::Components::CameraComponent*>(
            active_camera_actor_->GetComponent("CameraComponent")
        );
        auto transcomp = active_camera_actor_->GetTransformComponent();
        camctrl_.SetCamInExParam(camcomp->cam_, transcomp->trans_);
    }
}

void CameraView::RenderTick(float deltat){   
    UpdateViewCam(deltat);
    rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );

    fbo_.Bind();
    
    auto& renderer = *(rtcontext_.core_renderer_);
    auto& shape_drawer = *(rtcontext_.shape_drawer_);
    renderer.SetClearColor(camctrl_.cam_->clear_color_);
    renderer.ApplyGLstate(SRender::Core::Default_GLstate);
    glStencilMask(0xFF); // stencil mask also influence glclear()
    renderer.ClearBuffer();
    glStencilMask(0x00);
    
    scenerenderpass_.Draw();


    shape_drawer.DrawCamFrame(camctrl_.extrinsic_->GetMat(),camctrl_.cam_->Getfocal_length()/camctrl_.cam_->Getsensor_size_h() ,camctrl_.cam_->Getaspect_ratio(), {1,1,1,1});

    fbo_.Unbind();

}


}
