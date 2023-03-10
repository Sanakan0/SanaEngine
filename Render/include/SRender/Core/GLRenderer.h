#pragma once
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include <SRender/Settings/GLSet.h>
#include <cstddef>
#include <memory>
#include <stdint.h>
namespace SRender::Core{

enum class SGLState:uint8_t{
    DEPTH_TEST = 0x01,
    CULL_FACE = 0x02,
    BLEND = 0X04,
    DEPTH_WRITING = 0X08,
    BACK_CULL = 0X20,
    FRONT_CULL = 0X40
};


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
    void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, GLenum format, GLenum type, void* data){
        glReadPixels(x, y, width, height, format, type, data);
    };
    void ApplyGLstate(uint8_t mask);
private:
    uint8_t glstate_;
};


class GLShapeDrawer{
public:
    GLShapeDrawer(GLRenderer& renderer);
    ~GLShapeDrawer();
    void DrawLine(const glm::vec3& start,const glm::vec3& end,const glm::vec3& color,float width=1.0f);
    void DrawGrid();
    void DrawArrow(const glm::mat4& model_mat);
private:
    void InitGridShader();
    void InitLineShader();
    void InitArrowShader();
    std::unique_ptr<Resources::SMesh> linemeshp_=nullptr;
    std::unique_ptr<Resources::SMesh> panelmeshp_=nullptr;
    std::unique_ptr<Resources::SMesh> arrowmeshp_=nullptr;
    std::unique_ptr<Resources::GLShader> lineshader_=nullptr;
    std::unique_ptr<Resources::GLShader> gridshader_=nullptr;
    std::unique_ptr<Resources::GLShader> arrowshader_=nullptr;
    GLRenderer& renderer_;
};

}