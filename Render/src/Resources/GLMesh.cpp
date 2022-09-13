#include "SRender/Resources/GLMesh.h"
#include <memory>
namespace SRender::Resources{

GLMesh::GLMesh(std::vector <Vertex>& vs, std::vector <unsigned int>& vidx) {
	setup(vs,vidx);
}

void GLMesh::setup(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx) {
	vs_ = std::move(vs);
	vidx_ = std::move(vidx);
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);
	
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vs_.size(), &vs_[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*vidx_.size(), &vidx_[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0); //vertic
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,norm)); //norm
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,texcoord)); //texcoord
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void GLMesh::Bind() {
    glBindVertexArray(vao_);
}
void GLMesh::UnBind(){
    glBindVertexArray(0);
}

}