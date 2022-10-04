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
    std::vector<SMesh*>& GetMeshes(){return meshes_;}
    std::vector<SJoint>& GetJoints(){return joints_;}
    glm::mat4 modelmat_;
    void CalcPalette();
    void CalcDerivedJoint();
    std::vector<glm::mat4>palette_;
private:

    std::vector<SMesh*>meshes_;
    std::vector<SJoint>joints_;
    
    glm::quat rotate_;
    glm::vec3 pos_;

};



}