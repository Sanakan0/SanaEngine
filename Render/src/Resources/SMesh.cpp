#include "SRender/Resources/SMesh.h"
#include <memory>
namespace SRender::Resources{

SMesh::SMesh(std::vector <Vertex>& vs, std::vector <unsigned int>& vidx):vs_(vs),vidx_(vidx) {
	CreateBuf(vs,vidx);
}

SMesh::~SMesh(){
	glDeleteVertexArrays(1,&vao_);
	glDeleteBuffers(1,&vbo_);
	glDeleteBuffers(1,&ebo_);
}

void SMesh::CreateBuf(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx) {
	// vs_ = std::move(vs);
	// vidx_ = std::move(vidx);
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
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,norm)); //norm
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,texcoord)); //texcoord
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void SMesh::Bind() {
    glBindVertexArray(vao_);
}
void SMesh::UnBind(){
    glBindVertexArray(0);
}

}