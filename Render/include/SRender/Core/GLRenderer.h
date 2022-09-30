#pragma once
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SMesh.h"
#include <SRender/Settings/GLSet.h>
#include <cstddef>
#include <memory>
namespace SRender::Core{



class GLRenderer{
public:
    GLRenderer();
    ~GLRenderer();
    void Draw(Resources::SMesh& mesh,Setting::SPrimitive mode);
    void SetClearColor(float r,float g,float b,float a=1.0){glClearColor(r,g,b,a);};
    void ClearBuffer(bool colorbuf = 1,bool depthbuf = 1, bool stencilbuf = 1);
    void SetCullFace(Setting::SCullFace cullface){glCullFace(static_cast<GLenum>(cullface));};
    void SetViewPort(unsigned int x,unsigned int y,unsigned int w,unsigned int h){glViewport(x,y,w,h);};
    void SetRasterizationMode(Setting::SRasterization mode){ glPolygonMode(GL_FRONT_AND_BACK,static_cast<GLenum>(mode)); };
    void SetRasterizationLineWdith(float width){glLineWidth(width);};

    
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