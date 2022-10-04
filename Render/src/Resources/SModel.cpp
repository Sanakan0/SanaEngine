#include "glm/geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include <SRender/Resources/SModel.h>
#include <SRender/Resources/AssimpParser.h>

namespace SRender::Resources{

SModel::SModel(){
}
SModel::~SModel(){
    for (auto i:meshes_) delete i;
}
void SModel::CalcPalette(){
    std::vector<glm::mat4>tmp(joints_.size());
    for (int i=0;i<joints_.size();++i){
        auto& joint = joints_[i];
        tmp[i] = glm::scale( glm::mat4_cast(joint.orien),joint.scale);
        tmp[i][3] = glm::vec4(joint.trans,1);
        tmp[i]=modelmat_ * tmp[i] * joint.inverse_bind_mat;
        
    }
    palette_.swap(tmp);
}
void SModel::CalcDerivedJoint(){
    for (int i=0;i<joints_.size();++i){
        auto& joint = joints_[i];
        if (joint.parent>-1){
            auto& pa = joints_[joint.parent];
            joint.orien=glm::normalize(pa.orien*joint.orien);
            joint.scale=pa.scale*joint.scale;
            joint.trans =pa.trans + pa.orien*(pa.scale*joint.trans);
        }
    }
}
}