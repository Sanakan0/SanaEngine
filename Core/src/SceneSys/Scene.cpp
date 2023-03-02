#include "ECS/Component/Component.h"
#include "ECS/Component/MeshComponent.h"
#include <SceneSys/Scene.h>
#include <memory>

namespace SceneSys{

Scene::Scene(){}
Scene::~Scene(){}
ECS::Actor& Scene::CreateActor(const std::string& name){
    actors_.push_back(std::make_unique<ECS::Actor>(actor_id_cnt_++,name));
    auto& res = *actors_[actors_.size()-1];
    // auto meshcmp = res.GetComponent<ECS::Components::MeshComponent>();
    // if(meshcmp) basicrendercomponents_.meshcomps.push_back(meshcmp);
    return res;
}



}


