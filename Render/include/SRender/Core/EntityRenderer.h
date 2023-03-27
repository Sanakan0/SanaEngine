#pragma once
#include <SRender/Core/GLRenderer.h>
#include "ECS/Component/Component.h"
#include "ECS/Actor.h"
#include "SRender/Resources/GLShader.h"
#include "SRender/Resources/SBaseRenderResources.h"
#include "SRender/Resources/SMaterial.h"
#include "SRender/Resources/STexture.h"
#include "glm/fwd.hpp"
#include <SRender/Resources/SModel.h>
#include <SRender/Settings/GLSet.h>
#include <cstddef>
#include <memory>
#include <stdint.h>
namespace SRender::Core{

class GLShapeDrawer;

class EntityRenderer:public GLRenderer{
friend class GLShapeDrawer;
public:
    EntityRenderer();
    ~EntityRenderer();
    void DrawSkeleton(Resources::SModel& model);
    void DrawActorOutline(ECS::Actor& actor,float linewidth=5.0f);
    void DrawModelwithEmptyTex(Resources::SModel &model,Resources::STexture* empty_tex=nullptr);
    void DrawModel(Resources::SModel& model);
    GLShapeDrawer* GetShapeDrawer(){return shapedrawer_.get();}

    
private:
    std::unique_ptr<GLShapeDrawer> shapedrawer_;
    std::unique_ptr<Resources::STexture> emptytexture_;
    std::unique_ptr<Resources::STexture> outline_colored_texture_;
    std::unique_ptr<Resources::GLShader> unlitshader_;
    Resources::SMaterial empty_mat_;
};




class GLShapeDrawer{

public:
    GLShapeDrawer(EntityRenderer& renderer);
    ~GLShapeDrawer();
    void DrawLine(const glm::vec3& start,const glm::vec3& end,const glm::vec3& color,float width=1.0f);
    void DrawGrid();
    void DrawArrow(const glm::mat4& model_mat);
    void DrawPoint(const glm::vec3& pos,const glm::vec4& color);
    void DrawCircle(const glm::mat4& model_mat,const glm::vec4& diff_color,float width=1.0f);
    void DrawCursor(const glm::vec3& pos,const glm::vec4& diff_color,const glm::mat4& viewmat,float width=1.0f);
    
    // this cmd will clear depth buffer, read before it!
    void DrawTransGizmo(const glm::vec3& pos,const glm::mat4& viewmat,bool is_render_for_pick=false);
    
    void DrawGizmoArrow(const glm::mat4& model_mat,const glm::vec4& diff_color);
    void DrawCamFrame(const glm::mat4& model_mat,float fovyratio,float aspect,const glm::vec4& diff_color,bool is_activate=false,float width=1.0f,bool enable_depth_test=true);
private:
    void InitGridDrawer();
    void InitLineDrawer();
    void InitArrowDrawer();
    void InitGizmoDrawer();
    void InitCamFrameDrawer();
    void InitCircleDrawer();
    std::unique_ptr<Resources::SMesh> singlepointp_=nullptr;
    std::unique_ptr<Resources::SMesh> circlemeshp_=nullptr;
    std::unique_ptr<Resources::SMesh> linemeshp_=nullptr;
    std::unique_ptr<Resources::SMesh> panelmeshp_=nullptr;
    std::unique_ptr<Resources::SMesh> arrowmeshp_=nullptr;
    std::unique_ptr<Resources::SMesh> gizmoarrowmeshp_head_=nullptr;
    std::unique_ptr<Resources::SMesh> gizmoarrowmeshp_headbase_=nullptr;
    std::unique_ptr<Resources::SMesh> gizmoarrowmeshp_body_=nullptr;
    std::unique_ptr<Resources::SMesh> gizmoarrowmeshp_bodybase_=nullptr;
    std::unique_ptr<Resources::SMesh> camframemeshp_=nullptr;
    std::unique_ptr<Resources::SMesh> camframemesh_upp_=nullptr;
    std::unique_ptr<Resources::GLShader> lineshader_=nullptr;
    std::unique_ptr<Resources::GLShader> gridshader_=nullptr;
    std::unique_ptr<Resources::GLShader> arrowshader_=nullptr;
    std::unique_ptr<Resources::GLShader> gizmoshader_=nullptr;
    EntityRenderer& renderer_;
    
    glm::mat4 gizmoarrow_xtrans;
    glm::mat4 gizmoarrow_ytrans;

};

}