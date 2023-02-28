#pragma once
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace sm{

class STransform{
public:
    STransform()=default;
    glm::vec3 pos_{0,0,0};
    glm::vec3 scale_{1,1,1};
    glm::quat rotate_{1,0,0,0};
};

}