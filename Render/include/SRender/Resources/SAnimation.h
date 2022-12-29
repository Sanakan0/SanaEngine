#pragma once
#include "SRender/Resources/SModel.h"
#include "glm/detail/type_quat.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include <SRender/Resources/SBaseRenderResources.h>
namespace SRender::Resources{


struct joint_anima_t{
    std::vector<std::pair<double,glm::vec3>> trans_seq;
    std::vector<std::pair<double,glm::quat>> orien_seq;
    std::vector<std::pair<double,glm::vec3>> scale_seq;
    uint32_t t_curfrm,o_curfrm,s_curfrm;
};


class SAnimation{
public:
    SAnimation(){};
    SAnimation(std::string name,float total_t, std::vector<joint_anima_t>& joint_animation);
    ~SAnimation(){};
    void Tick(float deltaT);
    void Play(){playing_=true;}
    void Stop(){playing_=false;}
    void Reset(){curtime_=0;}
    void Playonce(){playonce_=true;}
    SModel* modelp_;
    std::string name_;
    float total_t_=0;
    std::vector<joint_anima_t> joint_animation_;
private:
    uint32_t FindCurFrm(uint32_t lastfrm,float curt);
    float curtime_=0;
    
    bool playing_=0;
    bool playonce_=0;

};

}