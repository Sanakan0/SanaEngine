
#include "SRender/LowRenderer/Camera.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"
#include <SMath/Quaternion.h>
#include <algorithm>
namespace SRender::LowRenderer{

Camera::Camera() {
}



glm::mat4 Camera::CalcViewMat(const glm::vec3& pos,const glm::quat& orien)const {
    auto rotat = glm::mat3_cast(glm::conjugate( orien));
    glm::mat4 res(rotat);
    res[3]=glm::vec4(rotat*-pos,1);
    return res;
}

glm::mat4 Camera::CalcProjectionMat(float w,float h) const{
	float ratio = w/h;
	return glm::perspective(glm::radians(fovy_) ,ratio,near_,far_);
}





}