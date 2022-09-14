#include<SEditor/Core/Application.h>
#include <iterator>
#include<windows.h>
#include<iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
int main(void){
    Assimp::Importer imp;
    SEditor::Core::Application app;
    app.Run();
}
