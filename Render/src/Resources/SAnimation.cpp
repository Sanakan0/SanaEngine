#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_float.hpp"
#include <SRender/Resources/SAnimation.h>

namespace SRender::Resources{

SAnimation::SAnimation(std::string name,float total_t,std::vector<joint_anima_t>& joint_animation):name_(name),total_t_(total_t), joint_animation_(std::move(joint_animation)){
    for (auto& jani:joint_animation_){ //some modal dont give init joint data
        if (jani.trans_seq.size()==0) jani.trans_seq.push_back({0,glm::vec3(0)});
        if (jani.orien_seq.size()==0) jani.orien_seq.push_back({0,glm::quat(0,0,0,1)});
        if (jani.scale_seq.size()==0) jani.scale_seq.push_back({0,glm::vec3(1)});
        jani.trans_seq.push_back({total_t,jani.trans_seq[jani.trans_seq.size()-1].second});
        jani.orien_seq.push_back({total_t,jani.orien_seq[jani.orien_seq.size()-1].second});
        jani.scale_seq.push_back({total_t,jani.scale_seq[jani.scale_seq.size()-1].second});
    }
}

void SAnimation::Tick(float deltaT){
    
    if(!playing_) return;
    curtime_+=deltaT;

    if(playonce_ && curtime_>total_t_){
        curtime_=0;
        playing_=0;
    }
    curtime_= fmod(curtime_,total_t_);

    //if play
    for (int i=0;i<joint_animation_.size();++i){
        //move frm
        //TODO
        auto& jani = joint_animation_[i];

        while(!(jani.trans_seq[jani.t_curfrm].first<=curtime_&&jani.trans_seq[(jani.t_curfrm+1)%jani.trans_seq.size()].first>=curtime_)) jani.t_curfrm=(jani.t_curfrm+1)%jani.trans_seq.size();
        while(!(jani.scale_seq[jani.s_curfrm].first<=curtime_&&jani.scale_seq[(jani.s_curfrm+1)%jani.scale_seq.size()].first>=curtime_)) jani.s_curfrm=(jani.s_curfrm+1)%jani.scale_seq.size();
        while(!(jani.orien_seq[jani.o_curfrm].first<=curtime_&&jani.orien_seq[(jani.o_curfrm+1)%jani.orien_seq.size()].first>=curtime_)) jani.o_curfrm=(jani.o_curfrm+1)%jani.orien_seq.size();
        
        float alpha_t = (curtime_-jani.trans_seq[jani.t_curfrm].first)/
        (jani.trans_seq[jani.t_curfrm+1].first-jani.trans_seq[jani.t_curfrm].first);
        float alpha_o = (curtime_-jani.orien_seq[jani.o_curfrm].first)/
        (jani.orien_seq[jani.o_curfrm+1].first-jani.orien_seq[jani.o_curfrm].first);
        float alpha_s = (curtime_-jani.scale_seq[jani.s_curfrm].first)/
        (jani.scale_seq[jani.s_curfrm+1].first-jani.scale_seq[jani.s_curfrm].first);
        modelp_->GetJoints()[i].orien = glm::slerp(jani.orien_seq[jani.o_curfrm].second,jani.orien_seq[jani.o_curfrm+1].second,alpha_o);
        modelp_->GetJoints()[i].trans = glm::mix(jani.trans_seq[jani.t_curfrm].second,jani.trans_seq[jani.t_curfrm+1].second,alpha_t);
        modelp_->GetJoints()[i].scale = glm::mix(jani.scale_seq[jani.s_curfrm].second,jani.scale_seq[jani.s_curfrm+1].second,alpha_s);
        
        //float at = 
    }
    

}



}