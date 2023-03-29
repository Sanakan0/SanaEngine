#include "SEditor/Panels/VisLocPanel.h"
#include "SRender/Resources/STextureLoader.h"
#include "SGUI/Widgets/CustomWidgets.h"
#include "imgui/imgui.h"
#include <SGUI/IconsFontAwesome6.h>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdint.h>

namespace SEditor::Panels {

VisLocPanel::VisLocPanel(){
    window_padding_={0,0};

    img1 = cv::imread(R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\GovFacility\DJI_0315.JPG)");
    img2 = cv::imread(R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\GovFacility\DJI_0316.JPG)");
    img1.resize(2048);
    img2.resize(2048);
    cv::Mat tmp;
    cv::cvtColor(img1, tmp, cv::COLOR_BGR2RGBA);
    cv::flip(tmp, tmp, 1);
    imgp1.reset(SRender::Resources::STextureLoader::LoadFromMemory(tmp.data, tmp.size().width, tmp.size().height, "img1"));
     

    cv::cvtColor(img2, tmp, cv::COLOR_BGR2RGBA);
    cv::flip(tmp, tmp, 1);
    imgp2.reset(SRender::Resources::STextureLoader::LoadFromMemory(tmp.data, tmp.size().width, tmp.size().height, "img2"));


}

void VisLocPanel::DrawContent(){
    float scale = 5;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
    ImGui::Image((void*)(uint64_t)imgp1->id, {(float)imgp1->width/scale,(float)imgp1->height/scale});
    ImGui::SameLine();
    ImGui::Image((void*)(uint64_t)imgp2->id, {(float)imgp2->width/scale,(float)imgp2->height/scale});
    ImGui::PopStyleVar();
    static auto res = locengine.ORBFeatureMatch(img1, img2);
    ImVec2 lupos(canvas_pos_.first,canvas_pos_.second);
	ImVec2 lupos1(lupos);
    lupos1.x+=(float)imgp1->width/scale;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    draw_list->AddLine(lupos1, {lupos1.x+(float)imgp1->width/scale,lupos1.y+(float)imgp1->height/scale},IM_COL32(255,0,0,255));
    for (auto &i :res){
        ImVec2 p1(lupos.x+i.pt2.pt.x/scale,lupos.y+i.pt2.pt.y/scale);
        ImVec2 p2(lupos1.x+i.pt1.pt.x/scale,lupos1.y+i.pt1.pt.y/scale);
        draw_list->AddLine(p1, p2,IM_COL32(255,255,255,255));
    }
}



}