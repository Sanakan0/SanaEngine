#include "SEditor/Panels/VisLocPanel.h"
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/STextureLoader.h"
#include "SGUI/Widgets/CustomWidgets.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include <SGUI/IconsFontAwesome6.h>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdint.h>

namespace SEditor::Panels {

VisLocPanel::VisLocPanel():
scenemanager_(SANASERVICE(SceneSys::SceneManager)){
    window_padding_={0,0};
    hori_scrollable_=true;
    img1 = cv::imread(R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\GovFacility\DJI_0314.JPG)");
    img2 = cv::imread(R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\GovFacility\DJI_0315.JPG)");
    

    cv::Mat tmp;
    cv::cvtColor(img1, tmp, cv::COLOR_BGR2RGBA);
    imgp1.reset(SRender::Resources::STextureLoader::LoadFromMemory(tmp.data, tmp.size().width, tmp.size().height, "img1"));


    cv::cvtColor(img2, tmp, cv::COLOR_BGR2RGBA);
    imgp2.reset(SRender::Resources::STextureLoader::LoadFromMemory(tmp.data, tmp.size().width, tmp.size().height, "img2"));


}

void VisLocPanel::DrawContent(){
    float scale = 5;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
    ImGui::Image((void*)(uint64_t)imgp1->id, {(float)imgp1->width/scale,(float)imgp1->height/scale});
    ImGui::SameLine();
    //ImGui::Image((void*)(uint64_t)imgp2->id, {(float)imgp2->width/scale,(float)imgp2->height/scale});
    ImGui::Image((void*)(uint64_t)locengine.fbo_.tex_buf_id_,ImVec2(locengine.fbo_.buf_size_.first/scale,locengine.fbo_.buf_size_.second/scale),ImVec2(0,1),ImVec2(1,0) );

    ImGui::PopStyleVar();
    static VisualLoc::matchpairvec res ;
    ImVec2 lupos(canvas_pos_.first,canvas_pos_.second);
	ImVec2 lupos1(lupos);
    lupos1.x+=(float)imgp1->width/scale;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    //draw_list->AddLine(lupos1, {lupos1.x+(float)imgp1->width/scale,lupos1.y+(float)imgp1->height/scale},IM_COL32(255,0,0,255));
    for (auto &i :res){
        ImVec2 p1(lupos.x+i.pt1.pt.x/scale,lupos.y+i.pt1.pt.y/scale);
        ImVec2 p2(lupos1.x+i.pt2.pt.x/scale,lupos1.y+i.pt2.pt.y/scale);
        draw_list->AddLine(p1, p2,IM_COL32(255,255,255,255));
    }
    if (ImGui::Button("test")){
        locengine.LocPipeline(img1,*scenemanager_.GetActiveCamera());

    }
    if (ImGui::Button("save")){
        img1 = locengine.Test2(*scenemanager_.GetActiveCamera());
        cv::Mat tmp;
        cv::cvtColor(img1, tmp, cv::COLOR_BGR2RGBA);
        imgp1.reset(SRender::Resources::STextureLoader::LoadFromMemory(tmp.data, tmp.size().width, tmp.size().height, "img2"));
    }
    if (ImGui::Button("feature test")){
        res.clear();
        res =locengine.Test(img1,*scenemanager_.GetActiveCamera());
    }
    //ImGui::Image((void*)(uint64_t)locengine.fbo_.tex_buf_id_,ImVec2(locengine.fbo_.buf_size_.first/10.0,locengine.fbo_.buf_size_.second/10.0),ImVec2(0,1),ImVec2(1,0) );

}



}