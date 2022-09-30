
#include <SRender/Resources/SModelLoader.h>
#include <SRender/Resources/AssimpParser.h>
namespace SRender::Resources{
void SModelLoader::LoadSimpleModel(std::string path, SModel &model){
    AssimpParser parser;
    model.path_=path;
    parser.LoadModel(model.modelmat_,path,model.meshes_);
    model.modelmat_=model.modelmat_;
}
void SModelLoader::LoadModelWithAnima(std::string path, SModel &model, std::vector<SAnimation> &animas){
    AssimpParser parser;
    model.path_=path;
    parser.LoadModel(model.modelmat_,path,model.meshes_,model.joints_,animas);
    model.modelmat_=model.modelmat_;
}
}