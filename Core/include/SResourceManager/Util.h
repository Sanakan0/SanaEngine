#pragma once
#include <string>
namespace ResourceManager {
class PathManager{
public:
    void static SetEngineAssetPath(const std::string& pth){
        engine_asset_path_=pth;
    }
    void static SetProjectPath(const std::string& pth){
        prj_path_=pth;
    }
    static const std::string& GetProjectPath(){
        return prj_path_;
    }
    std::string static GetFullPath(const std::string& pth){
        if (pth[0]==':'){
            return engine_asset_path_+std::string(pth.data()+1,pth.data()+pth.size());
        }else if(pth[0]=='@'){
            return prj_path_+std::string(pth.data()+1,pth.data()+pth.size());
        }else{
            return pth;
        }
    }
    
private:
    inline static std::string prj_path_="";
    inline static std::string engine_asset_path_="";
};


}