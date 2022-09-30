#include <SRender/Resources/SAnimation.h>

namespace SRender::Resources{

SAnimation::SAnimation(std::string name,float total_t,std::vector<joint_anima_t>& joint_animation):name_(name),total_t_(total_t), joint_animation_(std::move(joint_animation)){
    for (auto& jani:joint_animation_){
        jani.trans_seq.push_back({total_t,jani.trans_seq[jani.trans_seq.size()-1].second});
        jani.orien_seq.push_back({total_t,jani.orien_seq[jani.orien_seq.size()-1].second});
        jani.scale_seq.push_back({total_t,jani.scale_seq[jani.scale_seq.size()-1].second});
    }
}

void SAnimation::Tick(float deltaT){
    

    //if play
    for (auto& jani:joint_animation_){
        //move frm
        //TODO
        while(!(jani.trans_seq[jani.t_curfrm].first<=curtime_&&jani.trans_seq[(jani.t_curfrm+1)%jani.trans_seq.size()].first>=curtime_)) jani.t_curfrm=(jani.t_curfrm+1)%jani.trans_seq.size();
        while(!(jani.scale_seq[jani.s_curfrm].first<=curtime_&&jani.scale_seq[(jani.s_curfrm+1)%jani.scale_seq.size()].first>=curtime_)) jani.t_curfrm=(jani.t_curfrm+1)%jani.trans_seq.size();
        while(!(jani.orien_seq[jani.o_curfrm].first<=curtime_&&jani.orien_seq[(jani.o_curfrm+1)%jani.orien_seq.size()].first>=curtime_)) jani.t_curfrm=(jani.t_curfrm+1)%jani.trans_seq.size();
        

        //float at = 
    }
    

}



}