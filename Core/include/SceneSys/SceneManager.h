#pragma once

#include "SceneSys/Scene.h"
#include <memory>
namespace SceneSys{

class SceneManager{
public:
    SceneManager();
    ~SceneManager();
    void CreateScene();
    void LoadScene();
    Scene* GetScene(){return scenep_.get();}
private:
    std::unique_ptr<Scene> scenep_;
};

}