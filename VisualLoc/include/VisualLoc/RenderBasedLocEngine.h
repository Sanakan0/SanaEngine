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


struct LocPipelineSetting{
    float fm_lowesratio=0.65;
    int pnp_iterationscount=1000;
    float pnp_reprojectionerror=8;
    double pnp_confidence=0.99;
};

class RenderBasedLocEngine{
public:
    RenderBasedLocEngine();
    void LocPipeline(const cv::Mat& ref_img,ECS::Actor& initialcam,const LocPipelineSetting& setting);
    void LocPipelineMultiRandom(const cv::Mat& ref_img,ECS::Actor& initialcam,const LocPipelineSetting& setting);
    int RunVisLocSingle(const cv::Mat& ref_img,const LocPipelineSetting& setting);
    std::tuple<glm::quat,glm::vec3,cv::Mat> RansacPnpPass(const std::vector<cv::Point3f>& objpts,const std::vector<cv::Point2f>& imgpts,
        const cv::Mat& inmat,const cv::Mat& distCoeffs,
        bool useExtrinsicGuess ,
            int iterationscount,
            float reprojectionerror,
            double confidence);
    matchpairvec TestFeatureMatch(const cv::Mat& ref_img,ECS::Actor& initialcam,const LocPipelineSetting& setting);
    cv::Mat TestRenderCapture(ECS::Actor& initialcam);
    
    cv::Mat Fbo2Cvmat();

    glm::vec3 RetrievPosFromPixel(int x,int y);

    cv::Mat GetIntrinsicMat(double fx,double fy,double cx,double cy);

    matchpairvec ORBFeatureMatch(const cv::Mat& img1,const cv::Mat& img2);
    matchpairvec SURFFeatureMatch(const cv::Mat& img1,const cv::Mat& img2,float lowe_nnr=0.65);
    
    SRender::Buffers::GLFrameBuffer fbo_;
private:
    
    SRender::LowRenderer::Camera defaultcam_;
    sm::Transform defaultex_;
    SEditor::Core::ACamera acam_;
    SRender::SimpleRenderPipeline renderpipline_;
    SRender::Core::EntityRenderer& renderer_;

};

}