#pragma once
#include "SMath/Quaternion.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SMath/SMat.h"
namespace sm{

class Transform{
public:
    const glm::mat4& GetMat(){
        world_mat_ = MakeMat(world_scale_, world_orien_, world_pos_);
        local_mat_ = MakeMat(local_scale_, local_orien_, local_pos_);
        if (local_mat_==glm::mat4(1)){
            return total_mat_ = world_mat_;
        }
        return total_mat_ = world_mat_*local_mat_;
    }

    Transform()=default;

    //get world pos
    inline const glm::vec3& GetPosW(){ return world_pos_;}
    inline void SetPosW(const glm::vec3& pos){ world_pos_=pos;}
    //get local pos
    inline const glm::vec3& GetPosL(){ return local_pos_;}
    inline void SetPosL(const glm::vec3& pos){ local_pos_=pos;}

    //get world pos
    inline const glm::vec3& GetScaleW(){ return world_scale_;}
    inline void SetScaleW(const glm::vec3& scale){ world_scale_=scale;}
    //get local pos
    inline const glm::vec3& GetScaleL(){ return local_scale_;}
    inline void SetScaleL(const glm::vec3& scale){ local_scale_=scale;}

    inline const glm::quat& GetOrienW(){ return world_orien_;}
    inline const glm::vec3& GetOrienVecW(){ return world_orien_*sm::OglCamPrimForward;}
    inline void SetOrienW(const glm::quat& orien){ 
        world_orien_=orien;
        world_euler_xyz_deg_=Quat2Eul(world_orien_);
    }
    
    inline const glm::quat& GetOrienL(){ return local_orien_;}
    inline void SetOrienL(const glm::quat& orien){ local_orien_=orien;}

    
    //get world pos
    inline const glm::vec3& GetEulerW(){ return world_euler_xyz_deg_;}
    inline void SetEulerW(const glm::vec3& Euler){ 
        world_euler_xyz_deg_=Euler;
        world_orien_=Eul2Quat(world_euler_xyz_deg_);
    }
    //get local pos
    inline const glm::vec3& GetEulerL(){ return local_euler_xyz_deg_;}
    inline void SetEulerL(const glm::vec3& Euler){ local_euler_xyz_deg_=Euler;}


private:
    glm::vec3 local_pos_{0,0,0};
    glm::vec3 local_scale_{1,1,1};
    glm::quat local_orien_{1,0,0,0};
    glm::vec3 world_pos_{0,0,0};
    glm::vec3 world_scale_{1,1,1};
    glm::quat world_orien_{1,0,0,0};
    
    glm::vec3 world_euler_xyz_deg_{0,0,0};
    glm::vec3 local_euler_xyz_deg_{0,0,0};


    glm::mat4 local_mat_{1};
    glm::mat4 world_mat_{1};
    glm::mat4 total_mat_{1};
};

}