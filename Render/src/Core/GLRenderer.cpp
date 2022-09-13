#include "SRender/Core/GLRenderer.h"

namespace SRender::Core{

GLRenderer::GLRenderer(){};
GLRenderer::~GLRenderer(){};

void GLRenderer::Draw(Resources::GLMesh& mesh,GLenum mode){
    mesh.Bind();
    if (mesh.vidx_.size()>0){
        //glDrawElements(GL_TRIANGLES,mesh.vidx_.size(),)
        glDrawElements(mode,mesh.vidx_.size(),GL_UNSIGNED_INT,nullptr);
    }else{
        glDrawArrays(mode,0,mesh.vs_.size());
    }
    mesh.UnBind();
}
void GLRenderer::SetViewPort(unsigned int x,unsigned int y,unsigned int w,unsigned int h){
    glViewport(x,y,w,h);
}


}