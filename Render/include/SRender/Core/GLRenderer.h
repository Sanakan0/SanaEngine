#pragma once
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "glm/fwd.hpp"
#include <SRender/Settings/GLSet.h>
#include <cstddef>
#include <memory>
#include <stdint.h>
namespace SRender::Core{

enum SGLState:uint8_t{
    EMPTY = 0X00,
    DEPTH_TEST = 0x01,
    CULL_FACE = 0x02,
    BLEND = 0X04,
    DEPTH_WRITING = 0X08,
    COLOR_WRITING = 0X10,
    BACK_CULL = 0X20,
    FRONT_CULL = 0X40
};
inline constexpr uint8_t Default_GLstate=
        SGLState::EMPTY|SGLState::DEPTH_TEST|SGLState::DEPTH_WRITING|SGLState::COLOR_WRITING|SGLState::CULL_FACE|SGLState::BACK_CULL;

inline constexpr uint8_t Skeleton_Default_GLstate=
        SGLState::EMPTY|SGLState::COLOR_WRITING;

inline constexpr uint8_t Stencil_GLstate=
        SGLState::EMPTY|SGLState::CULL_FACE|SGLState::BACK_CULL;

inline constexpr uint8_t Outline_GLstate=
        SGLState::EMPTY|SGLState::CULL_FACE|SGLState::BACK_CULL|SGLState::COLOR_WRITING;

inline constexpr uint8_t Grid_GLstate_=
        SGLState::EMPTY|SGLState::BLEND|SGLState::DEPTH_TEST|SGLState::COLOR_WRITING;//|SGLState::DEPTH_WRITING;


class GLRenderer{
public:
    GLRenderer();
    ~GLRenderer();
    void Draw(Resources::SMesh& mesh,Setting::SPrimitive mode);
    // e.g glenable(GL_DEPTH_TEST) or disable
    void SetGlxxable(GLenum capability,bool is_enable);
    void SetClearColor(float r,float g,float b,float a=1.0){glClearColor(r,g,b,a);};
    void ClearBuffer(bool colorbuf = 1,bool depthbuf = 1, bool stencilbuf = 1);
    void SetCullFace(Setting::SCullFace cullface){glCullFace(static_cast<GLenum>(cullface));};
    void SetViewPort(unsigned int x,unsigned int y,unsigned int w,unsigned int h){glViewport(x,y,w,h);};
    void SetRasterizationMode(Setting::SRasterization mode){ glPolygonMode(GL_FRONT_AND_BACK,static_cast<GLenum>(mode)); };
    void SetRasterizationLineWdith(float width){glLineWidth(width);};
    void SetColorWriting(bool is_enable);
    void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, GLenum format, GLenum type, void* data){
        glReadPixels(x, y, width, height, format, type, data);
    };
    void ApplyGLstate(uint8_t mask);
    void ApplyPreviousGLstate();

    
private:
    uint8_t glstate_;
    uint8_t his_glstate_;
    
};



}