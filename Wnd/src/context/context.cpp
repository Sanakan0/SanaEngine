
#include "SWnd/context/context.h"
#include <GLFW/glfw3.h>
#include <iostream>
namespace SWnd{

Context::Context(contextSETTING s):setting(s){
    //glfw init
    int initcode=glfwInit();
    if (initcode == GLFW_FALSE){
        std::cout << "glfwinit failed"<<std::endl;
    }else{
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED,true);
    }   

    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    Wnd_p = glfwCreateWindow(setting.W_WIDTH, setting.W_HEIGHT, "SanaEngine", NULL, NULL);

    if (Wnd_p == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(Wnd_p);
    if (s.unlock_fps_) glfwSwapInterval(0); // need to set after make_context

    auto tmp = glfwGetVideoMode(glfwGetPrimaryMonitor());
    moniter_info_.width = tmp->width;
    moniter_info_.height = tmp->height;
    moniter_info_.fps = tmp->refreshRate;
    moniter_info_.colorbits = tmp->redBits;
    auto x = sizeof(MoniterInfo);
}
Context::~Context(){
    glfwTerminate();
}

int Context::setup_GLAD(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
    }
    return 0;
}

void Context::Maximize(){
    glfwMaximizeWindow(Wnd_p);
}

void Context::Show(){
    glfwShowWindow(Wnd_p);
}

void Context::Hide(){
    glfwHideWindow(Wnd_p);
}

void Context::Focus(){
    glfwFocusWindow(Wnd_p);
}

void Context::swapbuffer(){glfwSwapBuffers(Wnd_p);}
void Context::pollevents(){glfwPollEvents();}
int Context::shouldclose(){return glfwWindowShouldClose(Wnd_p);}

}