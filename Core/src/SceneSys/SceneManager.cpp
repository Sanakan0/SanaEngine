#include "SCore/Global/ServiceLocator.h"
#include "SRender/Resources/Settings.h"
#include "SResourceManager/ModelManager.h"
#include "SResourceManager/TextureManager.h"
#include "SceneSys/Scene.h"
#include<SceneSys/SceneManager.h>
#include <fstream>
#include <memory>
#include <filesystem>
#include <sstream>
namespace SceneSys{

SceneManager::SceneManager(){

}

SceneManager::~SceneManager(){

}

void SceneManager::CreateScene(){
    scenep_ = std::make_unique<Scene>();
	scenep_->DeleteActorEvent.AddListener(std::bind(&SceneManager::OnDeleteActor,this,std::placeholders::_1));
}

void SceneManager::DestroyScene(){
    scenep_.reset(nullptr);

    SceneLoadSetting_ = SRender::Resources::ModelLoadSetting();
    selected_actor_id_=0;
    selected_actor_p_=nullptr;
    active_camera_id_=0;
    active_camera_p_=nullptr;
}

void SceneManager::LoadScene(std::string pth){
    if (pth=="") return;
	DestroyScene();
    CreateScene();
    tinyxml2::XMLDocument doc;
	std::ifstream in(std::filesystem::u8path(pth));
	std::stringstream ss ;
	ss << in.rdbuf();

	doc.Parse(ss.str().c_str()); //use ifstream to support utf8 path
//	doc.LoadFile(pth.c_str());
	
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();
		if (root)
		{
			tinyxml2::XMLNode* sceneNode = root->FirstChildElement("scene");
			if (sceneNode)
			{
	
				GetScene()->Deserialize(doc, sceneNode);
			
			}
		}
	}
	SANASERVICE(ResourceManager::ModelManager).LoadResourcesMultiThread();
	SANASERVICE(ResourceManager::TextureManager).UploadAll();
}



}