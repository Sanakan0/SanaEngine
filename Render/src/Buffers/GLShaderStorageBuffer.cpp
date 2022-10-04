#include <SRender/Buffers/GLShaderStorageBuffer.h>

namespace SRender::Buffers{

GLShaderStorageBuffer::GLShaderStorageBuffer(){
    glGenBuffers(1,&ssbo_id_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo_id_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_STREAM_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_id_);
}
GLShaderStorageBuffer::~GLShaderStorageBuffer(){
    glDeleteBuffers(1,&ssbo_id_);
}
void GLShaderStorageBuffer::Bind(uint32_t bindingpoint){
    bindingpoint_=bindingpoint;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,bindingpoint_,ssbo_id_);
}
void GLShaderStorageBuffer::Unbind(){
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER,bindingpoint_,0);
}


}