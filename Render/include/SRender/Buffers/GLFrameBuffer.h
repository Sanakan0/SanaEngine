#pragma once
#include "glad/glad.h"
#include <stdint.h>
#include <utility>
#include <vector>
namespace SRender::Buffers{

class GLFrameBuffer{
public:
    GLFrameBuffer(int w=0,int h=0,bool depth_only=0,bool independent_depth32=0);
    ~GLFrameBuffer();
    void BindColor(uint32_t slot);
    void BindDepth(uint32_t slot);
    void UnbindTex();
    void Bind();
    void Unbind();
    void Resize(int w,int h);
    
    std::vector<uint8_t> DownloadColor();
    std::vector<float> DownloadDepth();

    unsigned int fbo_id_=0;
    unsigned int tex_buf_id_=0;
    unsigned int depth_stencil_buf_id_=0;
    unsigned int depth_buf_id_=0;
    unsigned int stencil_buf_id_=0;
    std::pair<int,int> buf_size_;
    const bool depth_only_;
    const bool independent_depth32_;
private:
    void Setup(int w,int h);
    void SetupDepth32(int w,int h);
    void SetupDepthonly(int w,int h);
    void DeleteBuf();
};


}