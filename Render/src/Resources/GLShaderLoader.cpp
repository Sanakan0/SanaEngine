#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/GLShader.h"
#include <fstream>
#include <sstream>
#include <iostream>
namespace SRender::Resources{

std::string GLShaderLoader::file_path_;

void GLShaderLoader::KillShader(GLShader *&shaderp){
    delete shaderp;
    shaderp = nullptr;
}

GLShader* GLShaderLoader::LoadFromStr(const std::string &vshader, const std::string &fshader){
    uint32_t programid = CreateProgram(vshader,fshader);
    return programid?new GLShader("",programid):nullptr;
}

GLShader* GLShaderLoader::LoadFromFile(const std::string& pth){
    file_path_=pth;
    auto[vshader,fshader] = ParseShader(pth);
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
        std::cout << "[shader link error] "  << " file:" << file_path_ << "\n" << errorlog << std::endl;  
        glDeleteProgram(programid);
        return 0;
    }
    glValidateProgram(programid);
    glDeleteShader(vshaderid);
    glDeleteShader(fshaderid);
    return programid;

}

uint32_t GLShaderLoader::CompileShader(GLenum type,const std::string& src){
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
        std::cout << "[shader error] " << typestr << " file:" << file_path_ << "\n" << errorlog << std::endl;  
        glDeleteShader(shaderid);
        return 0;
    }
    return shaderid;
}

std::pair<std::string,std::string> GLShaderLoader::ParseShader(const std::string& pth){
    std::ifstream input(pth);
    enum class shaderT{VERT=0,FRAG=1,NONE=-1};
    shaderT type;
    std::string tmpline;
    std::stringstream ss[2];
    while(std::getline(input,tmpline)){
        if (tmpline.find("#VERTEX")){
            type = shaderT::VERT;
        }else if (tmpline.find("#FRAGMENT")){
            type = shaderT::FRAG;
        }else if (type == shaderT::NONE){
            ss[static_cast <int> (type)] << tmpline << "\n";
        }
    }
    return{ss[0].str(),ss[1].str()};
}


}