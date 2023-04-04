#pragma once
#include <SWnd/context/context.h>
#include <utility>
namespace SWnd::Input{
typedef int KeyState;
typedef int AKey;
typedef int MouseBtnState;
typedef int AMouseBtn;
class InputManager{
public:
    InputManager(GLFWwindow* wndp);
    KeyState GetGlfwKeyState(AKey key);
    MouseBtnState GetGlfwMouseState(AMouseBtn btn);
    std::pair<int,int> GetCursorPos();
    std::pair<int,int> GetCursorDelta();
    void Tick();
    void ResetMouseStates();
    GLFWwindow* wndp_;
    int mid_btn_=0;
    int left_btn_=0;
    int right_btn_=0;
    bool first_mouse_pressed_=false;
    std::pair<int,int> lastpos_,curpos_;
    std::pair<int,int> cursordelta_;
};

}