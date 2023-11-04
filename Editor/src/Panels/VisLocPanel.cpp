#include "SEditor/Panels/VisLocPanel.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Util/NfdDialog.h"
#include "SRender/LowRenderer/Camera.h"
#include "SRender/Resources/STextureLoader.h"
#include "SGUI/Widgets/CustomWidgets.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include "spdlog/spdlog.h"
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
    name_ = "视觉定位控制面板";
    hori_scrollable_=true;

    {
        auto tmptex = SRender::Resources::STextureLoader::CreateColor(0);
        img1 = cv::Mat(tmptex->height,tmptex->width,CV_8UC4,tmptex->rawdata);
        cv::cvtColor(img1, img1, cv::COLOR_RGBA2BGR);
        cv::flip(img1, img1, 0);
        imgp1.reset(tmptex);
        imgp1->FreeRawData();
    }



    std::string filepth=R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\中文测试\DJI_0314.JPG)";
    auto tmptex = SRender::Resources::STextureLoader::LoadFromFile(filepth);
    if (tmptex!=nullptr){
        img1 = cv::Mat(tmptex->height,tmptex->width,CV_8UC4,tmptex->rawdata);
        cv::cvtColor(img1, img1, cv::COLOR_RGBA2BGR);
        cv::flip(img1, img1, 0);
        imgp1.reset(tmptex);
        imgp1->FreeRawData();
    }

    

    // cv::cvtColor(img2, tmp, cv::COLOR_BGR2RGBA);
    // imgp2.reset(SRender::Resources::STextureLoader::LoadFromMemory(tmp.data, tmp.size().width, tmp.size().height, "img2"));


}

void VisLocPanel::DrawContent(){
    static float scale = 5;
    ImVec2 lupos(ImGui::GetCursorScreenPos());
	ImVec2 lupos1(lupos);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
    ImGui::Image((void*)(uint64_t)imgp1->id, {(float)imgp1->width/scale,(float)imgp1->height/scale},ImVec2(0,1),ImVec2(1,0));
    ImGui::SameLine();
    //ImGui::Image((void*)(uint64_t)imgp2->id, {(float)imgp2->width/scale,(float)imgp2->height/scale});
    ImGui::Image((void*)(uint64_t)locengine.fbo_.tex_buf_id_,ImVec2(locengine.fbo_.buf_size_.first/scale,locengine.fbo_.buf_size_.second/scale),ImVec2(0,1),ImVec2(1,0) );

    ImGui::PopStyleVar();
    // static SRender::LowRenderer::RadialDistortion distortioninfo{{0,0,0},SRender::LowRenderer::DistortionModel::INDEX};
    // static float norm_fh=1;
    // static float sensor_scale=1;
    // undistpipeline.Run(*imgp1, norm_fh,sensor_scale, distortioninfo);
    // auto uimgw=undistpipeline.GetFbo().buf_size_.first;
    // auto uimgh=undistpipeline.GetFbo().buf_size_.second;
    // ImGui::Image((void*)(uint64_t)undistpipeline.GetFbo().tex_buf_id_, {(float)uimgw/scale,(float)uimgh/scale},ImVec2(0,1),ImVec2(1,0));
    static VisualLoc::matchpairvec res ;
    
    lupos1.x+=(float)imgp1->width/scale;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    //draw_list->AddLine(lupos1, {lupos1.x+(float)imgp1->width/scale,lupos1.y+(float)imgp1->height/scale},IM_COL32(255,0,0,255));
    for (auto &i :res){
        ImVec2 p1(lupos.x+i.pt1.pt.x/scale,lupos.y+i.pt1.pt.y/scale);
        ImVec2 p2(lupos1.x+i.pt2.pt.x/scale,lupos1.y+i.pt2.pt.y/scale);
        draw_list->AddLine(p1, p2,IM_COL32(255,255,255,255));
    }
    ImGui::Spacing();
    ImGui::DragFloat("图像缩放",&scale,0.01);
    // ImGui::DragFloat("sensor scale",&sensor_scale,0.01);
    if (ImGui::Button("打开...")){
        static std::vector<nfdfilteritem_t> filters={{"Image","png,jpg,jpeg"}};
        auto filepth = Util::NfdDialog::OpenFileDlg(filters,ResourceManager::PathManager::GetProjectPath());
        if (filepth!=""){
            auto tmptex = SRender::Resources::STextureLoader::LoadFromFile(filepth);
            if (tmptex!=nullptr){
                img1 = cv::Mat(tmptex->height,tmptex->width,CV_8UC4,tmptex->rawdata);
                cv::cvtColor(img1, img1, cv::COLOR_RGBA2BGR);
                cv::flip(img1, img1, 0);
                imgp1.reset(tmptex);
                imgp1->FreeRawData();
            }
            
        }
    }
    // const char* models[] = { "NONE","INDEX","POLY3","POLY5","PTLENS" ,"DIVISION"};
    // ImGui::Combo("Distortion Model", &distortioninfo.dist_type, models, IM_ARRAYSIZE(models));
    // ImGui::PushItemWidth(80);
    // switch (distortioninfo.dist_type) {
    // case 0:
    //     break;
    // case 1:
    //     ImGui::DragFloat("k",&distortioninfo.dist_para[0],0.001);
    //     break;
    // case 2:
    //     ImGui::DragFloat("k",&distortioninfo.dist_para[0],0.001);
    //     break;
    // case 3:
    //     ImGui::DragFloat("k1",&distortioninfo.dist_para[0],0.001);
    //     ImGui::SameLine();
    //     ImGui::DragFloat("k2",&distortioninfo.dist_para[1],0.001);
    //     break;
    // case 4:
    //     ImGui::DragFloat("a",&distortioninfo.dist_para[0],0.001);
    //     ImGui::SameLine();
    //     ImGui::DragFloat("b",&distortioninfo.dist_para[1],0.001);
    //     ImGui::SameLine();
    //     ImGui::DragFloat("c",&distortioninfo.dist_para[2],0.001);
    //     break;
    // case 5:
    //     ImGui::DragFloat("K",&distortioninfo.dist_para[0],0.001);
    //     break;
    // }
    // ImGui::DragFloat("norm_fh",&norm_fh,0.001);

    ImGui::Separator();
    
    ImGui::PushItemWidth(ImGui::GetFontSize() * 8);
    
    ImGui::Text("SURF 设置:");
    ImGui::InputFloat("lowe's ratio", &locsetting.fm_lowesratio);
    ImGui::Text("Pnp 设置:");
    ImGui::InputInt("iterationscount", &locsetting.pnp_iterationscount);
    ImGui::InputDouble("confidence", &locsetting.pnp_confidence);
    ImGui::InputFloat("reprojectionerror", &locsetting.pnp_reprojectionerror);

    ImGui::PopItemWidth();
    ImGui::Separator();
    
    if (ImGui::Button("随机采样位姿求解")){
        auto tmp = scenemanager_.GetActiveCamera();
        if (tmp==nullptr){
            spdlog::error("[VISLOC] No Camera Activated");
        }
        else{
            locengine.LocPipelineMultiRandom(img1,*scenemanager_.GetActiveCamera(),locsetting);
        }
    }
    if (ImGui::Button("位姿优化")){
        auto tmp = scenemanager_.GetActiveCamera();
        if (tmp==nullptr){
            spdlog::error("[VISLOC] No Camera Activated");
        }
        else{
            locengine.LocPipeline(img1,*scenemanager_.GetActiveCamera(),locsetting);
        }
    }
    
    if (ImGui::Button("特征测试")){
        auto tmp = scenemanager_.GetActiveCamera();
        if (tmp==nullptr){
            spdlog::error("[VISLOC] No Camera Activated");
        }
        else{
            res.clear();
            res =locengine.TestFeatureMatch(img1,*scenemanager_.GetActiveCamera(),locsetting);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("图像投影")){
        scenemanager_.enable_img_prj_^=1;
        scenemanager_.img_tex_ = imgp1;
    }

    if (ImGui::Button("保存图像")){
        auto tmp = scenemanager_.GetActiveCamera();
        if (tmp==nullptr){
            spdlog::error("[VISLOC] No Camera Activated");
        }
        else{
            img1 = locengine.TestRenderCapture(*scenemanager_.GetActiveCamera());
            cv::Mat tmp;
            cv::cvtColor(img1, tmp, cv::COLOR_BGR2RGBA);
            imgp1.reset(SRender::Resources::STextureLoader::LoadFromMemory(tmp.data, tmp.size().width, tmp.size().height, "img2"));
        }
    }
    //ImGui::Image((void*)(uint64_t)locengine.fbo_.tex_buf_id_,ImVec2(locengine.fbo_.buf_size_.first/10.0,locengine.fbo_.buf_size_.second/10.0),ImVec2(0,1),ImVec2(1,0) );

}



}