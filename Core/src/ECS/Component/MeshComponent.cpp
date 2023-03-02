#include "ECS/Component/Component.h"
#include <ECS/Component/MeshComponent.h>
namespace ECS::Components {
MeshComponent::MeshComponent(Actor& parentactor):Component(parentactor){

}

void MeshComponent::Tick(float delta_t){
    
}

}