#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <SRender/Resources/SMesh.h>
namespace SRender::Resources{

class SModel{
friend class SModelLoader;
public:
    SModel();
    ~SModel();
    std::string path_;
private:
    std::vector<SMesh*>meshes_;

};


}