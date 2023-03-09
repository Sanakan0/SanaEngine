#pragma once

#include "glm/ext/quaternion_float.hpp"
namespace sm{

// Order: scale , rotate, translate
glm::mat4 MakeMat(const glm::vec3&scale ,const glm::quat& orien,const glm::vec3& pos);

}