#include "glm/gtc/quaternion.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
namespace SGUI{

void ToggleButton(const char* str_id,const char* icon0,const char* icon1, bool* v)
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * 2.0f;
	float radius = height * 0.50f;
	float rounding = 0.2f;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	if (ImGui::IsItemClicked()) *v = !*v;

	if (ImGui::IsItemHovered())
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32( ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * rounding);
	else
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * rounding);
	ImVec2 center0 = ImVec2(radius,radius);
    ImVec2 center1 = ImVec2(width-radius,radius);

	ImVec2 center = *v ? center0:center1;
	draw_list->AddRectFilled(ImVec2((p.x + center.x) - radius, p.y + 1.5f),
		ImVec2((p.x  + center.x) + radius, p.y + height - 1.5f), IM_COL32(255, 255, 255, 255), height * rounding);
    
    float fontsz=height;
    auto icon_c0= IM_COL32(255,0,255,255);
    auto icon_c1 =IM_COL32(255,0,255,127);
    draw_list->AddText(nullptr,fontsz,ImVec2(p.x+center0.x-fontsz/3,p.y+center0.y-fontsz/2),*v?icon_c0:icon_c1,icon0);
    draw_list->AddText(nullptr,fontsz,ImVec2(p.x+center1.x-fontsz/3,p.y+center0.y-fontsz/2),*v?icon_c1:icon_c0,icon1);
}
void ToggleButton(const char* str_id,const char* icon0, bool* v)
{

	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height;
	float radius = height * 0.50f;
	float rounding = 0.2f;

	ImGui::InvisibleButton(str_id, ImVec2(width, height));
	if (ImGui::IsItemClicked()) *v = !*v;

	if (ImGui::IsItemHovered())
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32( ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * rounding);
	else
		draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * rounding);
	ImVec2 center = ImVec2(radius,radius);


	//ImVec2 center = *v ? center0:center1;
	draw_list->AddRectFilled(ImVec2((p.x + center.x) - radius, p.y + 1.5f),
		ImVec2((p.x  + center.x) + radius, p.y + height - 1.5f), IM_COL32(255, 255, 255, 255), height * rounding);
    
    float fontsz=height;
    auto icon_c0= IM_COL32(255,0,255,255);
    auto icon_c1 =IM_COL32(255,0,255,127);
    draw_list->AddText(nullptr,fontsz,ImVec2(p.x+center.x-fontsz/3,p.y+center.y-fontsz/2),*v?icon_c0:icon_c1,icon0);
}


void Campass(const glm::quat& main_cam_orien_,float sz){
	static const char axisChar[4]={'0','X','Y','Z'};
	static constexpr int axisC[4]={0,static_cast<int>(IM_COL32(130, 76, 76, 255)),
	static_cast<int>(IM_COL32(76, 130, 76, 255)),
	static_cast<int>(IM_COL32(76, 76, 130, 255))
	} ;
	
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	std::pair<int,glm::vec3> nodes[6];
	for (int i=1;i<=3;++i){
		auto tmp = glm::vec3(0);
		tmp[i-1]=sz;
		nodes[i-1]=std::make_pair(i, tmp*main_cam_orien_);
		tmp[i-1]=-sz;
		nodes[i+2]=std::make_pair(-i, tmp*main_cam_orien_);
	}

	std::sort(std::begin(nodes),std::end(nodes),[](std::pair<int,glm::vec3> a,std::pair<int,glm::vec3> b) -> bool{
		return a.second.z<b.second.z;
	});
	
	for (int i=0;i<6;++i){
		ImVec2 tmp{p.x+nodes[i].second.x,p.y-nodes[i].second.y};
		char buf[3];
		if (nodes[i].first>0){
			buf[0]=' ';
			buf[1]=axisChar[nodes[i].first];
			buf[2]='\0';
			draw_list->AddLine(p, tmp, axisC[nodes[i].first],3.0f);
		}else{
			buf[0]='-';
			buf[1]=axisChar[-nodes[i].first];
			buf[2]='\0';
		}
		draw_list->AddCircleFilled(tmp, 14,  axisC[std::abs(nodes[i].first)]);
		draw_list->AddText(nullptr,18.0f,{tmp.x-8,tmp.y-8},IM_COL32(255, 255, 255, 255), buf);
		
	}
	
}

}