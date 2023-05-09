#include "SRender/Buffers/GLFrameBuffer.h"

namespace SRender::Buffers{

void GLFrameBuffer::BindColor(uint32_t slot){
    glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex_buf_id_);
}

void GLFrameBuffer::BindDepth(uint32_t slot){
    glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, depth_buf_id_);
}

void GLFrameBuffer::UnbindTex(){
    glBindTexture(GL_TEXTURE_2D,0);
}

void GLFrameBuffer::Setup(int w,int h){
    glGenFramebuffers(1,&fbo_id_);
    glGenTextures(1,&tex_buf_id_);
    glGenRenderbuffers(1,&depth_stencil_buf_id_);
    
    	/* Setup texture */
	glBindTexture(GL_TEXTURE_2D, tex_buf_id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Setup framebuffer */
    glBindRenderbuffer(GL_RENDERBUFFER,depth_stencil_buf_id_);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_STENCIL,w,h);
    glBindRenderbuffer(GL_RENDERBUFFER,0);

    Bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_buf_id_, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buf_id_);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buf_id_);
    Unbind();
}


void GLFrameBuffer::SetupDepthonly(int w,int h){
    glGenFramebuffers(1,&fbo_id_);

    glGenTextures(1,&depth_buf_id_);
    
    	/* Setup texture */


	/* Setup framebuffer */

    glBindTexture(GL_TEXTURE_2D, depth_buf_id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32F,w,h,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);
	glBindTexture(GL_TEXTURE_2D, 0);

    Bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_buf_id_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    Unbind();
}

void GLFrameBuffer::DeleteBuf(){
    glDeleteFramebuffers(1, &fbo_id_);
    glDeleteTextures(1,&tex_buf_id_);
    glDeleteTextures(1,&depth_buf_id_);
    glDeleteRenderbuffers(1,&depth_stencil_buf_id_);
}

GLFrameBuffer::GLFrameBuffer(int w,int h,bool depth_only):
buf_size_(w,h),depth_only_(depth_only){
    buf_size_=std::make_pair(w,h);
    if (depth_only_) SetupDepthonly(w,h);
    else Setup(w, h);
}

GLFrameBuffer::~GLFrameBuffer(){
    DeleteBuf();
}


void GLFrameBuffer::Bind(){
    glBindFramebuffer(GL_FRAMEBUFFER,fbo_id_);
}

void GLFrameBuffer::Unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void GLFrameBuffer::Resize(int w,int h){
    if (buf_size_.first==w&&buf_size_.second==h){
        return;
    }
    buf_size_=std::make_pair(w,h);
    DeleteBuf();
    if (depth_only_) SetupDepthonly(w, h);
    else Setup(w, h);
}


}