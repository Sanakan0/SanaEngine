#include "SWnd/Input/InputManager.h"
#include <imgui/imgui.h>
namespace SWnd::Input{

InputManager::InputManager(GLFWwindow* wndp):wndp_(wndp){}

KeyState InputManager::GetGlfwKeyState(AKey key){
    return glfwGetKey(wndp_,key);
}
MouseBtnState InputManager::GetGlfwMouseState(AMouseBtn btn){
    return glfwGetMouseButton(wndp_,btn);
}

void InputManager::Tick(){
    mid_btn_ = GetGlfwMouseState(GLFW_MOUSE_BUTTON_MIDDLE);
    left_btn_ = GetGlfwMouseState(GLFW_MOUSE_BUTTON_LEFT);
    right_btn_ = GetGlfwMouseState(GLFW_MOUSE_BUTTON_RIGHT);
    lastpos_=curpos_;
    curpos_=GetCursorPos();
    if(mid_btn_||left_btn_||right_btn_){
        if(!first_mouse_pressed_){
            first_mouse_pressed_=true;
        }
    }
    if(!mid_btn_ && !left_btn_ && !right_btn_){ 
        first_mouse_pressed_=false;
    }
    if(right_btn_){
        //glfwSetInputMode(wndp_,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
        //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        
    }
    if(!right_btn_){
        //glfwSetInputMode(wndp_,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
        //ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
    
}

void InputManager::ResetMouseStates(){
    first_mouse_pressed_=false;
}

std::pair<int,int> InputManager::GetCursorPos(){
    double x, y;
	glfwGetCursorPos(wndp_, &x, &y);
    return{x,y};
}
std::pair<int,int> InputManager::GetCursorDelta(){
    auto& io=ImGui::GetIO();
    return {curpos_.first-lastpos_.first,curpos_.second-lastpos_.second};
    //return {io.MouseDelta.x,io.MouseDelta.y};
}
void InputManager::SetCursorPos(double x,double y){
    glfwSetCursorPos(wndp_, x,y);
    auto[dx,dy]=GetCursorDelta();
    curpos_={x,y};
    lastpos_={x-dx,y-dy};
}


}


