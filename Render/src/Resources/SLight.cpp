#include "glm/fwd.hpp"
#include <SRender/Resources/SLight.h>

namespace SRender::Resources{

SLight::SLight(sm::Transform& transform,Type type)
:transform_(transform),type_(static_cast<float>(type)){
    
}

glm::mat4 SLight::GenLightMat() const{
    glm::mat4 res(0);
    auto pos = transform_.GetPosW();
    auto lightorien = transform_.GetOrienVecW();
    res[0]=glm::vec4(pos,0);
    res[1]=glm::vec4(lightorien,0);
    return res;
}

}