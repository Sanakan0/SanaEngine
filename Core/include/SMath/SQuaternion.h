#pragma once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace sm{
#define SM_PI (4.f * atan(1.f))
#define SM_feps(f) (fabs(f)<1e-5?1:0)
inline constexpr glm::vec3 OglCamPrimForward{0,0,-1};
inline constexpr glm::vec3 OglCamPrimUp{0,1,0};
inline constexpr glm::vec3 OglCamPrimRight{1,0,0};
//Opengl Camera model: forward -z,up y
glm::quat LookAt(glm::vec3 up,glm::vec3 forward);

glm::quat Eul2Quat(glm::vec3 euler_deg);
//convert to euler degree
glm::vec3 Quat2Eul(glm::quat qua);



}