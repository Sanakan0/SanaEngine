#pragma once
#include "glm/ext/quaternion_float.hpp"
#include "imgui/imgui.h"
namespace SGUI{

void ToggleButton(const char* str_id,const char* icon0,const char* icon1, bool* v);
void ToggleButton(const char* str_id,const char* icon0, bool* v);

void Campass(const glm::quat& main_cam_orien_,float sz=50.0f);

}