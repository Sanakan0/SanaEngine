#pragma once
#include<SGUI/Core/UImanager.h>
#include<SEditor/Panels/SceneView.h>
#include<SEditor/Core/RuntimeContext.h>
#include<memory>
#include<chrono>
namespace SEditor::Core{

class Editor{
public:
    Editor(RuntimeContext& rtcontext);
    ~Editor();
    void Run();
    void PreTick();
    // Tick time unit is second
    void Tick(float deltat);
    void PostTick();
    void LogicTick(float deltat);
    void RenderTick(float deltat);
    void SetUpUI();
    RuntimeContext& rtcontext_;
    std::chrono::steady_clock::time_point last_tick_time_;
};

}
