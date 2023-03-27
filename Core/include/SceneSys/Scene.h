#pragma once
#include "ECS/Actor.h"
#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/Component.h"
#include "ECS/Component/MeshComponent.h"
#include "ECS/Component/TransformComponent.h"
#include <memory>
#include <mutex>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace SceneSys{

enum SceneSetting{
    Actor_ID_Max = (1<<24)-5
};
struct BasicRenderComponents{
    std::vector<ECS::Components::MeshComponent*> meshcomps;
    std::vector<ECS::Components::CameraComponent*> camcomps;
};

class Scene{
public:
    Scene();
    ~Scene();
    ECS::Actor& CreateActor(const std::string& name="Default");
    auto& GetActors(){return actors_;}
    ECS::Actor* GetActorbyID(ECS::ActorID id);
    const BasicRenderComponents& GetBasicRenderComponent(){return basicrendercomponents_;}
    void OnAddComponent(ECS::Components::Component& component);
    

private:
    std::unordered_map<ECS::ActorID , std::unique_ptr<ECS::Actor>> actors_;
    BasicRenderComponents basicrendercomponents_;
    ECS::ActorID actor_id_cnt_=1;
    std::mutex mutex_;
};
    
}