#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/Component.h"
#include "SRender/LowRenderer/Camera.h"
#include "Serialize/Serializer.h"
#include "glm/fwd.hpp"
#include "imgui/imgui.h"
#include <limits>
namespace ECS::Components {

CameraComponent::CameraComponent(Actor& parentactor):
Component(parentactor){

}

void CameraComponent::Tick(float delta_t){

}

void CameraComponent::DrawInspector() {
    if(ImGui::CollapsingHeader("Camera")){
        ImGui::Text("basic config");
        auto fovy = cam_.Getfovy();
        ImGui::DragFloat("fovy(degree)",&fovy,0.5,0,180);
        cam_.Setfovy(fovy);

        auto flen = cam_.Getfocal_length();
        ImGui::DragFloat("focal length(mm)",&flen,0.5,0.1,std::numeric_limits<float>::max());
        cam_.Setfocal_length(flen);

        auto ssz = cam_.Getsensor_size_h();
        ImGui::DragFloat("sensor size(height mm)",&ssz,0.5,0,180);
        cam_.Setsensor_size_h(ssz);

        auto ar = cam_.Getaspect_ratio();
        ImGui::DragFloat("aspect ratio(w/h)",&ar,0.05,0,180,"%.8f");
        cam_.Setaspect_ratio(ar);
        static int whr[2]={1,1};
        ImGui::InputInt2("aspect ratio w/h", whr);
        ImGui::SameLine();
        if (ImGui::Button("set")){
            whr[0]=std::max(1,whr[0]);
            whr[1]=std::max(1,whr[1]);
            cam_.Setaspect_ratio(1.0f*whr[0]/whr[1]);
        }

        ImGui::DragFloat("near",&cam_.near_,0.5,std::numeric_limits<float>::min(),cam_.far_);
        ImGui::DragFloat("far",&cam_.far_,0.5,cam_.near_,std::numeric_limits<float>::max());

        ImGui::ColorEdit4("background color", &cam_.clear_color_.r);
        
        
        
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("advance config");
        const char* models[] = { "NONE","INDEX","POLY3","POLY5","PTLENS" ,"DIVISION"};
        ImGui::Combo("Distortion Model", &cam_.distortion_.dist_type, models, IM_ARRAYSIZE(models));
        ImGui::PushItemWidth(80);
        switch (cam_.distortion_.dist_type) {
        case 0:
            break;
        case 1:
            ImGui::DragFloat("k",&cam_.distortion_.dist_para[0],0.01);
            break;
        case 2:
            ImGui::DragFloat("k",&cam_.distortion_.dist_para[0],0.01);
            break;
        case 3:
            ImGui::DragFloat("k1",&cam_.distortion_.dist_para[0],0.01);
            ImGui::SameLine();
            ImGui::DragFloat("k2",&cam_.distortion_.dist_para[1],0.01);
            break;
        case 4:
            ImGui::DragFloat("a",&cam_.distortion_.dist_para[0],0.01);
            ImGui::SameLine();
            ImGui::DragFloat("b",&cam_.distortion_.dist_para[1],0.01);
            ImGui::SameLine();
            ImGui::DragFloat("c",&cam_.distortion_.dist_para[2],0.01);
            break;
        case 5:
            ImGui::DragFloat("K",&cam_.distortion_.dist_para[0],0.01);
            break;
        }
        ImGui::PopItemWidth();
        
        
    }
}


void CameraComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    SCore::Serializer::SerializeFloat(p_doc,p_node, "fovy", cam_.Getfovy());
    SCore::Serializer::SerializeFloat(p_doc,p_node, "sensor_size_h", cam_.Getsensor_size_h());
    SCore::Serializer::SerializeFloat(p_doc,p_node, "focal_length", cam_.Getfocal_length());
    SCore::Serializer::SerializeFloat(p_doc,p_node, "aspect_ratio", cam_.Getaspect_ratio());
    SCore::Serializer::SerializeFloat(p_doc,p_node, "near", cam_.near_);
    SCore::Serializer::SerializeFloat(p_doc,p_node, "far", cam_.far_);
    SCore::Serializer::SerializeVec4(p_doc,p_node, "clear_color", cam_.clear_color_);

    auto tmppara=glm::vec3(cam_.distortion_.dist_para[0],cam_.distortion_.dist_para[1],cam_.distortion_.dist_para[2]);
    SCore::Serializer::SerializeVec3(p_doc,p_node, "Distortion_Parameter",tmppara);
    auto distname = SRender::LowRenderer::MODEL_NAMES[cam_.distortion_.dist_type];
    SCore::Serializer::SerializeString(p_doc,p_node, "Distortion_Type",distname);
}

void CameraComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    float fovy,sensor_sz,aspect;
    SCore::Serializer::DeserializeFloat(p_doc,p_node, "fovy", fovy);
    SCore::Serializer::DeserializeFloat(p_doc,p_node, "sensor_size_h", sensor_sz);
    SCore::Serializer::DeserializeFloat(p_doc,p_node, "aspect_ratio", aspect);

    cam_.Setsensor_size_h(sensor_sz);
    cam_.Setfovy(fovy);
    cam_.Setaspect_ratio(aspect);

    SCore::Serializer::DeserializeFloat(p_doc,p_node, "near", cam_.near_);
    SCore::Serializer::DeserializeFloat(p_doc,p_node, "far", cam_.far_);
    SCore::Serializer::DeserializeVec4(p_doc,p_node, "clear_color", cam_.clear_color_);

    glm::vec3 tmppara;
    SCore::Serializer::DeserializeVec3(p_doc,p_node, "Distortion_Parameter",tmppara);
    for (int i=0;i<3;++i){
        cam_.distortion_.dist_para[i]=tmppara[i];
    }
    std::string distname;
    SCore::Serializer::DeserializeString(p_doc,p_node, "Distortion_Type",distname);
    for (int i=0;i<SRender::LowRenderer::MODEL_NAMES.size();++i){
        if (distname==SRender::LowRenderer::MODEL_NAMES[i]){
            cam_.distortion_.dist_type=i;
            break;
        }
    }


}


}