#include "SResourceManager/ShaderManager.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SResourceManager/Util.h"
#include "spdlog/spdlog.h"
namespace ResourceManager {

SRender::Resources::GLShader* ShaderManager::CreateResources(const std::string& pth){
    auto tmp = SRender::Resources::GLShaderLoader::LoadFromFile(Util::GetFullPath(pth));
    if (tmp!=nullptr) {
        repo_.Append(pth, tmp);
        return tmp;
    }
    spdlog::error("[ShaderManager] Resource create failed : "+pth);
    return nullptr;
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