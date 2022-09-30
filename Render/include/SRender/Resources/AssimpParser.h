#include "SRender/Resources/SAnimation.h"
#include <assimp/matrix4x4.h>
#include <assimp/mesh.h>
#include <memory>
#include <stdint.h>
#include <string>
#include <SRender/Resources/SMesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
namespace SRender::Resources{
// only support single model  
class AssimpParser{
public:
    AssimpParser();
    ~AssimpParser();
    bool LoadModel(glm::mat4& model_mat, std::string path,std::vector<SMesh*>& meshes);
    bool LoadModel(glm::mat4& model_mat, std::string path,std::vector<SMesh*>& meshes,std::vector<SJoint>& joints,std::vector<SAnimation>& sanimas);
private:
    void BuildName2NodeMap(const aiNode* node);
    void ProcessNode(const aiMatrix4x4& transform_mat, const aiNode* node, const aiScene *scene);
    void ProcessMaterial();
    void ProcessMesh(const aiMatrix4x4& transform_mat, const aiMesh* mesh,const aiScene* scene);
    //void ProcessBone(const aiMesh* mesh,const aiScene* scene,std::vector<SBone>& bones );
    void ProcessSkeleton(const aiNode* node, const aiScene *scene,std::vector<SJoint>& joints);
    void ProcessAnimation(const aiScene *scene,std::vector<SAnimation>& sanimas,std::vector<SJoint>& joints); //after skeletonproc
    void MarkSkeleton(const aiNode* node);
    void Clear();
    bool loadwithskeleton=0;
    std::vector<int> aimeshofmesh_;
    std::unordered_map<std::string,uint32_t> name2jointidx_;
    std::unordered_map<std::string,const aiNode*> name2ainode_;
    std::vector<std::vector<Vertex>> vertex_data_;
    std::vector<std::vector<uint32_t>> idx_data_;
    std::vector<std::vector<VertexWithWeight>> vertex_data_w_;
    aiMatrix4x4 ai_model_mat;
};

}