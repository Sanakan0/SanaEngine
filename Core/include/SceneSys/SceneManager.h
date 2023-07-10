#pragma once

#include "ECS/Actor.h"
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
    ECS::Actor* GetSelectedActor(){return selected_actor_p_;}
    void SetActiveCamera(ECS::ActorID id){
        active_camera_id_=id;
        active_camera_p_=scenep_->GetActorbyID(id);
    }
    ECS::Actor* GetActiveCamera(){return active_camera_p_;}
    glm::vec3 cursor_pos_{0,0,0};
    bool enable_img_prj_=0;
    std::shared_ptr<SRender::Resources::STexture> img_tex_;
private:
    SRender::Resources::ModelLoadSetting SceneLoadSetting_;
    std::unique_ptr<Scene> scenep_;
    ECS::ActorID selected_actor_id_=0;
    ECS::Actor* selected_actor_p_=nullptr;
    ECS::ActorID active_camera_id_=0;
    ECS::Actor* active_camera_p_=nullptr;
    
};

}