#include "SEditor/Panels/CameraView.h"
#include "ECS/Component/CameraComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SAnimation.h"
#include "SRender/Settings/GLSet.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "imgui/imgui.h"
#include <SRender/Resources/SModel.h>
#include <SRender/Resources/SModelLoader.h>
#include <SEditor/Core/SimpleJoint.h>
#include <iostream>
#include <memory>

namespace SEditor::Panels{

CameraView::CameraView(Core::RuntimeContext& rtcontext):rtcontext_(rtcontext){
    name_="相机视图";
}



void CameraView::LogicTick(float deltat){
    
    //update camctl if changed
    if (auto curcam = rtcontext_.scene_manager_->GetActiveCamera();curcam!=active_camera_actor_){
        active_camera_actor_=curcam;
        if (active_camera_actor_ == nullptr){
            camctrl_.SetCamInExParam(cam_, cam_extrinsic_);
            scenerenderpass_.BindDistortionInfo(nullptr);
        }else{
            auto camcomp = static_cast<ECS::Components::CameraComponent*>(
                active_camera_actor_->GetComponent("CameraComponent")
            );
            auto transcomp = active_camera_actor_->GetTransformComponent();
            camctrl_.SetCamInExParam(camcomp->cam_, transcomp->trans_);
        
            scenerenderpass_.BindDistortionInfo(&camcomp->cam_.distortion_);
        }
        
    }

    camctrl_.cam_->CacheProjectionMat(camctrl_.cam_->Getaspect_ratio(),1);
    auto& editor_ubo = SANASERVICE(SRender::Buffers::GLUniformBuffer);
    editor_ubo.BufferSubData(camctrl_.cam_->GetProjectionMat()*camctrl_.cam_->GetViewMat(),sizeof(glm::mat4)*3+sizeof(glm::vec4));

}

void CameraView::RenderTick(float deltat){   
    UpdateViewCam(deltat);
    rtcontext_.core_renderer_->SetViewPort(0, 0,canvas_size_.first ,canvas_size_.second );
    if (enable_line){
        rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::LINE);
        rtcontext_.core_renderer_->SetRasterizationLineWdith(1);
    }else if (enable_point){
        rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::POINT);
        rtcontext_.core_renderer_->SetRasterizationPointSize(1);
    }else{
        rtcontext_.core_renderer_->SetRasterizationMode(SRender::Setting::SRasterization::FILL);
    }
    fbo_.Bind();
    
    auto& renderer = *(rtcontext_.core_renderer_);
    auto& shape_drawer = *(rtcontext_.shape_drawer_);
    renderer.SetClearColor(camctrl_.cam_->clear_color_);
    renderer.ApplyGLstate(SRender::Core::Default_GLstate);
    glStencilMask(0xFF); // stencil mask also influence glclear()
    renderer.ClearBuffer();
    glStencilMask(0x00);
    
    scenerenderpass_.Draw();

    auto fovyratio = camctrl_.cam_->Getfocal_length()/camctrl_.cam_->Getsensor_size_h();
    auto tmpscale = glm::mat4((camctrl_.cam_->near_+camctrl_.cam_->far_)/2.0f/fovyratio);
    tmpscale[3][3]=1;
    shape_drawer.DrawCamFrame(camctrl_.extrinsic_->GetMat()*tmpscale,camctrl_.cam_->Getfocal_length()/camctrl_.cam_->Getsensor_size_h() ,camctrl_.cam_->Getaspect_ratio(), {1,1,1,1});

    fbo_.Unbind();

    
    
}


}
