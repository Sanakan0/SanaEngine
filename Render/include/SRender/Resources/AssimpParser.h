#include <assimp/mesh.h>
#include <string>
#include <SRender/Resources/SMesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace SRender::Resources{
    
class AssimpParser{
public:
   static bool LoadModel(std::string path,std::vector<SMesh*>& meshes);


private:
    static void ProcessNode(const aiNode* node, const aiScene *scene,std::vector<SMesh*>& meshes);
    static void ProcessMaterial();
    static void ProcessMesh(const aiMesh* mesh,const aiScene* scene,std::vector<SMesh*>& meshes );
};
    

}