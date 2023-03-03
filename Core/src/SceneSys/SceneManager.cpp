#include "SceneSys/Scene.h"
#include<SceneSys/SceneManager.h>
#include <memory>
namespace SceneSys{

SceneManager::SceneManager(){

}

SceneManager::~SceneManager(){

}

void SceneManager::CreateScene(){
    scenep_ = std::make_unique<Scene>();
}

void SceneManager::LoadScene(){

}



}