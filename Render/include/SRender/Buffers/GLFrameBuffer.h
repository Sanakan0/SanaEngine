#pragma once
#include "glad/glad.h"
#include <utility>
namespace SRender::Buffers{

class GLFrameBuffer{
public:
    GLFrameBuffer(int w=0,int h=0,bool readable_depth=0);
    ~GLFrameBuffer();
    void BindColor(uint32_t slot);
    void BindDepth(uint32_t slot);
    void UnbindTex();
    void Bind();
    void Unbind();
    void Resize(int w,int h);
    
    unsigned int fbo_id_=0;
    unsigned int tex_buf_id_=0;
    unsigned int depth_stencil_buf_id_=0;
    unsigned int depth_buf_id_=0;
    std::pair<int,int> buf_size_;
    const bool readable_depth_;
private:
    void Setup(int w,int h);
    void SetupDepthonly(int w,int h);
    void DeleteBuf();
};


}