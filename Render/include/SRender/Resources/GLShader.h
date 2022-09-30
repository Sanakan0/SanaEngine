#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
namespace SRender::Resources{

class GLShader{
friend class GLShaderLoader;
public:
    GLShader(const std::string path,uint32_t programid);
    ~GLShader();
    GLShader(const GLShader&) = delete;
    void Bind();
    void Unbind();
    void SetUniInt(const std::string& name, int val);
    void SetUniFloat(const std::string& name, float val);
    void SetUniVec2(const std::string& name, glm::vec2 val);
    void SetUniVec3(const std::string& name, glm::vec3 val);
    void SetUniVec4(const std::string& name, glm::vec4 val);
    void SetUniMat4(const std::string& name, glm::mat4 val);
    int GetUniformLocation(const std::string& name);
    unsigned int GetID() const;
    unsigned int id_;
    const std::string path_;
};


}