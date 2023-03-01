#pragma once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace SRender::LowRenderer{


//Opengl Camera model: forward -z,up y
class Camera {
public:
	Camera();
	//trans in camera coord
	void translate(glm::vec3 trans);
	
	void Orbit(float hori_deg,float verti_deg );
	void CalcLookAt();
	void FpsRotate(float hori_deg,float verti_deg);
	void zoom(float step);
	void reset();
	void resetworldup();
	float CalcDisPerPix(int w,int h); //Calc distance per pixel at the panel of cnterpoint
	glm::mat4 CalcViewMat(const glm::vec3& pos,const glm::quat& orien) const;
	glm::mat4 CalcProjectionMat(int w,int h) const;
	const glm::mat4& GetViewMat() const{return view_;};
	const glm::mat4& GetProjectionMat() const{return prj_;};
	void CacheViewMat(const glm::vec3& pos,const glm::quat& orien){view_ = CalcViewMat(pos,orien);};
	void CacheProjectionMat(int w,int h){prj_=CalcProjectionMat(w,h);};

	//CAM extrinsic and intrinsic
	float fov_=45;
	float near_=1;
	float far_=1000;

	glm::mat4 view_,prj_;
private:

};

}