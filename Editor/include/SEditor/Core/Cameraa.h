#pragma once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace SEditor::Core{

class Cameraa {
public:
	Cameraa();
	void translate(glm::vec3 trans);
	void around(float hori,float verti );
	void zoom(float step);
	void reset();
	void resetworldup();
	float CalcDisPerPix(int w,int h); //Calc distance per pixel at the panel of cnterpoint
	glm::mat4 CalcViewMat() const;
	glm::mat4 CalcProjectionMat(int w,int h) const;
	glm::mat4 CalcViewMat(glm::vec3 pos, glm::quat quaternion) const;
	const glm::mat4& GetViewMat() const{return view_;};
	const glm::mat4& GetProjectionMat() const{return prj_;};
	void CacheViewMat(){view_ = CalcViewMat();};
	void CacheProjectionMat(int w,int h){prj_=CalcProjectionMat(w,h);};
	glm::vec3 campos, camcenter, camright;
	glm::vec3 worldup;
	float fov_=45;
	float near_=1;
	float far_=1000;
private:
	glm::mat4 view_,prj_;
};

}