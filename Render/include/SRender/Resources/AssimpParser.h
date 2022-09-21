#include <string>
#include <SRender/Resources/SMesh.h>
namespace SRender::Resources{
    
class AssimpParser{
public:
    bool LoadModel(std::string path,std::vector<SMesh*> meshes);


private:
    void ProcessNode();
    void ProcessMaterial();
    void ProcessMesh();
};
    

}