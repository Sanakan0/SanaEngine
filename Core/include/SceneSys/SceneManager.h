#pragma once

#include "ECS/Actor.h"
#include "ECS/Component/LightComponent.h"
#include "SRender/LowRenderer/Camera.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/Settings.h"
#include "SceneSys/Scene.h"
#include <cstddef>
#include <memory>
#include <utility>
namespace SceneSys{

class SceneManager{
public:
    SceneManager();
    ~SceneManager();
    void CreateScene();
    void DestroyScene();
    void LoadScene(std::string pth);
    void LoadTiles();
    Scene* GetScene(){return scenep_.get();}

    SRender::Resources::ModelLoadSetting& GetSceneLoadSetting(){return SceneLoadSetting_;}
    //0 when no selected actor
    void SetSelectedActor(ECS::ActorID id){
        selected_actor_id_=id;
        selected_actor_p_=scenep_->GetActorbyID(id);
    }
    ECS::ActorID GetSelectedActorID(){return selected_actor_id_;}
    ECS::Actor* GetSelectedActor(){return selected_actor_p_;}
    void SetActiveCamera(ECS::ActorID id){
        active_camera_id_=id;
        active_camera_p_=scenep_->GetActorbyID(id);
    }
    ECS::Actor* GetActiveCamera(){return active_camera_p_;}
    ECS::Actor* GetMainLight(){return main_light_p_;}
    glm::vec3 cursor_pos_{0,0,0};
    bool enable_img_prj_=0;
    std::shared_ptr<SRender::Resources::STexture> img_tex_;

    void OnAddComponent(ECS::Components::Component& component){
        if (auto tmp = dynamic_cast<ECS::Components::LightComponent*>(&component)){
            main_light_p_ = &tmp->parentactor_;
            main_light_id_= tmp->parentactor_.GetID();
        }
    }

    void OnDeleteActor(ECS::Actor& actor){
        if (actor.GetID() == active_camera_id_){
            active_camera_id_ = 0;
            active_camera_p_ = nullptr;
        }
        if (actor.GetID() == selected_actor_id_){
            selected_actor_id_=0;
            selected_actor_p_=nullptr;
        }
        if (actor.GetID() == main_light_id_){
            if (GetScene()->GetBasicRenderComponent().lightcomps.size()>0){
                main_light_p_=&GetScene()->GetBasicRenderComponent().lightcomps[0]->parentactor_;
                main_light_id_=main_light_p_->GetID();
            }else{
                main_light_id_=0;
                main_light_p_=nullptr;
            }
            
        }
    }

private:
    SRender::Resources::ModelLoadSetting SceneLoadSetting_;
    std::unique_ptr<Scene> scenep_;
    ECS::ActorID selected_actor_id_=0;
    ECS::Actor* selected_actor_p_=nullptr;
    ECS::ActorID active_camera_id_=0;
    ECS::Actor* active_camera_p_=nullptr;
    ECS::ActorID main_light_id_=0;
    ECS::Actor* main_light_p_=nullptr;
};

}