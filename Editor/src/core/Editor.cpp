#include "SEditor/Core/Editor.h"
#include <SCore/Global/ServiceLocator.h>
using namespace SEditor::Core;

Editor::Editor(RuntimeContext& rtcontext):rtcontext_(rtcontext){  
    SetUpUI();
}

Editor::~Editor(){
    
}

void Editor::Run(){
    while(!rtcontext_.wndcontext_->shouldclose()){
        float delta_time;
        {
            using namespace std::chrono;

            steady_clock::time_point tick_time = steady_clock::now();
            duration<float> time_span = duration_cast<duration<float>>(tick_time - last_tick_time_);
            delta_time                = time_span.count();

            last_tick_time_ = tick_time;
        }
        PreTick();
        Tick(delta_time);
        PostTick();
    }
}

void Editor::PreTick(){
    rtcontext_.wndcontext_->pollevents();
}

void Editor::Tick(float deltat){
    LogicTick(deltat);
    RenderTick(deltat);
}

void Editor::LogicTick(float deltat){
    static auto& sceneview = rtcontext_.uimanager_->GetPanel<SEditor::Panels::SceneView>("Scene View");
    sceneview.LogicTick(deltat);
}

void Editor::RenderTick(float deltat){
    static auto& sceneview = rtcontext_.uimanager_->GetPanel<SEditor::Panels::SceneView>("Scene View");
    sceneview.RenderTick();
    rtcontext_.uimanager_->RenderTick();
}

void Editor::PostTick(){
    rtcontext_.wndcontext_->swapbuffer();
}

void Editor::SetUpUI(){
     rtcontext_.uimanager_->CreatePanel<SEditor::Panels::SceneView>("Scene View");
}