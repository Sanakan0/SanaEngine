#include "ECS/Component/MeshComponent.h"
#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SShader.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include "SMath/Quaternion.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/matrix.hpp"
#include <SRender/Core/EntityRenderer.h>
#include <memory>
#include <stdint.h>

namespace SRender::Core{


EntityRenderer::EntityRenderer():
emptytexture_(Resources::STextureLoader::CreateColor((255<<24)|(255<<16)|(255<<8)|255)),
outline_colored_texture_(Resources::STextureLoader::CreateColor((255<<24)|(128<<16)|(57<<8)|237)),
unlitshader_(Resources::GLShaderLoader::LoadFromFile(ResourceManager::Util::GetFullPath(":shaders\\unlit.glsl"))){
    shapedrawer_ = std::make_unique<GLShapeDrawer>(*this);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glStencilFunc(GL_ALWAYS,1,0xFF);
};

EntityRenderer::~EntityRenderer(){}

void EntityRenderer::DrawSkeleton(Resources::SModel& model){
    ApplyGLstate(Skeleton_Default_GLstate);
    glm::mat4 scale=  glm::scale(glm::mat4(1),glm::vec3(0.1));
    for (int i=0;i<model.GetJoints().size();++i){
        shapedrawer_->DrawArrow(model.palette_[i]*glm::inverse(model.GetJoints()[i].inverse_bind_mat)*scale);
    }
    ApplyPreviousGLstate();
}

void EntityRenderer::DrawModel(Resources::SModel& model){
    auto& meshes = model.GetMeshes();
    auto material=model.GetMaterial();
    for (int i=0;i<meshes.size();++i){
        //bind texture
        material->BindTexturebyidx(meshes[i]->material_idx_,emptytexture_.get());
        //draw sth
        Draw(*meshes[i], Setting::SPrimitive::TRIANGLES);
        material->UnBindTexture();
    }
}

void EntityRenderer::DrawModelwithEmptyTex(Resources::SModel &model,Resources::STexture* empty_tex){
    auto& meshes = model.GetMeshes();
    if (empty_tex==nullptr)empty_tex=emptytexture_.get();
    for (int i=0;i<meshes.size();++i){
        //bind texture
        empty_mat_.BindTexturebyidx(meshes[i]->material_idx_,empty_tex);
        //draw sth
        Draw(*meshes[i], Setting::SPrimitive::TRIANGLES);
        empty_mat_.UnBindTexture();
    }
}

void EntityRenderer::DrawActorOutline(ECS::Actor& actor,float linewidth){
    auto meshcomp = static_cast<ECS::Components::MeshComponent*>(
        actor.GetComponent("MeshComponent"));
    
    if (!meshcomp) return;
    auto model = meshcomp->GetModel();
    
    auto transcomp = actor.GetTransformComponent();
    
    
    
    unlitshader_->Bind();

    if (transcomp) {
        unlitshader_->SetUniMat4("ModelMat", transcomp->GetMat());
    }else{
        unlitshader_->SetUniMat4("ModelMat", glm::mat4(1));
    }

    
    //draw to stencil
    ApplyGLstate(Stencil_GLstate);
    glStencilMask(0xFF);
    DrawModel(*model);
    glStencilMask(0x00);
    ApplyPreviousGLstate();
    //draw outline
    ApplyGLstate(Outline_GLstate);
    SetRasterizationMode(Setting::SRasterization::LINE);
    SetRasterizationLineWdith(linewidth);
    glStencilFunc(GL_NOTEQUAL,1,0XFF);
    
    
    DrawModelwithEmptyTex(*model, outline_colored_texture_.get());


    glStencilFunc(GL_ALWAYS,1,0xFF);
    SetRasterizationLineWdith(1.0);
    SetRasterizationMode(Setting::SRasterization::FILL);
    ApplyPreviousGLstate();
    unlitshader_->Unbind();
    
}



}

namespace SRender::Core{
GLShapeDrawer::GLShapeDrawer(EntityRenderer& renderer):renderer_(renderer){
    //std::vector <Vertex>& vs, std::vector <unsigned int> &vidx
    InitLineDrawer();
    //init grid drawer
    InitGridDrawer();
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //
    InitArrowDrawer();
    InitGizmoDrawer();
    InitCamFrameDrawer();
    
}

GLShapeDrawer::~GLShapeDrawer(){
    
}
void GLShapeDrawer::DrawCamFrame(const glm::mat4 &model_mat, float fovyratio, float aspect,glm::vec4 diff_color){
    renderer_.ApplyGLstate(LineMesh_GLstate);
    gizmoshader_->Bind();
    renderer_.SetRasterizationMode(Setting::SRasterization::LINE);
    gizmoshader_->SetUniMat4("ModelMat", glm::scale(model_mat,{aspect,1,fovyratio}));
    gizmoshader_->SetUniVec4("diffuse_color", diff_color);
    renderer_.Draw(*camframemeshp_, Setting::SPrimitive::TRIANGLE_FAN);
    renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    gizmoshader_->Unbind();
    renderer_.ApplyPreviousGLstate();
}

void GLShapeDrawer::DrawTransGizmo(const glm::vec3 &pos,const glm::mat4& viewmat){
    
    renderer_.ClearBuffer(0,1,0);

    float scale = -(viewmat*glm::vec4(pos,1)).z*0.05;
    auto ztrans = glm::mat4(scale);
    ztrans[3]=glm::vec4(pos,1);
    auto ytrans = gizmoarrow_ytrans*scale;
    ytrans[3]=glm::vec4(pos,1);
    auto xtrans = gizmoarrow_xtrans*scale;
    xtrans[3]=glm::vec4(pos,1);
    DrawGizmoArrow(ztrans,{0,0,1,1});
    DrawGizmoArrow(xtrans,{1,0,0,1});
    DrawGizmoArrow(ytrans,{0,1,0,1});
}
void GLShapeDrawer::DrawGizmoArrow(const glm::mat4& model_mat,const glm::vec4& diff_color){
    gizmoshader_->Bind();
    //renderer_.SetRasterizationMode(Setting::SRasterization::LINE);
    gizmoshader_->SetUniMat4("ModelMat", model_mat);
    gizmoshader_->SetUniVec4("diffuse_color", diff_color);
    renderer_.Draw(*gizmoarrowmeshp_head_, Setting::SPrimitive::TRIANGLE_FAN);
    renderer_.Draw(*gizmoarrowmeshp_headbase_, Setting::SPrimitive::TRIANGLE_FAN);
    renderer_.Draw(*gizmoarrowmeshp_body_, Setting::SPrimitive::TRIANGLE_STRIP);
    renderer_.Draw(*gizmoarrowmeshp_bodybase_, Setting::SPrimitive::TRIANGLE_FAN);
    //renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    gizmoshader_->Unbind();
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
    renderer_.ApplyGLstate(Grid_GLstate_);
    gridshader_->Bind();
    renderer_.Draw(*panelmeshp_, Setting::SPrimitive::TRIANGLES);
    gridshader_->Unbind();
    renderer_.ApplyPreviousGLstate();
}

void GLShapeDrawer::DrawArrow(const glm::mat4 &model_mat){
    renderer_.ApplyGLstate(LineMesh_GLstate);
    arrowshader_->Bind();
    renderer_.SetRasterizationMode(Setting::SRasterization::LINE);
    arrowshader_->SetUniMat4("ModelMat", model_mat);
    renderer_.Draw(*arrowmeshp_, Setting::SPrimitive::TRIANGLES);
    renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    arrowshader_->Unbind();
    renderer_.ApplyPreviousGLstate();
}



void GLShapeDrawer::InitCamFrameDrawer(){
    std::vector<Resources::Vertex> camv;
    camv.push_back({
        {0,0,0},
        {0,0},
        {0,0,0}
    });
    float tmpx[5]={1,1,-1,-1,1};
    float tmpy[5]={-1,1,1,-1,-1};
    for (int i=0;i<5;++i){
        camv.push_back({
            {tmpx[i],tmpy[i],-1},
            {0,0},
            {0,0,0}
        });
    }
    std::vector<uint32_t> idx;
    camframemeshp_ = std::make_unique<Resources::SMesh>(camv,idx);
}

void GLShapeDrawer::InitGizmoDrawer(){
    

    int cnt=10;
    float rbody=0.02f;
    float hbody=2.0f;
    std::vector<Resources::Vertex> bodyv;
    std::vector<Resources::Vertex> bodyv0;
    bodyv0.push_back({
        {0,0,0},
        {0,0},
        {0,0,0}
    });

    for (int i=0;i<=cnt;++i){
        float theta = 2.0f*i*SM_PI/cnt;
        float x=cosf(-theta)*rbody;
        float y=sinf(-theta)*rbody;
        bodyv.push_back({
            {x,y,0},
            {0,0},
            {0,0,0}
        });
        bodyv.push_back({
            {x,y,hbody},
            {0,0},
            {0,0,0}
        });
        bodyv0.push_back({
            {cosf(theta)*rbody,sinf(theta)*rbody,0},
            {0,0},
            {0,0,0}
        });
    }

   
    
    float rhead=0.1f;
    float hhead=0.5f;
    std::vector<Resources::Vertex> headv;
    headv.push_back({
        {0,0,hbody+hhead},
        {0,0},
        {0,0,0}
    });
    std::vector<Resources::Vertex> headv0;
    headv0.push_back({
        {0,0,hbody},
        {0,0},
        {0,0,0}
    });
    for (int i=0;i<=cnt;++i){
        float theta = 2.0f*i*SM_PI/cnt;
        headv.push_back({
            {cosf(theta)*rhead,sinf(theta)*rhead,hbody},
            {0,0},
            {0,0,0}
        });
        headv0.push_back({
            {cosf(-theta)*rhead,sinf(-theta)*rhead,hbody},
            {0,0},
            {0,0,0}
        });
    }
    
  
    std::vector<uint32_t> idx;
    gizmoarrowmeshp_body_ = std::make_unique<Resources::SMesh>(bodyv,idx);
    gizmoarrowmeshp_bodybase_ = std::make_unique<Resources::SMesh>(bodyv0,idx);
    gizmoarrowmeshp_head_ = std::make_unique<Resources::SMesh>(headv,idx);
    gizmoarrowmeshp_headbase_ = std::make_unique<Resources::SMesh>(headv0,idx);

    gizmoshader_=std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile("../assets/shaders/gizmo.glsl"));
    assert(gizmoshader_!=nullptr&&"ShapeDrawer.gizmoshader_ create failed!");

    auto cos45 = cos(SM_PI/4);
    auto sin45 =sin(SM_PI/4);
    gizmoarrow_ytrans = glm::mat4_cast(glm::quat(cos45,sin45,0,0));
    gizmoarrow_xtrans = glm::mat4_cast(glm::quat(cos45,0,sin45,0)); 
   
}

void GLShapeDrawer::InitArrowDrawer(){
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

void GLShapeDrawer::InitLineDrawer(){
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

void GLShapeDrawer::InitGridDrawer(){
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