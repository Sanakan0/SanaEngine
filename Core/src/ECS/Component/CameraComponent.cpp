#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/Component.h"
#include "imgui/imgui.h"
#include <limits>
namespace ECS::Components {

CameraComponent::CameraComponent(Actor& parentactor):
Component(parentactor){

}

void CameraComponent::Tick(float delta_t){

}
DrawCmd CameraComponent::GetInspectorDrawCmd() {
    return std::bind(&CameraComponent::DrawInspector,this);
}

void CameraComponent::DrawInspector() {
    if(ImGui::CollapsingHeader("Camera")){
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

        ImGui::DragFloat("near",&cam_.near_,std::numeric_limits<float>::min(),cam_.far_);
        ImGui::DragFloat("far",&cam_.far_,cam_.near_,std::numeric_limits<float>::max());

        ImGui::ColorEdit4("background color", &cam_.clear_color_.r);
        
        
        
    }
}





}