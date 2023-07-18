#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/GLShader.h"
#include "SResourceManager/Util.h"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <vcruntime.h>
namespace SRender::Resources{

std::string GLShaderLoader::file_path_;

void GLShaderLoader::RecompileShader(GLShader &shader, const std::string &pth){
    file_path_=pth;
    auto[vshader,fshader] = ParseShader(ResourceManager::PathManager::GetFullPath(pth));
    uint32_t newprogramid = CreateProgram(vshader,fshader);
    if (newprogramid){
        glDeleteProgram(shader.id_);
        shader.id_=newprogramid;
        spdlog::info("[SHADER COMPILE SUCCESS] > "+file_path_+" ");
    }else{
        spdlog::error("[SHADER COMPILE FAILED] > "+file_path_+" ");
    }
}

void GLShaderLoader::KillShader(GLShader *&shaderp){
    delete shaderp;
    shaderp = nullptr;
}

GLShader* GLShaderLoader::LoadFromStr(const std::string &vshader, const std::string &fshader){
    file_path_="str_shader";
    uint32_t programid = CreateProgram(vshader,fshader);
    return programid?new GLShader("",programid):nullptr;
}

GLShader* GLShaderLoader::LoadFromFile(const std::string& pth){
    file_path_=pth;
    auto[vshader,fshader] = ParseShader(ResourceManager::PathManager::GetFullPath(pth));
    uint32_t programid = CreateProgram(vshader,fshader);
    
    return programid?new GLShader(pth,programid):nullptr;
}

uint32_t GLShaderLoader::CreateProgram(const std::string& v_shader,const std::string& f_shader){
    uint32_t programid = glCreateProgram();
    uint32_t vshaderid = CompileShader(GL_VERTEX_SHADER,v_shader);
    uint32_t fshaderid = CompileShader(GL_FRAGMENT_SHADER,f_shader);

    if (!vshaderid || !fshaderid) return 0;

    glAttachShader(programid,vshaderid);
    glAttachShader(programid,fshaderid);
    glLinkProgram(programid);

    int linkstatus;
    glGetProgramiv(programid,GL_LINK_STATUS,&linkstatus);
    if (!linkstatus){
        int len;
        glGetShaderiv(programid,GL_INFO_LOG_LENGTH,&len);
        std::string errorlog(len,' ');
        glGetProgramInfoLog(programid, len, &len, errorlog.data());  
        spdlog::error("[SHADER LINK ERROR] > " + file_path_ + "\n" + errorlog);
        glDeleteProgram(programid);
        return 0;
    }
    glValidateProgram(programid);
    glDeleteShader(vshaderid);
    glDeleteShader(fshaderid);
    spdlog::info("[SHADER COMPILE SUCCESS] > "+file_path_+" ");
    return programid;

}

uint32_t GLShaderLoader::CompileShader(GLenum type,const std::string& src){
    if (src==""){
        std::string typestr = type == GL_VERTEX_SHADER? "VERTEX shader":"FRAGMENT shader";
        spdlog::error("[SHADER COMPILE FAILED] > " + typestr + " > " + file_path_ + " file is empty or no such file \n");
        return 0;
    }
    uint32_t shaderid = glCreateShader(type);
    const char* src_code = src.c_str();
    glShaderSource(shaderid,1,&src_code,NULL);
    glCompileShader(shaderid);
    int comp_status;
    glGetShaderiv(shaderid,GL_COMPILE_STATUS,&comp_status);
    if (!comp_status){
        int len;
        glGetShaderiv(shaderid,GL_INFO_LOG_LENGTH,&len);
        std::string errorlog(len,' ');
        glGetShaderInfoLog(shaderid, len, &len, errorlog.data());
        std::string typestr = type == GL_VERTEX_SHADER? "VERTEX shader":"FRAGMENT shader";
        spdlog::error("[SHADER COMPILE FAILED] > " + typestr + " > " + file_path_ + "\n" + errorlog);
        glDeleteShader(shaderid);
        return 0;
    }
    return shaderid;
}

std::pair<std::string,std::string> GLShaderLoader::ParseShader(const std::string& pth){
    namespace fs = std::filesystem;
    std::ifstream input(std::filesystem::u8path( pth));
    enum class shaderT{VERT=0,FRAG=1,NONE=-1};
    shaderT type;
    std::string tmpline;
    std::stringstream ss[2];
    fs::path path(pth);
    while(std::getline(input,tmpline)){
        if (tmpline.find("#VERTEX")!=std::string::npos){
            type = shaderT::VERT;
        }else if (tmpline.find("#FRAGMENT")!=std::string::npos){
            type = shaderT::FRAG;
        }else if (tmpline.find("#include")!=std::string::npos){ // ADD relative include support
            size_t idx = tmpline.find_first_of(R"(")");
	        size_t idx2 = tmpline.find_last_of(R"(")");
	        std::string include_pth(&tmpline[idx+1],&tmpline[idx2]);
            fs::path include_full_pth = path.parent_path()/include_pth;
            std::ifstream tmpi(std::filesystem::u8path(include_full_pth.generic_string()));
            std::stringstream tmpbuf;
            tmpbuf << tmpi.rdbuf();
            ss[static_cast <int> (type)] << tmpbuf.str();
        }else if (type != shaderT::NONE){
            ss[static_cast <int> (type)] << tmpline << "\n";
        }
    }
    return{ss[0].str(),ss[1].str()};
}


}