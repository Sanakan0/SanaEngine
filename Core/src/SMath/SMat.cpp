
#include "glm/gtc/quaternion.hpp"

namespace sm {
glm::mat4 MakeMat(const glm::vec3&scale ,const glm::quat& orien,const glm::vec3& pos) {
    auto rotat = glm::mat3_cast( orien);
    rotat[0]*=scale.x;
    rotat[1]*=scale.y;
    rotat[2]*=scale.z;
    glm::mat4 res(rotat);
    res[3]=glm::vec4(pos,1);
    return res;
}
}

