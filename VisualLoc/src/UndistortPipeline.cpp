#pragma once
#include "SCore/Global/ServiceLocator.h"
#include "SRender/Buffers/GLFrameBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Resources/GLShaderLoader.h"
#include "SResourceManager/Util.h"
#include "VisualLoc/UndistortPipeline.h"
#include "Debug/Assertion.h"
namespace VisualLoc{


UndistortPipeline::UndistortPipeline():renderer_(SANASERVICE(SRender::Core::EntityRenderer))
{
    std::vector<SRender::Resources::Vertex> tmpv;
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
    std::vector<uint32_t> idx{0,1,2,2,3,0};
    panelmeshp_ = std::make_unique<SRender::Resources::SMesh>(tmpv,idx);
    auto pth = ResourceManager::PathManager::GetFullPath(":shaders\\undistortion.glsl");
    shaderp_ = std::unique_ptr<SRender::Resources::GLShader> (SRender::Resources::GLShaderLoader::LoadFromFile(pth));
    SANA_ASSERT(shaderp_!=nullptr&&"UNDISTORTION create failed!");
   
}

void UndistortPipeline::Run(const SRender::Resources::STexture& distimg,float norm_fh,float usensorscale,const SRender::LowRenderer::RadialDistortion& distortioninfo){
    
    int uwidth = distimg.width*usensorscale;
    int uheight = distimg.height*usensorscale;

    shaderp_->SetUniFloatV("DistInfo.dist_para",const_cast<float*>(distortioninfo.dist_para),3);
    shaderp_->SetUniInt("DistInfo.dist_type",(int)distortioninfo.dist_type);
    shaderp_->SetUniFloat("picwidth", uwidth);
    shaderp_->SetUniFloat("picheight", uheight);
    shaderp_->SetUniFloat("norm_fh", norm_fh);
    shaderp_->SetUniFloat("sensorscale", (float)uwidth/distimg.width);

    distimg.Bind(0);
    fbo_.Resize(uwidth, uheight);
    fbo_.Bind();

    renderer_.SetViewPort(0, 0, uwidth, uheight);
    renderer_.SetClearColor(0.0f, 0.0f, 0.0f);
    renderer_.ApplyGLstate(SRender::Core::Default_GLstate);
    glStencilMask(0xFF); // stencil mask also influence glclear()
    renderer_.ClearBuffer();
    glStencilMask(0x00);
    
    shaderp_->Bind();
    renderer_.SetRasterizationMode(SRender::Setting::SRasterization::FILL);
    renderer_.Draw(*panelmeshp_, SRender::Setting::SPrimitive::TRIANGLES);
    
    distimg.Unbind();
    fbo_.Unbind();

}



}