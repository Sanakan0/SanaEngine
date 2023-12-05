#pragma once
#include "SMath/Transform.h"
namespace SRender::Resources{

struct SLight{
    enum class Type{DIRECTIONAL};
    SLight(sm::Transform& transfrom,Type type);

private:
    sm::Transform& transform_;
};


}