#include <SRender/Resources/SModel.h>
#include <SRender/Resources/AssimpParser.h>

namespace SRender::Resources{

SModel::SModel(){
    
}
SModel::~SModel(){
    for (auto i:meshes_) delete i;
}


}