#include "SEditor/Core/Application.h"
#include <iostream>
#include <spdlog/spdlog.h>
namespace SEditor::Core{

Application::Application():editor_(rtcontext_){}

Application::~Application(){}

void Application::Run(){
    spdlog::info("[SANAEngine] Engine start!");
    editor_.Run();
}

}