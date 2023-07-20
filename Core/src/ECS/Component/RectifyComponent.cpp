
#include "ECS/Component/RecitfyComponent.h"
#include "Serialize/Serializer.h"
#include "glm/fwd.hpp"

namespace ECS::Components {

RecifyComponent::RecifyComponent(Actor& parentactor):
Component(parentactor){

}

void RecifyComponent::Tick(float delta_t){

}

void RecifyComponent::DrawInspector() {

}

void RecifyComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node){
    SCore::Serializer::SerializeString(p_doc, p_node, "img_pth", img_pth_);
    tinyxml2::XMLNode* linesnode = p_doc.NewElement("Lines");
	p_node->InsertEndChild(linesnode);
    for (auto& v:lines){
        tinyxml2::XMLNode* linenode = p_doc.NewElement("Line");
	    linesnode->InsertEndChild(linenode);
        for (auto& p:v){
            tinyxml2::XMLNode* Pointnode = p_doc.NewElement("Point");
	        linenode->InsertEndChild(Pointnode);
            SCore::Serializer::SerializeVec2d(p_doc,Pointnode,"coord",glm::dvec2(p.first,p.second));
        }
    }
}

void RecifyComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node){
    SCore::Serializer::DeserializeString(p_doc, p_node, "img_pth", img_pth_);
    tinyxml2::XMLNode* linesnode = p_node->FirstChildElement("Lines");
    if (linesnode){
        tinyxml2::XMLElement* currentLine = linesnode->FirstChildElement("Line");
        while(currentLine){
            lines.push_back({});
            tinyxml2::XMLElement* currentP = currentLine->FirstChildElement("Point");
            while(currentP){
                auto tmpp = glm::dvec2();
                SCore::Serializer::DeserializeVec2d(p_doc,currentP,"coord",tmpp);
                lines.back().push_back({tmpp.x,tmpp.y});
                currentP = currentP->NextSiblingElement("Point");
            }
            currentLine = currentLine->NextSiblingElement("Line");
        }
    }
}

}