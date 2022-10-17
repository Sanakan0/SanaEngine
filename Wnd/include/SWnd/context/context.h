#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
namespace SWnd{

struct contextSETTING{
    int W_WIDTH, W_HEIGHT;
    bool unlock_fps_;
};
class Context{
public:
    Context(contextSETTING s);
    ~Context();
    int setup_GLAD();
    void swapbuffer();
    void pollevents();
    int shouldclose();
    void Maximize();
    void Show();
    void Hide();
    void Focus();
    int W_WIDTH, W_HEIGHT;
    GLFWwindow* Wnd_p;
    contextSETTING setting;
};


}
