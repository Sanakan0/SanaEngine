
#include "SEditor/Core/Cameraa.h"
#define PI 3.14159265359
namespace SEditor::Core{

Cameraa::Cameraa() {
	reset();
}

void Cameraa::reset() {
	campos = glm::vec3(5, 5, 5);
	camcenter = glm::vec3(0, 0, 0);
	camright = glm::vec3(1, 0, -1);
	worldup = glm::vec3(0, 1, 0);
}

void Cameraa::resetworldup() {
	glm::vec3 tmp = glm::cross(camright, camcenter - campos)*worldup;
	float flag = tmp[0] + tmp[1] + tmp[2];
	if ( flag<0) {
		worldup[1] *= -1;
	}
}

void Cameraa::translate(glm::vec3 trans) {
	glm::vec3 camup = glm::normalize(glm::cross(camright, camcenter - campos));
	trans = glm::mat3(camright, camup, glm::vec3(0))*trans;
	campos = trans+campos;
	camcenter = trans + camcenter;
}

void Cameraa::zoom(float ratio) {
	glm::vec3 dir = campos - camcenter;
	float dis = glm::distance(camcenter, campos);
	campos = camcenter + dir*(ratio + 1);
}

void Cameraa::around(float hori,float verti) {
	glm::mat3 camr = glm::rotate(glm::mat4(1), glm::radians(verti), camright);
	glm::vec3 relepos = campos - camcenter;
	relepos = camr*relepos;
	campos = camcenter + relepos;

	camr = glm::rotate(glm::mat4(1), glm::radians(hori), worldup);
	relepos = campos - camcenter;
	relepos = camr*relepos;
	campos = camcenter + relepos;
	camright = camr*camright;
}

glm::mat4 Cameraa::CalcViewMat()const {
	return glm::lookAt(campos, camcenter, glm::cross(camright, (camcenter - campos)));
}

glm::mat4 Cameraa::CalcProjectionMat(int w,int h) const{
	float ratio = w/static_cast<float>(h);
	return glm::perspective(fov_,ratio,near_,far_);
}

float Cameraa::CalcDisPerPix(int w,int h){
	float dis = glm::distance(camcenter, campos);
	return dis*tan(fov_ / 2 * PI / 180)*2/h;
}



}