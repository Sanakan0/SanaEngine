#include "SRender/Resources/SModel.h"
namespace SRender::Resources{

class SModelLoader{
public:
    bool LoadModel(std::string path,std::vector<SMesh*> meshes);
private:

};

}