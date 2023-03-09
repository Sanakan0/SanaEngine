#pragma once
#include "ECS/Actor.h"
#include "ECS/Component/Component.h"
#include "ECS/Component/MeshComponent.h"
#include "ECS/Component/TransformComponent.h"
#include <memory>
#include <mutex>
#include <stdint.h>
#include <string>
#include <vector>
namespace SceneSys{


struct BasicRenderComponents{
    std::vector<ECS::Components::MeshComponent*> meshcomps;
};

class Scene{
public:
    Scene();
    ~Scene();
    ECS::Actor& CreateActor(const std::string& name="Default");
    std::vector<std::unique_ptr<ECS::Actor>>& GetActors(){return actors_;}
    const BasicRenderComponents& GetBasicRenderComponent(){return basicrendercomponents_;}
    void OnAddComponent(ECS::Components::Component& component);
    

private:
    std::vector<std::unique_ptr<ECS::Actor>> actors_;
    BasicRenderComponents basicrendercomponents_;
    uint64_t actor_id_cnt_=1;
    std::mutex mutex_;
};
    
}