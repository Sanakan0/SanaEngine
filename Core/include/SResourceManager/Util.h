#pragma once
#include <string>
namespace ResourceManager {
class Util{
public:
    void static SetEngineAssetPath(const std::string& pth){
        engine_asset_path_=pth;
    }
    std::string static GetFullPath(const std::string& pth){
        if (pth[0]==':'){
            return engine_asset_path_+std::string(pth.data()+1,pth.data()+pth.size());
        }else{
            return pth;
        }
    }
private:
    inline static std::string prj_asset_path_="";
    inline static std::string engine_asset_path_="";
};


}