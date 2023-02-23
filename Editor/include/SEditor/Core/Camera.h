#pragma once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace SEditor::Core{


//Opengl Camera model: forward -z,up y
class Camera {
public:
	Camera(glm::vec3 pos=glm::vec3(5,5,5),glm::vec3 center=glm::vec3(0));
	//trans in camera coord
	void translate(glm::vec3 trans);
	
	void Orbit(float hori_deg,float verti_deg );
	void CalcLookAt();
	void FpsRotate(float hori_deg,float verti_deg);
	void zoom(float step);
	void reset();
	void resetworldup();
	float CalcDisPerPix(int w,int h); //Calc distance per pixel at the panel of cnterpoint
	glm::mat4 CalcViewMat() const;
	glm::mat4 CalcProjectionMat(int w,int h) const;
	const glm::mat4& GetViewMat() const{return view_;};
	const glm::mat4& GetProjectionMat() const{return prj_;};
	void CacheViewMat(){view_ = CalcViewMat();};
	void CacheProjectionMat(int w,int h){prj_=CalcProjectionMat(w,h);};
	glm::vec3 camcenter;
	glm::vec3 worldup{0, 0, 1};

	//CAM extrinsic and intrinsic
	float fov_=45;
	float near_=1;
	float far_=1000;
	glm::vec3 pos_;
	glm::quat orien_;
	glm::vec3 euler_xyz_deg_;
	glm::mat4 view_,prj_;
private:

};

}