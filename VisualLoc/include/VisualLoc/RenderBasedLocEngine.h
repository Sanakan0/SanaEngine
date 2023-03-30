#pragma once
#include "ECS/Actor.h"
#include "SEditor/Core/CameraCtrl.h"
#include "SMath/Transform.h"
#include "SRender/Buffers/GLFrameBuffer.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/LowRenderer/Camera.h"
#include "SRender/Passes/SimpleRenderPipeline.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
namespace VisualLoc{

struct MatchPair{
    cv::KeyPoint pt1;
    cv::KeyPoint pt2;
    float distance;
};

using matchpairvec = std::vector<MatchPair>;

class RenderBasedLocEngine{
public:
    RenderBasedLocEngine();
    void LocPipeline(const cv::Mat& ref_img,ECS::Actor& initialcam);
    
    matchpairvec Test(const cv::Mat& ref_img,ECS::Actor& initialcam);
    cv::Mat Test2(ECS::Actor& initialcam);
    
    cv::Mat Fbo2Cvmat();

    glm::vec3 RetrievPosFromPixel(int x,int y);

    cv::Mat GetIntrinsicMat(double fx,double fy,double cx,double cy);

    matchpairvec ORBFeatureMatch(const cv::Mat& img1,const cv::Mat& img2);
    matchpairvec SURFFeatureMatch(const cv::Mat& img1,const cv::Mat& img2);
    
    SRender::Buffers::GLFrameBuffer fbo_;
private:
    SRender::LowRenderer::Camera defaultcam_;
    sm::Transform defaultex_;
    SEditor::Core::ACamera acam_;
    SRender::SimpleRenderPipeline renderpipline_;
    SRender::Core::EntityRenderer& renderer_;

};

}