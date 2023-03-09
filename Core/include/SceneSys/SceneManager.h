#pragma once

#include "ECS/Actor.h"
#include "SceneSys/Scene.h"
#include <memory>
#include <utility>
namespace SceneSys{

class SceneManager{
public:
    SceneManager();
    ~SceneManager();
    void CreateScene();
    void LoadScene();
    Scene* GetScene(){return scenep_.get();}
    //0 when no selected actor
    void SetSelectedActor(ECS::ActorID id){
        selected_actor_id_=id;
        selected_actor_p_=scenep_->GetActorbyID(id);
    }
    ECS::Actor* GetSelectedActor(){return selected_actor_p_;}
private:
    std::unique_ptr<Scene> scenep_;
    ECS::ActorID selected_actor_id_=0;
    ECS::Actor* selected_actor_p_=nullptr;
};

}