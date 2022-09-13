#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
namespace SRender::Resources{

struct Vertex {
    float pos[3];
    float norm[3];
    float texcoord[2];
};



// this will MOVE DATA IN
class GLMesh {
public:
	unsigned int vao_, vbo_, ebo_;
	std::vector <Vertex> vs_;
	std::vector <unsigned int> vidx_;
	GLMesh(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
	void Bind();
    void UnBind();
	void setup(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
};


} 
