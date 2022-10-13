#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <vector>
#include <cmath>
class SimpleJoint{
public:
    SimpleJoint(){
        float pi = 4*atan(1);
        glm::vec3 v = glm::cross(forward, up);
        float theta = pi/4;
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
        if (playing){
            normalized_curtime_+=deltat/total_t_;
            normalized_curtime_ -= floor(normalized_curtime_);
            

            int oidx = normalized_curtime_/o_interv;
            int pidx = normalized_curtime_/p_interv;
            float oa = (normalized_curtime_-o_interv*oidx)/o_interv;
            float op = (normalized_curtime_-p_interv*pidx)/p_interv;
            orien_keyfrm[oidx]= glm::normalize(orien_keyfrm[oidx]);
            orien_keyfrm[oidx+1] = glm::normalize(orien_keyfrm[oidx+1]);
            cur_orien = glm::slerp(orien_keyfrm[oidx],orien_keyfrm[oidx+1], oa);
            cur_pos = glm::mix(pos_keyfrm[pidx],pos_keyfrm[pidx+1], op);
        }
    }
    glm::mat4 GetCurTickModelMat(const glm::mat4& model){
       glm::mat4 tmp = glm::mat4_cast(cur_orien);
       tmp[3]=glm::vec4(cur_pos,1);
       return model*tmp;
    };
    int Play(){playing=true;return 1;}

    std::vector<glm::quat> orien_keyfrm;
private:
    bool playing=false;
    float normalized_curtime_=0;
    float total_t_=2.0f;
    glm::vec3 forward{0,-1,0};
    glm::vec3 up{0,0,1};
    glm::quat cur_orien;
    glm::vec3 cur_pos;
    
    std::vector<glm::vec3> pos_keyfrm;
    float o_interv,p_interv;
};