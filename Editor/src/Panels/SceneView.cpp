#include "SEditor/Panels/SceneView.h"
#include <iostream>

namespace SEditor::Panels{

SceneView::SceneView(){
    name_="Scene View";
}

SceneView::~SceneView(){

}

void SceneView::LogicTick(float deltat){
    SView::LogicTick(deltat);
}

void SceneView::RenderImpl(){
    fbo_.Bind();
    
    glClearColor(0.2,0.4,0.2,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    fbo_.Unbind();

}


}
