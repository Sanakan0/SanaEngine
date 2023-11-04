#include "ECS/Component/DatasetGenComponent.h"
#include "SEditor/Core/RuntimeContext.h"
#include "SMath/Transform.h"
#include "Serialize/Serializer.h"
#include "glm/fwd.hpp"
#include "imgui/imgui.h"
#include "ECS/Actor.h"
#include "ECS/Component/TransformComponent.h"
#include "SRender/Core/EntityRenderer.h"
#include "SCore/Global/ServiceLocator.h"
namespace ECS::Components {

DatasetGenComponent::DatasetGenComponent(Actor& parentactor):
Component(parentactor){

}

void DatasetGenComponent::Tick(float delta_t){

}

void DatasetGenComponent::DrawInspector() {
    if(ImGui::CollapsingHeader("Dataset")){
        
        ImGui::DragFloat("extends x", &extends_.x,1,1);
        ImGui::DragFloat("extends y", &extends_.y,1,1);
        ImGui::DragFloat("extends z", &extends_.z,1,1);
        ImGui::Separator();
        ImGui::DragFloat("步长", &step_,1,1,FLT_MAX/INT_MAX);
        ImGui::DragFloat("fovt",&fovy_,0.1);
        ImGui::DragFloat("aspe",&aspect_,0.1);
    }
}
void DatasetGenComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {

}

void DatasetGenComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {

}




void DatasetGenComponent::OnDrawGizmo(float delta_t){
    

    static auto& rtcontext = SANASERVICE(SEditor::Core::RuntimeContext);
    if (rtcontext.scene_manager_->GetSelectedActorID()!=parentactor_.GetID()) return;
    auto& renderer = *rtcontext.core_renderer_;
    
    auto drawaround = [&](glm::vec3 pos){
        sm::Transform trans;
        trans.SetPosW(pos);
        glm::vec3 euler{0,0,0};
        for (int i=0;i<3;++i){
            euler.x = i*45+45;
            for (int j=0;j<4;++j){
                euler.z = j*90;
                trans.SetEulerW(euler);
                renderer.GetShapeDrawer()->DrawCamFrame(trans.GetMat(),fovy_,aspect_,{1,1,1,1});
            }
        }
    };
    
    auto& trans = parentactor_.GetTransformComponent()->trans_;
    auto pos = trans.GetPosW()+trans.GetPosL()-extends_;
    auto num =  extends_*2.0f/step_;
    
    for (int i=0;i<=num.x;++i){
        for (int j=0;j<=num.y;++j){
            for (int k=0;k<=num.z;++k){
                auto curpos = pos+glm::vec3(i*step_,j*step_,k*step_);
                //renderer.GetShapeDrawer()->DrawPoint(curpos, {1,1,1,1});
                drawaround(curpos);
            }
        }
    }
}

}