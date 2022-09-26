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
	float texcoord[2];
    float norm[3];
};



// this will MOVE DATA IN
class SMesh {
public:
	SMesh(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
	~SMesh();
	unsigned int vao_, vbo_, ebo_;
	std::vector <Vertex> vs_;
	std::vector <unsigned int> vidx_;
	void Bind();
    void UnBind();
	void CreateBuf(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
};


} 
