#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <SRender/Resources/SMesh.h>
namespace SRender::Resources{

class SModel{
public:
    SModel(std::string path);
    ~SModel();
    std::string path_;
    std::vector<SMesh*>& GetMeshes(){return meshes_;}
private:
    std::vector<SMesh*>meshes_;

};


}