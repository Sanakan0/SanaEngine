#pragma once
#include "glm/detail/type_quat.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SRender/Resources/STexture.h>
#include <stdint.h>
#include <string>
#include <vcruntime.h>
#include <vector>
namespace SRender::Resources{
struct SJoint{
	glm::vec3 trans;
	glm::quat orien;
	glm::vec3 scale;
    glm::mat4 inverse_bind_mat;
	int parent; //-1 if root
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

enum class TextureStackType:int{
    DIFFUSE = 0x0000,
    SPECULAR,
    AMBIENT,
    EMISSIVE,
    NORMALS,
};

struct AssimpTextureStack{
    STexture* data[3] {nullptr,nullptr,nullptr}; 
};

class SMesh {
public:
	SMesh(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx,bool iscached=false);
	SMesh(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int> &vidx,bool iscached=false);
	SMesh(const SMesh&) = delete;
	~SMesh();
	size_t VertexSize(){return vertex_size_;}
	size_t IdxSize(){return idx_size_;}
	unsigned int vao_=0, vbo_=0, ebo_=0;
	size_t vertex_size_,idx_size_;

	void Bind();
    void UnBind();
	void CreateBuf(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
	void CreateBuf(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int> &vidx);
	void UploadCachedBuf();
	void CreateWeightBuf();
private:
	std::vector<Vertex> vs_cache_;
	std::vector<VertexWithWeight> vsw_cache_;
	std::vector<uint32_t> vidx_cache_;
};


} 
