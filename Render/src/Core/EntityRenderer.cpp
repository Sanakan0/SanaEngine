#include "Debug/Assertion.h"
#include "ECS/Actor.h"
#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/MeshComponent.h"
#include "SRender/Core/GLRenderer.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SRender/Resources/SShader.h"
#include "SRender/Resources/STexture.h"
#include "SRender/Resources/STextureLoader.h"
#include "SResourceManager/Util.h"
#include "SMath/Quaternion.h"
#include "SceneSys/Scene.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/matrix.hpp"
#include "imgui/imgui.h"
#include <SRender/Core/EntityRenderer.h>
#include <cstddef>
#include <memory>
#include <stdint.h>

namespace SRender::Core{


EntityRenderer::EntityRenderer():
emptytexture_(Resources::STextureLoader::CreateColor((255<<24)|(255<<16)|(255<<8)|255)),
outline_colored_texture_(Resources::STextureLoader::CreateColor((255<<24)|(128<<16)|(57<<8)|237)),
unlitshader_(Resources::GLShaderLoader::LoadFromFile(ResourceManager::PathManager::GetFullPath(":shaders\\unlit.glsl"))){
    shapedrawer_ = std::make_unique<GLShapeDrawer>(*this);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glStencilFunc(GL_ALWAYS,1,0xFF);

    InitAtmosphereDrawer();
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
    
    if (!meshcomp) {
        auto camcomp = static_cast<ECS::Components::CameraComponent*>(
            actor.GetComponent("CameraComponent"));
        if (camcomp){
            auto transcomp = actor.GetTransformComponent();
            SANA_ASSERT(transcomp!=nullptr&&"camera actor must have a transform component");
            shapedrawer_->DrawCamFrame(transcomp->GetMat(), camcomp->cam_.Getfocal_length()/camcomp->cam_.Getsensor_size_h(), camcomp->cam_.Getaspect_ratio(), {1,0.2,0,1},false,3.0f,false);
        }
    }else{
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


float earthR = 6371000;
float atmosD = 8000;
float aerosolD = 1000;
float atmosR = earthR + atmosD;
float aerosolR = earthR + aerosolD;
float betaR[] = {
	5.8045429962610875e-06,
	1.3562911419845631e-05,
	3.31125767086075e-05
};
float betaM[] = {
	3.2518262207319404e-05,
	4.891366975006311e-05,
	7.507490794050062e-05
};


void EntityRenderer::DrawAtmosphere(ECS::Actor* sunLight) {
    if (sunLight==nullptr) return;
    ApplyGLstate(Atmosphere_GLstate);
    atmosphere_shader_->Bind();

	// glm::mat4 invmvp = glm::inverse(ModelViewProjectionMatrix);
	// glUniformMatrix4fv(glGetUniformLocation(skylightShader, "invmvp"), 1, GL_FALSE, &invmvp[0][0]);
	atmosphere_shader_->SetUniFloat("earthR", earthR);
    atmosphere_shader_->SetUniFloat("atmosR", atmosR);
    atmosphere_shader_->SetUniFloat("aerosolR", aerosolR);
    atmosphere_shader_->SetUniFloatV("betaR", betaR, 3);
    atmosphere_shader_->SetUniFloatV("betaM", betaM, 3);

    auto lightTrans = sunLight->GetTransformComponent();
	glm::vec3 sundir = -(lightTrans->trans_.GetOrienW()*sm::OglCamPrimForward);

    atmosphere_shader_->SetUniVec3("sunDir", sundir);
    //atmosphere_shader_->SetUniFloatV("sunDir",  &sundir[0], 3);
    //atmosphere_shader_->GetUniforms();
    Draw(*canvas_, Setting::SPrimitive::TRIANGLES);

    atmosphere_shader_->Unbind();
    ApplyPreviousGLstate();

}

void EntityRenderer::InitAtmosphereDrawer(){
    std::vector<Resources::Vertex> tmpv;
     tmpv.push_back({
        {-1,-1,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {3,-1,0},
        {0,0},
        {0,0,0}
    });
    tmpv.push_back({
        {-1,3,0},
        {0,0},
        {0,0,0}
    });
   
    std::vector<uint32_t> idx{0,1,2};
    canvas_ = std::make_unique<Resources::SMesh>(tmpv,idx);
    atmosphere_shader_=std::unique_ptr<Resources::GLShader> (Resources::GLShaderLoader::LoadFromFile(":shaders\\Atmosphere.glsl"));
    //SANA_ASSERT(arrowshader_!=nullptr&&"Atmosphere.glsl create failed!");
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
    InitCircleDrawer();
}

GLShapeDrawer::~GLShapeDrawer(){
    
}


void GLShapeDrawer::DrawCircle(const glm::mat4& model_mat,const glm::vec4& diff_color,float width){
    renderer_.ApplyGLstate(LineMesh_GLstate);
    gizmoshader_->Bind();
    //renderer_.SetRasterizationMode(Setting::SRasterization::LINE);
    renderer_.SetRasterizationLineWdith(width);
    gizmoshader_->SetUniMat4("ModelMat", model_mat);
    gizmoshader_->SetUniVec4("diffuse_color", diff_color);
    renderer_.Draw(*circlemeshp_, Setting::SPrimitive::LINE_LOOP);
    renderer_.SetRasterizationLineWdith(1.0);
    //renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    gizmoshader_->Unbind();
    renderer_.ApplyPreviousGLstate();
}

void GLShapeDrawer::DrawPoint(const glm::vec3& pos,const glm::vec4& color){
    renderer_.ApplyGLstate(LineMesh_GLstate);
    glPointSize(4.0f);
    gizmoshader_->Bind();
    gizmoshader_->SetUniMat4("ModelMat", glm::translate(glm::mat4(1),pos));
    gizmoshader_->SetUniVec4("diffuse_color", color);
    renderer_.Draw(*singlepointp_, Setting::SPrimitive::POINTS);
    gizmoshader_->Unbind();
    glPointSize(1.0f);
    renderer_.ApplyPreviousGLstate();
}

void GLShapeDrawer::DrawCursor(const glm::vec3 &pos, const glm::vec4& diff_color,const glm::mat4& viewmat,float width){
    float scale = -(viewmat*glm::vec4(pos,1)).z*0.025;
    auto mz = glm::mat4(scale);
    mz[3] = glm::vec4(pos,1);
    auto my = gizmoarrow_ytrans*scale;
    my[3] = glm::vec4(pos,1);
    auto mx = gizmoarrow_xtrans*scale;
    mx[3] = glm::vec4(pos,1);
    DrawCircle(mx, diff_color);
    DrawCircle(my, diff_color);
    DrawCircle(mz, diff_color);
    DrawPoint(pos, diff_color);
}

void GLShapeDrawer::DrawCamFrame(const glm::mat4 &model_mat, float fovyratio, float aspect,const glm::vec4& diff_color,bool is_activate,float width,bool enable_depth_test){
    renderer_.ApplyGLstate(LineMesh_GLstate|(enable_depth_test?SGLState::DEPTH_TEST:SGLState::EMPTY));
    gizmoshader_->Bind();
    renderer_.SetRasterizationMode(Setting::SRasterization::LINE);
    renderer_.SetRasterizationLineWdith(width);
    gizmoshader_->SetUniMat4("ModelMat", glm::scale(model_mat,{aspect,1,fovyratio}));
    gizmoshader_->SetUniVec4("diffuse_color", diff_color);
    renderer_.Draw(*camframemeshp_, Setting::SPrimitive::TRIANGLE_FAN);
    
    if (is_activate) renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    renderer_.Draw(*camframemesh_upp_, Setting::SPrimitive::TRIANGLES);
    renderer_.SetRasterizationLineWdith(1.0);
    renderer_.SetRasterizationMode(Setting::SRasterization::FILL);
    gizmoshader_->Unbind();
    renderer_.ApplyPreviousGLstate();
}

void GLShapeDrawer::DrawTransGizmo(const glm::vec3 &pos,const glm::mat4& viewmat,bool is_render_for_pick){
    
    renderer_.ClearBuffer(0,1,0);
    glm::vec4 xcolor{222.0/255,100.0/255,100.0/255,1};
    glm::vec4 ycolor{100.0/255,222.0/255,100.0/255,1};
    glm::vec4 zcolor{100.0/255,100.0/255,222.0/255,1};
    auto scale = glm::vec3(-0.1,-0.1,-0.08);
    
    if (is_render_for_pick){
        uint32_t gizmoid = SceneSys::SceneSetting::Actor_ID_Max+1;
        auto bytep = reinterpret_cast<uint8_t*>(&gizmoid);
        xcolor = glm::vec4(bytep[0]/255.0f,bytep[1]/255.0f,bytep[2]/255.0f,1.0f);
        ++gizmoid;
        ycolor = glm::vec4(bytep[0]/255.0f,bytep[1]/255.0f,bytep[2]/255.0f,1.0f);
        ++gizmoid;
        zcolor = glm::vec4(bytep[0]/255.0f,bytep[1]/255.0f,bytep[2]/255.0f,1.0f);
        scale.x*=10;
        scale.y*=10;
    }

    scale*=(viewmat*glm::vec4(pos,1)).z;
    auto scalemat = glm::scale(glm::mat4(1),scale);
    auto ztrans = scalemat;
    ztrans[3]=glm::vec4(pos,1);
    auto ytrans = gizmoarrow_ytrans*scalemat;
    ytrans[3]=glm::vec4(pos,1);
    auto xtrans = gizmoarrow_xtrans*scalemat;
    xtrans[3]=glm::vec4(pos,1);
    DrawGizmoArrow(ztrans,zcolor);
    DrawGizmoArrow(xtrans,xcolor);
    DrawGizmoArrow(ytrans,ycolor);
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
            {tmpx[i]*0.5f,tmpy[i]*0.5f,-1},
            {0,0},
            {0,0,0}
        });
    }

    std::vector<Resources::Vertex> camup;
    float trih=0.6f;
    camup.push_back({
        {0,trih+0.5f,-1},
        {0,0},
        {0,0,0}
    });
    camup.push_back({
        {0.5,trih,-1},
        {0,0},
        {0,0,0}
    });
    camup.push_back({
        {-0.5,trih,-1},
        {0,0},
        {0,0,0}
    });
    std::vector<uint32_t> idx;
    camframemesh_upp_ = std::make_unique<Resources::SMesh>(camup,idx);
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

   
    
    float rhead=0.04f;
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
    SANA_ASSERT(gizmoshader_!=nullptr&&"ShapeDrawer.gizmoshader_ create failed!");

    auto cos45 = cos(SM_PI/4);
    auto sin45 = sin(SM_PI/4);
    gizmoarrow_ytrans = glm::mat4_cast(glm::quat(-cos45,sin45,0,0));
    gizmoarrow_xtrans = glm::mat4_cast(glm::quat(cos45,0,sin45,0)); 
   
}

void GLShapeDrawer::InitCircleDrawer(){
    std::vector<Resources::Vertex> tmpv;
    int cnt=32;
    float r=1;    
    for (int i=0;i<cnt;++i){
        float theta = 2.0f*i*SM_PI/cnt;
        float x=cosf(-theta)*r;
        float y=sinf(-theta)*r;
        tmpv.push_back({
            {x,y,0},
            {0,0},
            {0,0,0}
        });
    }
    std::vector<uint32_t> idx;
    circlemeshp_ = std::make_unique<Resources::SMesh>(tmpv,idx);
    std::vector<Resources::Vertex> pointv;
    pointv.push_back({
        {0,0,0},
        {0,0},
        {0,0,0}
    });
    singlepointp_ = std::make_unique<Resources::SMesh>(pointv,idx);

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
    SANA_ASSERT(arrowshader_!=nullptr&&"ShapeDrawer.arrowshader create failed!");
   
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
    SANA_ASSERT(lineshader_!=nullptr&&"ShapeDrawer.lineshader_ create failed!");

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
    SANA_ASSERT(gridshader_!=nullptr&&"ShapeDrawer.gridshader_ create failed!");
   

}




}