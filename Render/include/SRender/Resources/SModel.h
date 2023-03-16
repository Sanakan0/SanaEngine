#pragma once

//#include "SRender/Resources/AssimpParser.h"
#include "SRender/Resources/SMaterial.h"
#include <glad/glad.h>
#include <memory>
#include <vector>
#include <string>
#include <SRender/Resources/SBaseRenderResources.h>
namespace SRender::Resources{



class SModel{
friend class SModelLoader;
public:
    SModel();
    ~SModel();
    std::string path_;
    std::vector<SMesh*>& GetMeshes(){return meshes_;}
    std::vector<SJoint>& GetJoints(){return joints_;}
    SMaterial* GetMaterial(){return material_.get();}
    const BoundingSphere* GetBoundingSphere(){return &boundingsphere_;}
    glm::mat4 modelmat_;
    void CalcPalette();
    void CalcDerivedJoint();
    void UploadMeshBuffer();
    std::vector<glm::mat4>palette_;
private:
    void CalcBoundingSphere();

    std::vector<SMesh*>meshes_;
    std::vector<SJoint>joints_;

    std::unique_ptr<SMaterial> material_;

    glm::quat rotate_;
    glm::vec3 pos_;

    BoundingSphere boundingsphere_;
};



}