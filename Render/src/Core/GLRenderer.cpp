#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SMesh.h"
#include "SRender/Settings/GLSet.h"
#include<SRender/Resources/GLShaderLoader.h>
#include <memory>
#include <assert.h>
namespace SRender::Core{

GLRenderer::GLRenderer(){};
GLRenderer::~GLRenderer(){};

void GLRenderer::Draw(Resources::SMesh& mesh,Setting::SPrimitive mode){
    mesh.Bind();
    if (mesh.vidx_.size()>0){
        //glDrawElements(GL_TRIANGLES,mesh.vidx_.size(),)
        glDrawElements(static_cast<GLenum>(mode),mesh.vidx_.size(),GL_UNSIGNED_INT,nullptr);
    }else{
        glDrawArrays(static_cast<GLenum>(mode),0,mesh.vs_.size());
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


}
namespace SRender::Core{

void GLShapeDrawer::SetViewPrj(const glm::mat4 &viewprj){
    lineshader_->Bind();
    lineshader_->SetUniMat4("viewprj", viewprj);
    lineshader_->Unbind();
}

void GLShapeDrawer::DrawLine(const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &color,float width){
    lineshader_->Bind();

    lineshader_->SetUniVec3("start", start);
    lineshader_->SetUniVec3("end", end);
    lineshader_->SetUniVec3("color", color);
    
    renderer_.SetRasterizationMode(Setting::SRasterization::LINE);
    renderer_.SetRasterizationLineWdith(width);
    renderer_.Draw(*linemeshp_, Setting::SPrimitive::LINES);
    renderer_.SetRasterizationLineWdith(1.0);
    renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    
    lineshader_->Unbind();
}

GLShapeDrawer::GLShapeDrawer(GLRenderer& renderer):renderer_(renderer){
    //std::vector <Vertex>& vs, std::vector <unsigned int> &vidx
    Resources::Vertex tmp{
        {0,0,0},
        {0,0,0},
        {0,0}};
    std::vector<Resources::Vertex> tmpv{tmp,tmp};
    std::vector<unsigned int>tmpid{0,1};
    linemeshp_=std::make_unique<Resources::SMesh>(tmpv,tmpid);
    std::string vshader=R"(
#version 430 core


uniform mat4 viewprj;
uniform vec3 start;
uniform vec3 end;

void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
    gl_Position = viewprj * vec4(position, 1.0);
}

)";
    std::string fshader=R"(
#version 430 core
uniform vec3 color;
out vec4 FragColor;
void main(){
    FragColor = vec4(color,1.0);
}

)";
    lineshader_=std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromStr(vshader, fshader));
    assert(lineshader_!=nullptr&&"ShapeDrawer.lineshader_ create failed!");
}



}