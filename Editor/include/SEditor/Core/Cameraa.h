#pragma once
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
	glm::vec3 campos, camcenter, camright;
	glm::vec3 worldup;
	float fov_=45;
	float near_=10;
	float far_=1000;
};

}