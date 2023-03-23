#pragma once

#include "ECS/Actor.h"
#include "SRender/LowRenderer/Camera.h"
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
    void LoadScene();
    void LoadTiles();
    Scene* GetScene(){return scenep_.get();}
    //0 when no selected actor
    void SetSelectedActor(ECS::ActorID id){
        selected_actor_id_=id;
        selected_actor_p_=scenep_->GetActorbyID(id);
    }
    ECS::Actor* GetSelectedActor(){return selected_actor_p_;}
    void SetActiveCamera(ECS::ActorID id){
        active_camera_id_=id;
        active_camera_p_=scenep_->GetActorbyID(id);
    }
    ECS::Actor* GetActiveCamera(){return active_camera_p_;}
    glm::vec3 cursor_pos_{0,0,0};
private:
    std::unique_ptr<Scene> scenep_;
    ECS::ActorID selected_actor_id_=0;
    ECS::Actor* selected_actor_p_=nullptr;
    ECS::ActorID active_camera_id_=0;
    ECS::Actor* active_camera_p_=nullptr;
    
};

}