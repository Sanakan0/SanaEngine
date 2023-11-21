#include "SEditor/Panels/DistortionRectiferPanel.h"
#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/RectifyComponent.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Util/NfdDialog.h"
#include "SGUI/IconsFontAwesome6.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"
#include "imgui/imgui.h"
#include "spdlog/spdlog.h"
#include <cstddef>
#include <filesystem>
#include <vector>

namespace SEditor::Panels{

DistrotionRectifierPanel::DistrotionRectifierPanel():
sceneManager_(SANASERVICE(SceneSys::SceneManager)){
    window_padding_={0,0};
    name_ = "畸变矫正面板";
    hori_scrollable_=true;
    {

        auto tmptex = SRender::Resources::STextureLoader::CreateColor(0);
        
        uimgp.reset(tmptex);
        uimgp->FreeRawData();
    }
    std::string filepth=R"(C:\Users\cnt0\BUAA\DATA\去畸变监控图像\test1.png)";
    auto tmptex = SRender::Resources::STextureLoader::LoadFromFile(filepth);
    if (tmptex!=nullptr)
        uimgp.reset(tmptex);
}

void DistrotionRectifierPanel::DrawContent(){
    
    auto curactorp = sceneManager_.GetSelectedActor();
    if (actor_!=curactorp) actor_=curactorp;
    if (actor_==nullptr) return;
    auto Rectifycomp = static_cast<ECS::Components::RectifyComponent*>(
         actor_->GetComponent("RectifyComponent"));
    auto Camcomp = static_cast<ECS::Components::CameraComponent*>(
         actor_->GetComponent("CameraComponent"));
    if (Rectifycomp==nullptr||Camcomp==nullptr){
        return;
    }


    if (Rectifycomp->img_pth_!=uimgp->path){
        SRender::Resources::STexture* tmptex;
        if (Rectifycomp->img_pth_==""){
            tmptex = SRender::Resources::STextureLoader::CreateColor(0);
            tmptex->path="";
        }else{
            tmptex = SRender::Resources::STextureLoader::LoadFromFile(Rectifycomp->img_pth_);
        }
        
        if (tmptex!=nullptr){
            uimgp.reset(tmptex);
        }
        
    }

    
    static float scale = 5;
    std::vector<std::vector<std::pair<double,double>>>& lines = Rectifycomp->lines;
    static int selected = -1;
    ImVec2 lupos(ImGui::GetCursorScreenPos());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
    //ImGui::Image((void*)(uint64_t)uimgp->id, {(float)uimgp->width/scale,(float)uimgp->height/scale},ImVec2(0,1),ImVec2(1,0));

     ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
    float my_tex_w = uimgp->width/scale;
    float my_tex_h = uimgp->height/scale;
    ImGui::Image((void*)(uint64_t)uimgp->id, ImVec2(my_tex_w,my_tex_h),ImVec2(0,1),ImVec2(1,0));
    auto& io = ImGui::GetIO();
    float cursor_onimg_x=io.MousePos.x - pos.x;
    float cursor_onimg_y=io.MousePos.y - pos.y;
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        
        float region_sz = 32.0f;
        float region_x = cursor_onimg_x - region_sz * 0.5f;
        float region_y = cursor_onimg_y - region_sz * 0.5f;
        float zoom = 10.0f;
        if (region_x < 0.0f) { region_x = 0.0f; }
        else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
        if (region_y < 0.0f) { region_y = 0.0f; }
        else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
        ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
        ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
        ImGui::Text("Center: (%.2f, %.2f)", cursor_onimg_x, cursor_onimg_y);
        
        ImVec2 tippos = ImGui::GetCursorScreenPos();
        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, 1.0-(region_y) / my_tex_h);
        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, 1.0-(region_y + region_sz) / my_tex_h);
        ImGui::Image((void*)(uint64_t)uimgp->id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
        
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        auto tmpx = (cursor_onimg_x-region_x)*zoom;
        auto tmpy = (cursor_onimg_y-region_y)*zoom;
        auto tipcnter = ImVec2(tippos.x+tmpx,tippos.y+tmpy);
        
        draw_list->AddLine(ImVec2(tippos.x,tipcnter.y),ImVec2(tippos.x+region_sz*zoom,tipcnter.y), IM_COL32(255, 255,255,255));
        draw_list->AddLine(ImVec2(tipcnter.x,tippos.y),ImVec2(tipcnter.x,tippos.y+region_sz*zoom), IM_COL32(255, 255,255,255));
        draw_list->AddCircleFilled(tipcnter, 1, IM_COL32(255,255,255,255));
        
        ImGui::EndTooltip();

        scale += io.MouseWheel*0.2;
        scale = std::max(scale,0.1f);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
            if (0<=selected&&selected<lines.size()){
                float coord_u=cursor_onimg_x/my_tex_w;
                float coord_v=cursor_onimg_y/my_tex_h;
                if (coord_u<=1&&coord_u>=0&&coord_v<=1&&coord_v>=0){
                    lines[selected].push_back({coord_u,coord_v});
                }
            }
        }

    }

    //distortion img

    ImGui::PopStyleVar();
    auto &distortioninfo = Camcomp->cam_.distortion_;
    float norm_fh=my_tex_w/(my_tex_h*2);
    //static float norm_fh=0.5;
    static float sensor_scale=1;
    undistpipeline.Run(*uimgp, norm_fh,sensor_scale, distortioninfo);
    auto uimgw=undistpipeline.GetFbo().buf_size_.first;
    auto uimgh=undistpipeline.GetFbo().buf_size_.second;
    ImGui::SameLine();
    ImVec2 uimg_lupos(ImGui::GetCursorScreenPos());
    ImGui::Image((void*)(uint64_t)undistpipeline.GetFbo().tex_buf_id_, {(float)uimgw/scale,(float)uimgh/scale},ImVec2(0,1),ImVec2(1,0));

    ImGui::Spacing();

   
   
    
    // lupos1.x+=(float)imgp1->width/scale;

	// ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // //draw_list->AddLine(lupos1, {lupos1.x+(float)imgp1->width/scale,lupos1.y+(float)imgp1->height/scale},IM_COL32(255,0,0,255));
    // for (auto &i :res){
    //     ImVec2 p1(lupos.x+i.pt1.pt.x/scale,lupos.y+i.pt1.pt.y/scale);
    //     ImVec2 p2(lupos1.x+i.pt2.pt.x/scale,lupos1.y+i.pt2.pt.y/scale);
    //     draw_list->AddLine(p1, p2,IM_COL32(255,255,255,255));
    // }
    ImGui::DragFloat("图像缩放",&scale,0.01);
    ImGui::DragFloat("CCD尺寸缩放",&sensor_scale,0.01);
    if (ImGui::Button("打开...")){
        static std::vector<nfdfilteritem_t> filters={{"Image","png,jpg,jpeg"}};
        auto filepth = Util::NfdDialog::OpenFileDlg(filters,ResourceManager::PathManager::GetProjectPath());
        if (filepth!=""){
            auto path = std::filesystem::u8path(filepth);
            auto filename = path.filename().u8string();
            auto savepth = Util::NfdDialog::SaveDlg(filters,filename,ResourceManager::PathManager::GetProjectPath());
            if (savepth!=""){
                std::filesystem::copy_file(std::filesystem::u8path(filepth),std::filesystem::u8path(savepth),std::filesystem::copy_options::overwrite_existing );
            }
            auto relative = std::filesystem::relative(std::filesystem::u8path(savepth),
            std::filesystem::u8path(ResourceManager::PathManager::GetProjectPath()));
            std::cout <<"fk" << relative.generic_u8string()<<std::endl;
            Rectifycomp->img_pth_="@"+relative.generic_u8string();
            
        }
    }
    ImGui::Spacing();
    //add line
    ImGui::Separator();
    ImGui::Text("直线标定:");

   
    //ImGui::PushItemWidth(50);
    for (int n = 0; n < lines.size(); n++)
    {
        char buf[32];
        sprintf(buf, "Line %d", n);
        auto h=ImGui::GetFrameHeight();
        if (ImGui::Selectable(buf, selected == n,0,{100,h})){
            selected = n;
            
            
        }
        if (selected==n){
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_ROTATE_LEFT)&&!lines[selected].empty()){
                lines[selected].pop_back();
            }
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_XMARK)){
                lines.erase(lines.begin()+selected);
            }
        }
            
        
    }
    //ImGui::PopItemWidth();
    if (ImGui::Button("添加直线")){
        lines.push_back({});
        selected=lines.size()-1;
    }
    double aspect_ratio = (double)uimgp->width/uimgp->height;
    ImGui::Separator();

    if (ImGui::Button("标定弧度误差")){
        spdlog::info("[Recifier] curv st");
        Rectifycomp->lossval_= rectifier_.RectifyWithLines(lines, aspect_ratio, distortioninfo);
        spdlog::info("[Recifier] curv ed");
    }
    if (ImGui::Button("标定距离误差")){
        spdlog::info("[Recifier] dist st");
        Rectifycomp->lossval_= rectifier_.RectifyWithLines(lines, aspect_ratio, distortioninfo,true);
        spdlog::info("[Recifier] dist ed");
    }

    
    ImGui::Separator();
    ImGui::Text("标定信息:");
    ImGui::Spacing();
    ImGui::Text("error: %.8f",Rectifycomp->lossval_);
    VisualLoc::LinesT distlines;
    auto [dloss,aloss,dderiv,aderiv] = rectifier_.GetCurLoss(lines, aspect_ratio, distortioninfo,&distlines);
    ImGui::Text("Angle Loss: %.8f Dist Loss: %.8f Dist Loss pixel wise: %.8f", aloss,dloss,dloss*uimgp->width/2.0);
    ImGui::Text("Angle deriv: %.8f Dist deriv: %.8f", aderiv,dderiv);
    



    //renderline
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    auto linecol = IM_COL32(0,0,255,255);
    auto ptcol = IM_COL32(255,0,0,255);
    float ptsz=2;
    for (int k=0;k<lines.size();++k){
        auto& line=lines[k];
        if (line.size()<=0) continue;
        auto x = line[0].first*my_tex_w;
        auto y = line[0].second*my_tex_h;
        ImVec2 p0(lupos.x+x,lupos.y+y);
        draw_list->AddCircleFilled(p0, ptsz, ptcol);
        for (int i=1;i<line.size();++i){
            auto x = line[i].first*my_tex_w;
            auto y = line[i].second*my_tex_h;
            ImVec2 p1(lupos.x+x,lupos.y+y);
            //ImVec2 p2(lupos1.x+i.pt2.pt.x/scale,lupos1.y+i.pt2.pt.y/scale);
            draw_list->AddCircleFilled(p1, ptsz, ptcol);
            draw_list->AddLine(p0, p1, selected==k?ptcol:linecol);
            p0=p1;
        }
    }
    for (int k=0;k<distlines.size();++k){
        auto& line=distlines[k];
        if (line.size()<=0) continue;
        auto x = line[0].first/2;
        auto y = line[0].second/2*aspect_ratio;
        x+=0.5*sensor_scale;
        y+=0.5*sensor_scale;
        x*=(float)my_tex_w;
        y*=(float)my_tex_h;
        ImVec2 p0(uimg_lupos.x+x,uimg_lupos.y+y);
        draw_list->AddCircleFilled(p0, ptsz, ptcol);
        for (int i=1;i<line.size();++i){
            auto x = line[i].first/2;
            auto y = line[i].second/2*aspect_ratio;
            x+=0.5*sensor_scale;
            y+=0.5*sensor_scale;
            x*=(float)my_tex_w;
            y*=(float)my_tex_h;
            ImVec2 p1(uimg_lupos.x+x,uimg_lupos.y+y);
            //ImVec2 p2(lupos1.x+i.pt2.pt.x/scale,lupos1.y+i.pt2.pt.y/scale);
            draw_list->AddCircleFilled(p1, ptsz, ptcol);
            draw_list->AddLine(p0, p1, selected==k?ptcol:linecol);
            p0=p1;
        }
    }
    // if (0<=selected&&selected<lines.size()
    //             &&lines[selected].size()!=0){
    //     auto x = lines[selected][0].first*my_tex_w;
    //     auto y = lines[selected][0].second*my_tex_h;
    //     ImVec2 p0(lupos.x+x,lupos.y+y);
    //     draw_list->AddCircleFilled(p0, ptsz, ptcol);
    //     for (int i=1;i<lines[selected].size();++i){
    //         auto x = lines[selected][i].first*my_tex_w;
    //         auto y = lines[selected][i].second*my_tex_h;
    //         ImVec2 p1(lupos.x+x,lupos.y+y);
    //         //ImVec2 p2(lupos1.x+i.pt2.pt.x/scale,lupos1.y+i.pt2.pt.y/scale);
    //         draw_list->AddCircleFilled(p1, ptsz, ptcol);
    //         draw_list->AddLine(p0, p1,linecol);
    //         p0=p1;
    //     }
    // }
    



    ImGui::Separator();
    ImGui::Text("畸变参数:");
    
    
     const char* models[] = { "NONE","INDEX","POLY3","POLY5","PTLENS" ,"DIVISION"};
    ImGui::Combo("畸变模型", &distortioninfo.dist_type, models, IM_ARRAYSIZE(models));
    ImGui::PushItemWidth(80);
    switch (distortioninfo.dist_type) {
    case 0:
        break;
    case 1:
        ImGui::DragFloat("k",&distortioninfo.dist_para[0],0.0001,-100,100,"%.5f");
        break;
    case 2:
        ImGui::DragFloat("k",&distortioninfo.dist_para[0],0.0001,-100,100,"%.5f");
        break;
    case 3:
        ImGui::DragFloat("k1",&distortioninfo.dist_para[0],0.0001,-100,100,"%.5f");
        ImGui::SameLine();
        ImGui::DragFloat("k2",&distortioninfo.dist_para[1],0.0001,-100,100,"%.5f");
        break;
    case 4:
        ImGui::DragFloat("a",&distortioninfo.dist_para[0],0.001);
        ImGui::SameLine();
        ImGui::DragFloat("b",&distortioninfo.dist_para[1],0.001);
        ImGui::SameLine();
        ImGui::DragFloat("c",&distortioninfo.dist_para[2],0.001);
        break;
    case 5:
        ImGui::DragFloat("K",&distortioninfo.dist_para[0],0.0001,-100,100,"%.5f");
        break;
    }
    ImGui::DragFloat("norm_fh",&norm_fh,0.001);
}



}