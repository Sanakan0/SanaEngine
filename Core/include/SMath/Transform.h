#pragma once
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
    glm::vec3 local_pos_{0,0,0};
    glm::vec3 local_scale_{1,1,1};
    glm::quat local_orien_{1,0,0,0};
    glm::vec3 world_pos_{0,0,0};
    glm::vec3 world_scale_{1,1,1};
    glm::quat world_orien_{1,0,0,0};

private:
    glm::mat4 local_mat_{1};
    glm::mat4 world_mat_{1};
    glm::mat4 total_mat_{1};
};

}