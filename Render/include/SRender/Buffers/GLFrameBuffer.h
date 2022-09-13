#pragma once
#include "glad/glad.h"
#include <utility>
namespace SRender::Buffers{

class GLFrameBuffer{
public:
    GLFrameBuffer(int w=0,int h=0);
    ~GLFrameBuffer();
    void Bind();
    void Unbind();
    void Resize(int w,int h);

    unsigned int fbo_id_;
    unsigned int tex_buf_id_;
    unsigned int depth_stencil_buf_id_;
    std::pair<int,int> buf_size_;
private:
    void Setup(int w,int h);
    void DeleteBuf();
};


}