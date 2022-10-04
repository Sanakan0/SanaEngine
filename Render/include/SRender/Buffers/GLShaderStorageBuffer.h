#pragma once
#include <glad/glad.h>
#include <SRender/Resources/GLShader.h>
#include <stdint.h>
namespace SRender::Buffers{

class GLShaderStorageBuffer{
public:
    GLShaderStorageBuffer();
    ~GLShaderStorageBuffer();
    void Bind(uint32_t bindingpoint);
    void Unbind();
    template<typename T>
	void SendBlocks(T* datap, size_t size){
        glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo_id_);
        glBufferData(GL_SHADER_STORAGE_BUFFER,size,datap,GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
    }
private:
    uint32_t ssbo_id_;
	uint32_t bindingpoint_ = 0;
};


}