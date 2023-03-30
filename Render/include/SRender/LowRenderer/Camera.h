#pragma once
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>
namespace SRender::LowRenderer{


//Opengl Camera model: forward -z,up y
class Camera {
public:
	Camera();


	glm::mat4 CalcViewMat(const glm::vec3& pos,const glm::quat& orien) const;
	glm::mat4 CalcProjectionMat(float w,float h) const;
	const glm::mat4& GetViewMat() const{return view_;};
	const glm::mat4& GetProjectionMat() const{return prj_;};
	void CacheViewMat(const glm::vec3& pos,const glm::quat& orien){view_ = CalcViewMat(pos,orien);};
	void CacheProjectionMat(float w,float h){prj_=CalcProjectionMat(w,h);};


	void Setfovy(float fovy){
		fovy_ = fovy;
		focal_length_ = sensor_size_h_ /2.0 / std::tan(glm::radians(fovy_/2));
	}
	void Setaspect_ratio(float aspect){aspect_ratio_=aspect;}
	void Setfocal_length(float focal){
		focal_length_=focal;
		fovy_ = glm::degrees(2*std::atan( sensor_size_h_/2.0/focal_length_));
	}
	void Setsensor_size_h(float sh){
		sensor_size_h_ = sh;
		fovy_ = glm::degrees(2*std::atan( sensor_size_h_/2.0/focal_length_));
	}
	float Getfovy(){return fovy_;}
	float Getaspect_ratio(){return aspect_ratio_;}
	float Getfocal_length(){return focal_length_;}
	float Getsensor_size_h(){return sensor_size_h_;}
	//CAM extrinsic and intrinsic
	
	float near_=1;
	float far_=1000;

	
	glm::vec4 clear_color_{0.2,0.2,0.5,1.0};
	
private:
	float fovy_=27;
	float focal_length_=50;
	float sensor_size_h_=24;
	float aspect_ratio_=1.5; //3:2
	glm::mat4 view_,prj_;
};

}