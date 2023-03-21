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
void SModel::UploadMeshBuffer(){
    for (auto& m:meshes_){
        if (m->vbo_==0){
            m->UploadCachedBuf();
        }
    }
}

void SModel::CalcBoundingSphere(){
    float minx,miny,minz;
    float maxx,maxy,maxz;
    minx=miny=minz = std::numeric_limits<float>::max();
    maxx=maxy=maxz = std::numeric_limits<float>::lowest();
    for (auto& m:meshes_){
        auto& bs = m->boundingsphere_;
        minx = std::min(bs.pos.x-bs.radius,minx);
        miny = std::min(bs.pos.y-bs.radius,miny);
        minz = std::min(bs.pos.z-bs.radius,minz);

        maxx = std::max(bs.pos.x+bs.radius,maxx);
        maxy = std::max(bs.pos.y+bs.radius,maxy);
        maxz = std::max(bs.pos.z+bs.radius,maxz);
    }
    boundingsphere_.pos=glm::vec3(minx+maxx,miny+maxy,minz+maxz)/2.0f;
    float tmpr=0.0f;
    for (auto& m:meshes_){
        tmpr = std::max(glm::distance(boundingsphere_.pos,m->boundingsphere_.pos)+m->boundingsphere_.radius,tmpr);
    }
    boundingsphere_.radius=tmpr;
}

}