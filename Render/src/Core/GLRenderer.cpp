#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Settings/GLSet.h"
#include<SRender/Resources/GLShaderLoader.h>
#include <memory>
#include <assert.h>
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

void GLRenderer::ApplyGLstate(uint8_t mask){
    his_glstate_=glstate_;
    if (mask!=glstate_){
        if ( (mask & 0x01)!=(glstate_ & 0x01) ) SetGlxxable(GL_DEPTH_TEST, mask & 0x01);
        if ( (mask & 0x04)!=(glstate_ & 0x04) ) SetGlxxable(GL_BLEND, mask & 0x04);
        if ( (mask & 0x08)!=(glstate_ & 0x08) ) (mask & 0x08)?glDepthMask(0xFF):glDepthMask(0x00);

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
namespace SRender::Core{



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
    renderer_.ApplyGLstate(grid_glstate_);
    gridshader_->Bind();
    renderer_.Draw(*panelmeshp_, Setting::SPrimitive::TRIANGLES);
    gridshader_->Unbind();
    renderer_.ApplyPreviousGLstate();
}

void GLShapeDrawer::DrawArrow(const glm::mat4 &model_mat){
    arrowshader_->Bind();
    renderer_.SetRasterizationMode(Setting::SRasterization::LINE);
    arrowshader_->SetUniMat4("ModelMat", model_mat);
    renderer_.Draw(*arrowmeshp_, Setting::SPrimitive::TRIANGLES);
    renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    arrowshader_->Unbind();
}

GLShapeDrawer::GLShapeDrawer(GLRenderer& renderer):renderer_(renderer){
    //std::vector <Vertex>& vs, std::vector <unsigned int> &vidx
    InitLineShader();
    //init grid drawer
    InitGridShader();
    grid_glstate_=SGLState::EMPTY|SGLState::BLEND|SGLState::DEPTH_TEST|SGLState::DEPTH_WRITING;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //
    InitArrowShader();
    
}

GLShapeDrawer::~GLShapeDrawer(){
    
}

void GLShapeDrawer::InitArrowShader(){
    std::vector<Resources::Vertex> tmpv;
    tmpv.push_back({
        {0,1,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {0,0,0.5},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {0.5,0,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {0,0,-0.5},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {-0.5,0,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {0,-0.2,0},
        {0,0},
        {0,0,0}
    });
    std::vector<uint32_t> idx{0,2,1,0,3,2,0,4,3,0,1,4,5,1,2,5,2,3,5,3,4,5,4,1};
    arrowmeshp_ = std::make_unique<Resources::SMesh>(tmpv,idx);

    arrowshader_=std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile("../assets/shaders/test.glsl"));
    assert(arrowshader_!=nullptr&&"ShapeDrawer.arrowshader create failed!");
   
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
    mat4 ubo_ViewMat;
    mat4 ubo_PrjMat;
    mat4 ubo_PrjViewMat;
    vec3 ubo_ViewPos;
};
void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
    gl_Position = ubo_PrjViewMat * vec4(position, 1.0);
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
    mat4 ubo_ViewMat;
    mat4 ubo_PrjMat;
    mat4 ubo_PrjViewMat;
    vec3 ubo_ViewPos;
};

vec3 CalcWorldPos(vec3 pos){
    vec4 tmp = inverse(ubo_PrjViewMat)*vec4(pos,1.0);
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
    mat4 ubo_ViewMat;
    mat4 ubo_PrjMat;
    mat4 ubo_PrjViewMat;
    vec3 ubo_ViewPos;
};
in VS_OUT{
    vec3 nearpoint;
    vec3 farpoint;
} fs_in;

out vec4 FRAGMENT_COLOR;

// float CalcDepth(){
//     float t=-fs_in.nearpoint.y/(fs_in.farpoint.y-fs_in.nearpoint.y);
//     float k=(ubo_ViewPos.y-fs_in.farpoint.y)/(ubo_ViewPos.y-fs_in.nearpoint.y);
//     return (((k+1)*t-1)/((k-1)*t+1)+1)/2;
// }

float CalcDepth(vec3 fragpos){
    vec4 ndcpos = ubo_PrjViewMat*vec4(fragpos,1.0);
    return (ndcpos.z/ndcpos.w+1)/2;
}

float CalcGrid(vec3 fragpos,float scale){
    vec2 coord = fragpos.xy/scale;
    vec2 grid = abs(fract(coord-0.5)-0.5)/fwidth(coord);
    float line = min(grid.x,grid.y);
    return 1.0 - min(line,1.0);
}

float MultiGrid(vec3 fragpos,float dis,float a,float b,float c){
    const float ga = CalcGrid(fragpos,a);
    const float gb = CalcGrid(fragpos,b);
    const float gc = CalcGrid(fragpos,c);
    
    const float d0 = 10;
    const float d1 = 40;
    const float d2 = 160;
    const float d3 = 640;

    const float a01 = clamp( (dis-d0)/(d1-d0),0.0,1.0 );
    const float a12 = clamp((dis-d1)/(d2-d1),0.0,1.0 );
    const float a23 = clamp((dis-d2)/(d3-d2),0.0,1.0 );
    return mix(mix(mix(ga,gb,a01),gc,a12),0,a23);
}

void main(){
    
    float t = -ubo_ViewPos.z/(fs_in.farpoint.z-ubo_ViewPos.z);
    vec3 fragpos = t*(fs_in.farpoint-ubo_ViewPos)+ubo_ViewPos;
    float dis = length(fragpos-ubo_ViewPos);
    float grid0 = MultiGrid(fragpos,dis,1,4,16);
    gl_FragDepth = CalcDepth(fragpos);

    float red = clamp(1-abs(fragpos.y)/fwidth(fragpos.y)/2.5,0,1);
    float green = clamp(1-abs(fragpos.x)/fwidth(fragpos.x)/2.5,0,1);
    FRAGMENT_COLOR = vec4(0.3+red*0.2,0.3+green*0.2,0.3,(grid0+mix(red+green,0,clamp((dis-160)/(480),0.0,1.0 )))*float(t>0));

}


)";
    gridshader_=std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromStr(vshader, fshader));
    assert(gridshader_!=nullptr&&"ShapeDrawer.gridshader_ create failed!");
   

}

}