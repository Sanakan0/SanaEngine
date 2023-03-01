#pragma once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace sm{

class Transform{
public:
    Transform()=default;
    glm::vec3 local_pos_{0,0,0};
    glm::vec3 local_scale_{1,1,1};
    glm::quat local_orien_{1,0,0,0};
    glm::vec3 world_pos_{0,0,0};
    glm::vec3 world_scale_{1,1,1};
    glm::quat world_orien_{1,0,0,0};

    glm::mat4 local_mat_{1};
    glm::mat4 world_mat_{1};
};

}