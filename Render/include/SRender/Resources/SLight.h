#pragma once
#include "SMath/Transform.h"
#include "glm/fwd.hpp"
namespace SRender::Resources{

struct SLight{
    enum class Type{DIRECTIONAL};
    SLight(sm::Transform& transform,Type type=Type::DIRECTIONAL);

    glm::mat4 GenLightMat() const;

    float type_=0;
private:
    sm::Transform& transform_;
};


}