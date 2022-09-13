#include "SEditor/Core/Application.h"
#include <iostream>

namespace SEditor::Core{

Application::Application():editor_(rtcontext_){}

Application::~Application(){}

void Application::Run(){
    std::cout << "runconsole\n";
    editor_.Run();
}

}