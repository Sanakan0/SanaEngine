#include "SResourceManager/ShaderManager.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SResourceManager/Util.h"
namespace ResourceManager {

SRender::Resources::GLShader* ShaderManager::CreateResources(const std::string& pth){
    auto tmp = SRender::Resources::GLShaderLoader::LoadFromFile(Util::GetFullPath(pth));
    repo_.Append(pth, tmp);
    return tmp;
}

void ShaderManager::KillResource(const std::string &pth){
    if (auto tmpshaderp = repo_[pth];tmpshaderp!=nullptr){
        delete tmpshaderp;
        repo_.Remove(pth);
    }
}

void ShaderManager::ClearAll(){
    repo_.ClearAllResources();
}

void ShaderManager::ReloadResource(const std::string &pth){
    if (auto tmpshaderp = repo_[pth];tmpshaderp!=nullptr){
        SRender::Resources::GLShaderLoader::RecompileShader(*tmpshaderp, pth);
    }
    
}

}