#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SRender/Resources/GLShader.h>
namespace SRender::Resources{
class GLShaderLoader{
public:
    GLShaderLoader() = delete;
    static GLShader* LoadFromFile(const std::string& pth);
    static GLShader* LoadFromStr(const std::string& vshader,const std::string& fshader);
    static void KillShader(GLShader*& shaderp);
private:
    static std::pair<std::string,std::string> ParseShader(const std::string& pth);
    static uint32_t CreateProgram(const std::string& v_shader,const std::string& f_shader);
    static uint32_t CompileShader(GLenum type,const std::string& src);
    //static int CompileShader
    static std::string file_path_;
};



}