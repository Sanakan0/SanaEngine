#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Settings/GLSet.h"
#include "glm/fwd.hpp"
#include<SRender/Resources/GLShaderLoader.h>
#include <memory>
#include <stdint.h>
#include <vector>
namespace SRender::Core{

GLRenderer::GLRenderer(){};
GLRenderer::~GLRenderer(){};

void GLRenderer::Draw(Resources::SMesh& mesh,Setting::SPrimitive mode){
    mesh.Bind();
    if (mesh.IdxSize()>0){
        //glDrawElements(GL_TRIANGLES,mesh.vidx_.size(),)
        glDrawElements(static_cast<GLenum>(mode),mesh.IdxSize(),GL_UNSIGNED_INT,nullptr);
    }else{
        glDrawArrays(static_cast<GLenum>(mode),0,mesh.VertexSize());
    }
    mesh.UnBind();
}
void GLRenderer::ClearBuffer(bool colorbuf,bool depthbuf, bool stencilbuf){
    glClear(
        (colorbuf?GL_COLOR_BUFFER_BIT:0)|
        (depthbuf?GL_DEPTH_BUFFER_BIT:0)|
        (stencilbuf?GL_STENCIL_BUFFER_BIT:0)
    );
}

void GLRenderer::SetGlxxable(GLenum capability,bool is_enable){
    is_enable?glEnable(capability):glDisable(capability);
}

void GLRenderer::SetColorWriting(bool is_enable){
    glColorMask(is_enable,is_enable,is_enable,is_enable);
}


void GLRenderer::ApplyGLstate(uint8_t mask){
    his_glstate_=glstate_;
    if (mask!=glstate_){
        if ( (mask & 0x01)!=(glstate_ & 0x01) ) SetGlxxable(GL_DEPTH_TEST, mask & 0x01);
        if ( (mask & 0x04)!=(glstate_ & 0x04) ) SetGlxxable(GL_BLEND, mask & 0x04);
        if ( (mask & 0x08)!=(glstate_ & 0x08) ) (mask & 0x08)?glDepthMask(0xFF):glDepthMask(0x00);
        if ( (mask & 0x10)!=(glstate_ & 0x10) ) SetColorWriting(mask & 0x10);
        if ( (mask & 0x02)!=(glstate_ & 0x02) ) {
            SetGlxxable(GL_CULL_FACE, mask & 0x02);
            if (mask & 0x02){
                uint8_t backBit = mask & 0x20;
			    uint8_t frontBit = mask & 0x40;
                if (backBit&&frontBit){
                    glCullFace(GL_FRONT_AND_BACK);
                }else{
                    backBit?glCullFace(GL_BACK):glCullFace(GL_FRONT);
                }
            }
        
        }
        glstate_=mask;
    }
    
    
}

void GLRenderer::ApplyPreviousGLstate(){
    ApplyGLstate(his_glstate_);
}



}
