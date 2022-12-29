#include "SRender/Resources/SBaseRenderResources.h"
#include <memory>
#include <stdint.h>
namespace SRender::Resources{
//simple mesh
SMesh::SMesh(std::vector <Vertex>& vs, std::vector <unsigned int>& vidx):vertex_size_(vs.size()),idx_size_(vidx.size()) {
	CreateBuf(vs,vidx);
}
//mesh with skeleton
SMesh::SMesh(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int>& vidx):vertex_size_(vs_w.size()),idx_size_(vidx.size()) {
	CreateBuf(vs_w,vidx);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vs.size(), &vs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*vidx.size(), &vidx[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0); //vertic
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,norm)); //norm
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,texcoord)); //texcoord
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void SMesh::CreateBuf(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int> &vidx) {
	// vs_ = std::move(vs);
	// vidx_ = std::move(vidx);
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);
	
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithWeight)*vs_w.size(), &vs_w[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*vidx.size(), &vidx[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithWeight), (void *)0); //vertic
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithWeight), (void *)offsetof(VertexWithWeight,norm)); //norm
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithWeight), (void *)offsetof(VertexWithWeight,texcoord)); //texcoord
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(VertexWithWeight), (void *)offsetof(VertexWithWeight,joint_ids)); //vertic
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexWithWeight), (void *)offsetof(VertexWithWeight,weights)); //vertic
	glEnableVertexAttribArray(4);
	glBindVertexArray(0);
}


void SMesh::Bind() {
    glBindVertexArray(vao_);
}
void SMesh::UnBind(){
    glBindVertexArray(0);
}

}