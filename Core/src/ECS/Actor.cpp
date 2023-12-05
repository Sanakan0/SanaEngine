#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/DatasetGenComponent.h"
#include "ECS/Component/LightComponent.h"
#include "ECS/Component/MeshComponent.h"
#include "ECS/Component/RectifyComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "Serialize/Serializer.h"
#include <ECS/Actor.h>

namespace ECS {


Actor::Actor(ActorID id,const std::string& name):
id_(id),name_(name),transform_(AddComponent<Components::TransformComponent>()){
    
}

Actor::~Actor(){}


void Actor::Tick(float delta_t){
    for (auto& component:components_){
        component.second->Tick(delta_t);
    }
}

void Actor::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    tinyxml2::XMLNode* actorNode = p_doc.NewElement("actor");
	p_node->InsertEndChild(actorNode);

	SCore::Serializer::SerializeString(p_doc, actorNode, "name", name_);
	SCore::Serializer::SerializeInt64(p_doc, actorNode, "id", id_);
	//SCore::Serializer::SerializeInt64(p_doc, actorNode, "parent", m_parentID);

	tinyxml2::XMLNode* componentsNode = p_doc.NewElement("components");
	actorNode->InsertEndChild(componentsNode);

	for (auto&[component_type,component] : components_)
	{
		/* Current component root */
		tinyxml2::XMLNode* componentNode = p_doc.NewElement("component");
		componentsNode->InsertEndChild(componentNode);

		/* Component type */
		SCore::Serializer::SerializeString(p_doc, componentNode, "type", typeid(*component).name());

		/* Data node (Will be passed to the component) */
		tinyxml2::XMLElement* data = p_doc.NewElement("data");
		componentNode->InsertEndChild(data);

		/* Data serialization of the component */
		component->Serialize(p_doc, data);
	}

	
}

void Actor::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_actorsRoot) {
    SCore::Serializer::DeserializeString(p_doc, p_actorsRoot, "name", name_);
	//SCore::Serializer::DeserializeInt64(p_doc, p_actorsRoot, "id", id_);

	{
		tinyxml2::XMLNode* componentsRoot = p_actorsRoot->FirstChildElement("components");
		if (componentsRoot)
		{
			tinyxml2::XMLElement* currentComponent = componentsRoot->FirstChildElement("component");

			while (currentComponent)
			{
				std::string componentType = currentComponent->FirstChildElement("type")->GetText();
				Components::Component* component = nullptr;

				// TODO: Use component name instead of typeid (unsafe)

				if (componentType == typeid(Components::TransformComponent).name())			component = &transform_;
				else if (componentType == typeid(Components::MeshComponent).name())			component = &AddComponent<Components::MeshComponent>();
				else if (componentType == typeid(Components::CameraComponent).name())			component = &AddComponent<Components::CameraComponent>();
				else if (componentType == typeid(Components::RectifyComponent).name())			component = &AddComponent<Components::RectifyComponent>();
				else if (componentType == typeid(Components::LightComponent).name()) 		component = &AddComponent<Components::LightComponent>();
				else if (componentType == typeid(Components::DatasetGenComponent).name()) 		component = &AddComponent<Components::DatasetGenComponent>();
				
				if (component)
					component->Deserialize(p_doc, currentComponent->FirstChildElement("data"));

				currentComponent = currentComponent->NextSiblingElement("component");
			}
		}
	}


}

void Actor::OnUpdate(float delta_t){
	for (auto& [n,c]:components_){
		c->OnUpdate(delta_t);
	}
}

void Actor::OnDrawGizmo(float delta_t){
	for (auto& [n,c]:components_){
		c->OnDrawGizmo(delta_t);
	}
}

}