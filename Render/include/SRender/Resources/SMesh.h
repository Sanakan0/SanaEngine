#pragma once
#include "glm/detail/type_quat.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdint.h>
#include <string>
#include <vector>
namespace SRender::Resources{
struct SJoint{
	glm::vec3 trans;
	glm::quat orien;
	glm::vec3 scale;
    glm::mat4 inverse_bind_mat;
	int parent;
};

struct joint_weight_t{
	uint32_t joint_id;
	float weight;
};



struct joint_weight_group{
	uint32_t joint_ids[4];
	float weights[4];
};
struct Vertex {
    float pos[3];
	float texcoord[2];
    float norm[3];
};
struct VertexWithWeight{
	float pos[3];
	float texcoord[2];
    float norm[3];
	uint32_t joint_ids[4];
	float weights[4];
};



class SMesh {
public:
	SMesh(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
	SMesh(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int> &vidx);
	SMesh(const SMesh&) = delete;
	~SMesh();
	uint32_t VertexSize(){return vertex_size_;}
	uint32_t IdxSize(){return idx_size_;}
	unsigned int vao_, vbo_, ebo_;
	uint32_t vertex_size_,idx_size_;

	void Bind();
    void UnBind();
	void CreateBuf(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
	void CreateBuf(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int> &vidx);
	void CreateWeightBuf();
private:
};


} 
