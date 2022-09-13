#pragma once
#include "SRender/Resources/GLMesh.h"
#include "glad/glad.h"
namespace SRender::Core{




class GLRenderer{
public:
    GLRenderer();
    ~GLRenderer();
    void Draw(Resources::GLMesh& mesh,GLenum mode);
    void SetClearColor(float r,float g,float b,float a=1.0);
    void ClearBuffer(bool colorbuf = 1,bool depthbuf = 1, bool stencilbuf = 1);
    void SetCullFace();
    void SetViewPort(unsigned int x,unsigned int y,unsigned int w,unsigned int h);
    

    
};


}