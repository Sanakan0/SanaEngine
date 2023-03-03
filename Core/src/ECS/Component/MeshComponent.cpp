#include "ECS/Component/Component.h"
#include <ECS/Component/MeshComponent.h>
namespace ECS::Components {
MeshComponent::MeshComponent(Actor& parentactor,SRender::Resources::SModel* model):
Component(parentactor),
model_(model)
{

}

void MeshComponent::Tick(float delta_t){
    
}

}