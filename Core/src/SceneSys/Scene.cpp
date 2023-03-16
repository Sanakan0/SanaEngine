#include "ECS/Component/Component.h"
#include "ECS/Component/MeshComponent.h"
#include <SceneSys/Scene.h>
#include <functional>
#include <memory>

namespace SceneSys{

Scene::Scene(){}
Scene::~Scene(){}
ECS::Actor& Scene::CreateActor(const std::string& name){
    const std::lock_guard<std::mutex> lock(mutex_);
    actors_.emplace(std::make_pair(actor_id_cnt_,std::make_unique<ECS::Actor>(actor_id_cnt_,name)));
    //actors_.push_back(std::move());
    auto& res = *actors_[actor_id_cnt_];
    actor_id_cnt_++;
    // auto meshcmp = res.GetComponent<ECS::Components::MeshComponent>();
    // if(meshcmp) basicrendercomponents_.meshcomps.push_back(meshcmp);
    res.AddComponentEvent.AddListener(std::bind(&Scene::OnAddComponent,this,std::placeholders::_1));
    return res;
}

void Scene::OnAddComponent(ECS::Components::Component& component){
    if (auto tmp = dynamic_cast<ECS::Components::MeshComponent*>(&component)){
        basicrendercomponents_.meshcomps.push_back(tmp);
    }
}

ECS::Actor* Scene::GetActorbyID(ECS::ActorID id){
    if (auto tmp = actors_.find(id);tmp != actors_.end()){
        return tmp->second.get();
    }
    return nullptr;
}

}


