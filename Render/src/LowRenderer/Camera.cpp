
#include "SRender/LowRenderer/Camera.h"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/trigonometric.hpp"
#include <SMath/Quaternion.h>
#include <algorithm>
namespace SRender::LowRenderer{

Camera::Camera(glm::vec3 pos,glm::vec3 center):camcenter(center),pos_(pos) {
	CalcLookAt();
}

void Camera::reset() {
	pos_ = glm::vec3(5, 5, 5);
	camcenter = glm::vec3(0, 0, 0);
	worldup = glm::vec3(0, 0, 1);
}

void Camera::resetworldup() {
	// glm::vec3 tmp = glm::cross(camright, camcenter - pos_)*worldup;
	// float flag = tmp[0] + tmp[1] + tmp[2];
	// if ( flag<0) {
	// 	worldup[1] *= -1;
	// }
}

void Camera::translate(glm::vec3 trans) {
	trans=orien_*trans;
	pos_ = trans+pos_;
	camcenter = trans + camcenter;
}

void Camera::zoom(float ratio) {
	glm::vec3 dir = pos_ - camcenter;
	pos_ = camcenter + dir*(ratio + 1);
}

void Camera::Orbit(float hori_deg,float verti_deg) {
	euler_xyz_deg_.z+=hori_deg;
	euler_xyz_deg_.x+=verti_deg;
	orien_ = sm::Eul2Quat(euler_xyz_deg_);
    pos_ =camcenter - (orien_*sm::OglCamPrimForward)*glm::length(camcenter-pos_);
}

void Camera::CalcLookAt(){
    orien_ = sm::LookAt(worldup,camcenter-pos_);
    euler_xyz_deg_ = sm::Quat2Eul(orien_);
}

glm::mat4 Camera::CalcViewMat()const {
    auto rotat = glm::mat3_cast(glm::conjugate( orien_));
    glm::mat4 res(rotat);
    res[3]=glm::vec4(rotat*-pos_,1);
    return res;
}

glm::mat4 Camera::CalcProjectionMat(int w,int h) const{
	float ratio = w/static_cast<float>(h);
	return glm::perspective(fov_,ratio,near_,far_);
}

float Camera::CalcDisPerPix(int w,int h){
	float dis = glm::distance(camcenter, pos_);
	return dis*tan(fov_ / 2 * SM_PI / 180)*2/h;
}

void Camera::FpsRotate(float hori_deg,float verti_deg){
    // float sinh = sin(glm::radians(hori_deg)*0.5f);
    // float sinv = sin(glm::radians(verti_deg)*0.5f);
    // glm::vec3 curup = orien_*sm::OglCamPrimUp;
    // orien_*=glm::quat(cos(glm::radians(hori_deg)*0.5f),sinh*curup);
    // glm::vec3 curright = orien_*sm::OglCamPrimRight;
    // orien_*=glm::quat(cos(glm::radians(verti_deg)*0.5f),sinv*curright);
	// euler_xyz_deg_ = sm::Quat2Eul(orien_);
	// camcenter = pos_ + glm::length(camcenter-pos_)*(orien_*sm::OglCamPrimForward);
	
	euler_xyz_deg_.z+=hori_deg;
	euler_xyz_deg_.x=std::clamp(euler_xyz_deg_.x+verti_deg,0.0f,180.0f);
	orien_ = sm::Eul2Quat(euler_xyz_deg_);
	camcenter = pos_ + glm::length(camcenter-pos_)*(orien_*sm::OglCamPrimForward);
    
}




}