#include <SRender/Resources/SModel.h>
#include <SRender/Resources/AssimpParser.h>

namespace SRender::Resources{

SModel::SModel(std::string path):path_(path){
    AssimpParser::LoadModel(path, meshes_);
}
SModel::~SModel(){
    for (auto i:meshes_) delete i;
}

}