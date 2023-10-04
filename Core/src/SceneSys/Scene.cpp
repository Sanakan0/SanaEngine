#include "ECS/Actor.h"
#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/Component.h"
#include "ECS/Component/LightComponent.h"
#include "ECS/Component/MeshComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SceneSys/SceneManager.h"
#include "spdlog/spdlog.h"
#include <SceneSys/Scene.h>
#include <algorithm>
#include <functional>
#include <memory>
#include <string>

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
    auto& scenemanager= SANASERVICE(SceneManager);
	res.AddComponentEvent.AddListener(std::bind(&SceneManager::OnAddComponent,&scenemanager,std::placeholders::_1));
	spdlog::info("[SCENE] Actor created! id = "+std::to_string(res.GetID()));
    return res;
}

void Scene::DeleteActor(const ECS::ActorID id){
	const std::lock_guard<std::mutex> lock(mutex_);
	for (auto&[n,c] :actors_[id]->GetComponents()){
		if (auto tmp = dynamic_cast<ECS::Components::MeshComponent*>(c.get())){
			auto& vec= basicrendercomponents_.meshcomps;
			vec.erase(std::remove(vec.begin(), vec.end(), tmp), vec.end());
        
    	}
		else if (auto tmp = dynamic_cast<ECS::Components::CameraComponent*>(c.get())){
			auto& vec= basicrendercomponents_.camcomps;
			vec.erase(std::remove(vec.begin(), vec.end(), tmp), vec.end());
		}
	}
	actors_.erase(id);
}

void Scene::OnAddComponent(ECS::Components::Component& component){
    if (auto tmp = dynamic_cast<ECS::Components::MeshComponent*>(&component)){
        basicrendercomponents_.meshcomps.push_back(tmp);
    }
    else if (auto tmp = dynamic_cast<ECS::Components::CameraComponent*>(&component)){
        basicrendercomponents_.camcomps.push_back(tmp);
    }
}

ECS::Actor* Scene::GetActorbyID(ECS::ActorID id){
    if (auto tmp = actors_.find(id);tmp != actors_.end()){
        return tmp->second.get();
    }
    return nullptr;
}


void Scene::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root) {
    tinyxml2::XMLNode* sceneNode = p_doc.NewElement("scene");
	p_root->InsertEndChild(sceneNode);

	tinyxml2::XMLNode* actorsNode = p_doc.NewElement("actors");
	sceneNode->InsertEndChild(actorsNode);

	for (auto&[_ , actor] : actors_)
	{
		actor->Serialize(p_doc, actorsNode);
	}
}

void Scene::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root) {
    tinyxml2::XMLNode* actorsRoot = p_root->FirstChildElement("actors");

	if (actorsRoot)
	{
		tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");

		ECS::ActorID maxID = 1;

		while (currentActor)
		{
			auto& actor = CreateActor();
			actor.Deserialize(p_doc, currentActor);
			//maxID = std::max(actor.GetID() + 1, maxID);
			currentActor = currentActor->NextSiblingElement("actor");
		}

		//actor_id_cnt_ = maxID;

		// /* We recreate the hierarchy of the scene by attaching children to their parents */
		// for (auto&[_ , actor] : actors_)
		// {
		// 	if (actor->GetParentID() > 0)
		// 	{
		// 		if (auto found = FindActorByID(actor->GetParentID()); found)
		// 			actor->SetParent(*found);
		// 	}
		// }
	}
}


}


