#include "SRender/Buffers/GLUniformBuffer.h"

namespace SRender::Buffers{


GLUniformBuffer::GLUniformBuffer(size_t sz,int bindingpoint,GLenum usage){
    glGenBuffers(1,&ubo_id_);
    Bind();
    glBufferData(GL_UNIFORM_BUFFER,sz,NULL,usage);
    Unbind();
    glBindBufferBase(GL_UNIFORM_BUFFER,bindingpoint,ubo_id_);

}

GLUniformBuffer::~GLUniformBuffer(){};

void GLUniformBuffer::Bind(){
    glBindBuffer(GL_UNIFORM_BUFFER,ubo_id_);
}

void GLUniformBuffer::Unbind(){
    glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void GLUniformBuffer::BindShaderUniBlock(
    Resources::GLShader& shader,const std::string& blockname,int bindingpoint){
    glUniformBlockBinding(shader.id_,GetBlockLoc(blockname,shader),bindingpoint);
}



unsigned int GLUniformBuffer::GetBlockLoc(const std::string name,Resources::GLShader& shader){
    return glGetUniformBlockIndex(shader.GetID(),name.c_str());
}

unsigned int GLUniformBuffer::GetID() const {
    return ubo_id_;
}

}