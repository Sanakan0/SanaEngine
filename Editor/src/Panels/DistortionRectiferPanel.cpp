#include "SEditor/Panels/DistortionRectiferPanel.h"
#include "SEditor/Util/NfdDialog.h"
#include "SRender/Resources/STextureLoader.h"
#include "imgui/imgui.h"
#include <cstddef>
#include <vector>

namespace SEditor::Panels{

DistrotionRectifierPanel::DistrotionRectifierPanel(){
    window_padding_={0,0};
    name_ = "畸变矫正面板";
    hori_scrollable_=true;
    {

        auto tmptex = SRender::Resources::STextureLoader::CreateColor(0);
        
        uimgp.reset(tmptex);
        uimgp->FreeRawData();
    }
    std::string filepth=R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\中文测试\DJI_0314.JPG)";
    auto tmptex = SRender::Resources::STextureLoader::LoadFromFile(filepth);
    if (tmptex!=nullptr)
        uimgp.reset(tmptex);
}

void DistrotionRectifierPanel::DrawContent(){
    static float scale = 5;
    ImVec2 lupos(ImGui::GetCursorScreenPos());
	ImVec2 lupos1(lupos);
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
        ImVec2 uv0 = ImVec2((region_x) / my_tex_w, 1.0-(region_y) / my_tex_h);
        ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, 1.0-(region_y + region_sz) / my_tex_h);
        ImGui::Image((void*)(uint64_t)uimgp->id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
        ImGui::EndTooltip();
    }



    ImGui::PopStyleVar();
    static SRender::LowRenderer::RadialDistortion distortioninfo{{0,0,0},SRender::LowRenderer::DistortionModel::INDEX};
    static float norm_fh=1;
    static float sensor_scale=1;
    undistpipeline.Run(*uimgp, norm_fh,sensor_scale, distortioninfo);
    auto uimgw=undistpipeline.GetFbo().buf_size_.first;
    auto uimgh=undistpipeline.GetFbo().buf_size_.second;
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
    ImGui::Spacing();
    ImGui::Text("LINES:");
    static std::vector<std::vector<std::pair<float,float>>>lines;
    static int selected = -1;
    //ImGui::PushItemWidth(50);
    for (int n = 0; n < lines.size(); n++)
    {
        char buf[32];
        sprintf(buf, "Line %d", n);
        if (ImGui::Selectable(buf, selected == n,0,{100,0})){
            selected = n;
            
            
        }
        if (selected==n){
            ImGui::SameLine();
            if (ImGui::Button("delete")){
                lines.erase(lines.begin()+selected);
            }
        }
            
        
    }
    //ImGui::PopItemWidth();
    if (ImGui::Button("add line")){
        lines.push_back({});
    }
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
        if (0<=selected&&selected<lines.size()){
            float coord_u=cursor_onimg_x/my_tex_w;
            float coord_v=cursor_onimg_y/my_tex_h;
            lines[selected].push_back({coord_u,coord_v});
        }
    }

    ImGui::DragFloat("img scale",&scale,0.01);
    ImGui::DragFloat("sensor scale",&sensor_scale,0.01);
    if (ImGui::Button("打开文件")){
        
        auto filepth = Util::NfdDialog::OpenFileDlg();
        if (filepth!=""){
            auto tmptex = SRender::Resources::STextureLoader::LoadFromFile(filepth);
            if (tmptex!=nullptr){
                uimgp.reset(tmptex);
            }
            
        }
    }
}



}