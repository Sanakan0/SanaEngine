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
#define TEXTURESTACK_NUM 3
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

enum TextureStackType:int{
    DIFFUSE,
    SPECULAR_GlOSSINESS,
    NORMALS
};


struct TextureStack{
    STexture* DiffuseTex;
	STexture* SpecularTex; //srgb specular glossiness in PBR. rgb is fresnel, alpha is roughness
	STexture* NormalsTex;
};

struct BoundingSphere{
	glm::vec3 pos;
	float radius;
};

class SModel;
class SMesh {
friend class SModel;
public:
	SMesh(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx,uint32_t material_idx=-1,bool iscached=false);
	SMesh(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int> &vidx,uint32_t material_idx=-1,bool iscached=false);
	SMesh(const SMesh&) = delete;
	~SMesh();
	size_t VertexSize(){return vertex_size_;}
	size_t IdxSize(){return idx_size_;}
	unsigned int vao_=0, vbo_=0, ebo_=0;
	size_t vertex_size_,idx_size_;
	uint32_t material_idx_;
	void Bind();
    void UnBind();
	void CreateBuf(std::vector <Vertex>& vs, std::vector <unsigned int> &vidx);
	void CreateBuf(std::vector <VertexWithWeight>& vs_w, std::vector <unsigned int> &vidx);
	void UploadCachedBuf();
	void CreateWeightBuf();
private:
	template<typename T>
	inline void CalcBoundingSphere(std::vector<T>& vs){
		float minx,miny,minz;
		float maxx,maxy,maxz;
		minx=miny=minz = std::numeric_limits<float>::max();
		maxx=maxy=maxz = std::numeric_limits<float>::lowest();
		for (auto& v:vs){
			minx = std::min(v.pos[0],minx);
			miny = std::min(v.pos[1],miny);
			minz = std::min(v.pos[2],minz);

			maxx = std::max(v.pos[0],maxx);
			maxy = std::max(v.pos[1],maxy);
			maxz = std::max(v.pos[2],maxz);
		}
		boundingsphere_.pos=glm::vec3(minx+maxx,miny+maxy,minz+maxz)/2.0f;
		float tmpr=0.0f;
		for (auto& v:vs){
			tmpr = std::max(glm::distance(boundingsphere_.pos, {v.pos[0],v.pos[1],v.pos[2]} ),tmpr);
		}
		boundingsphere_.radius=tmpr;
	}

	std::vector<Vertex> vs_cache_;
	std::vector<VertexWithWeight> vsw_cache_;
	std::vector<uint32_t> vidx_cache_;
	BoundingSphere boundingsphere_;
};


} 
