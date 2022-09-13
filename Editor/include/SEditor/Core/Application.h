#pragma once
#include<SWnd/context/context.h>
#include<SGUI/Core/UImanager.h>
#include<SEditor/Core/RuntimeContext.h>
#include<SEditor/Core/Editor.h>
#include<memory>
#include<chrono>
namespace SEditor::Core{

class Application{

public:
Application();
~Application();
void Run();

private:
RuntimeContext rtcontext_;
Editor editor_;

};


}