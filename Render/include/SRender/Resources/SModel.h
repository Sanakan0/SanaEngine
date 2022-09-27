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
    std::vector<SBone>bones_;
    glm::mat4 modelmat_;
    glm::quat rotate_;
    glm::vec3 pos_;

};


}