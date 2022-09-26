#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SMesh.h"
#include "SRender/Settings/GLSet.h"
#include<SRender/Resources/GLShaderLoader.h>
#include <memory>
#include <assert.h>
#include <vector>
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

void GLShapeDrawer::DrawGrid(){
    gridshader_->Bind();
    renderer_.Draw(*panelmeshp_, Setting::SPrimitive::TRIANGLES);
    gridshader_->Unbind();
}

GLShapeDrawer::GLShapeDrawer(GLRenderer& renderer):renderer_(renderer){
    //std::vector <Vertex>& vs, std::vector <unsigned int> &vidx
    InitLineShader();
    InitGridShader();
}

GLShapeDrawer::~GLShapeDrawer(){
    
}

void GLShapeDrawer::InitLineShader(){
    Resources::Vertex tmp{
        {0,0,0},
        {0,0},
        {0,0,0}};
    std::vector<Resources::Vertex> tmpv{tmp,tmp};
    std::vector<unsigned int>tmpid{0,1};
    linemeshp_=std::make_unique<Resources::SMesh>(tmpv,tmpid);
    std::string vshader=R"(
#version 430 core


uniform vec3 start;
uniform vec3 end;
layout (std140,binding = 0) uniform EngineUBO{
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};
void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
    gl_Position = ubo_Projection * ubo_View * vec4(position, 1.0);
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

void GLShapeDrawer::InitGridShader(){
    std::vector<Resources::Vertex> tmpv;
    tmpv.push_back({
        {1,1,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {-1,1,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {-1,-1,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {1,-1,0},
        {0,0},
        {0,0,0}
    });
    std::vector<uint32_t> idx{0,1,2,0,2,3};
    panelmeshp_ = std::make_unique<Resources::SMesh>(tmpv,idx);

     std::string vshader=R"(
#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 

out VS_OUT{
    vec3 nearpoint;
    vec3 farpoint;
}vs_out;

layout (std140,binding = 0) uniform EngineUBO{
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

vec3 CalcWorldPos(vec3 pos){
    vec4 tmp = inverse(ubo_View)*inverse(ubo_Projection)*vec4(pos,1.0);
    return tmp.xyz/tmp.w;
}

void main(){
    vs_out.nearpoint = CalcWorldPos(vec3(pos.xy,0.0));
    vs_out.farpoint = CalcWorldPos(vec3(pos.xy,1.0));
    gl_Position = vec4(pos,1.0);
}

)";
    std::string fshader=R"(
#version 430 core
layout (std140,binding = 0) uniform EngineUBO{
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};
in VS_OUT{
    vec3 nearpoint;
    vec3 farpoint;
} fs_in;

out vec4 FRAGMENT_COLOR;

float CalcDepth(){
    float t=-fs_in.nearpoint.y/(fs_in.farpoint.y-fs_in.nearpoint.y);
    float k=(ubo_ViewPos.y-fs_in.farpoint.y)/(ubo_ViewPos.y-fs_in.nearpoint.y);
    return (((k+1)*t-1)/((k-1)*t+1)+1)/2;
}

float CalcDepth1(vec3 fragpos){
    vec4 ndcpos = ubo_Projection*ubo_View*vec4(fragpos,1.0);
    return ndcpos.z/ndcpos.w;
}

float CalcGrid(vec3 fragpos,float scale){
    vec2 coord = fragpos.xz/scale;
    vec2 grid = abs(fract(coord-0.5)-0.5)/fwidth(coord);
    float line = min(grid.x,grid.y);
    return 1.0 - min(line,1.0);

}

void main(){
    
    //float t = -ubo_ViewPos.y/(fs_in.farpoint.y-ubo_ViewPos.y);
    //vec3 fragpos = t*(fs_in.farpoint-ubo_ViewPos)+ubo_ViewPos;
    float t=-fs_in.nearpoint.y/(fs_in.farpoint.y-fs_in.nearpoint.y);
    vec3 fragpos = t*(fs_in.farpoint-fs_in.nearpoint)+fs_in.nearpoint;
    gl_FragDepth = CalcDepth1(fragpos);
    FRAGMENT_COLOR = vec4(0.2,0.2,0.2,CalcGrid(fragpos,8)*float(t>0));

}


)";
    gridshader_=std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromStr(vshader, fshader));
    assert(gridshader_!=nullptr&&"ShapeDrawer.gridshader_ create failed!");
   

}

}