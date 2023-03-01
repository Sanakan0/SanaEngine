#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SMath/SQuaternion.h>
#include <utility>
#include <vector>
#include <cmath>
class SimpleJoint{
public:
    SimpleJoint(){
        glm::vec3 v = glm::cross(forward, up);
        float theta = SM_PI/4;
        float sint = sin(theta/2);
        float cost = cos(theta/2);
        glm::quat midq(cost,v*sint);
        orien_keyfrm.push_back(glm::quat());
        orien_keyfrm.push_back(midq);
        orien_keyfrm.push_back(glm::quat());

        pos_keyfrm.push_back(glm::vec3(0));
        pos_keyfrm.push_back(forward*-1.0f);
        pos_keyfrm.push_back(glm::vec3(0));

        o_interv =  1.0f/(orien_keyfrm.size()-1);
        p_interv = 1.0f/(pos_keyfrm.size()-1);
    };
    ~SimpleJoint(){};
    void TickStatus(float deltat){
        if (shooting){
            normalized_curtime_+=deltat/total_t_;
            if (normalized_curtime_>=1){
                normalized_curtime_=0;
                shooting=false;
            }
            int oidx = static_cast<int>(normalized_curtime_/o_interv);
            int pidx = static_cast<int>(normalized_curtime_/p_interv);
            float oa = (normalized_curtime_-o_interv*oidx)/o_interv;
            float op = (normalized_curtime_-p_interv*pidx)/p_interv;
            cur_orien = glm::slerp(orien_keyfrm[oidx],orien_keyfrm[oidx+1], oa);
            cur_pos = glm::mix(pos_keyfrm[pidx],pos_keyfrm[pidx+1], op);
        }
    }
    glm::mat4 GetCurTickModelMat(const glm::mat4& model){
       glm::mat4 tmp = glm::mat4_cast(cur_orien);
       tmp[3]=glm::vec4(cur_pos,1);
       return model*tmp;
    };
    int Shoot(){shooting=true;return 1;}
private:
    bool shooting=false;
    float normalized_curtime_=0;
    float total_t_=0.3f;
    glm::vec3 forward{0,-1,0};
    glm::vec3 up{0,0,1};
    glm::quat cur_orien;
    glm::vec3 cur_pos;
    std::vector<glm::quat> orien_keyfrm;
    std::vector<glm::vec3> pos_keyfrm;
    float o_interv,p_interv;
};