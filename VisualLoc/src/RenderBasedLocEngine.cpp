#include "VisualLoc/RenderBasedLocEngine.h"
#include "SCore/Global/ServiceLocator.h"
#include "SEditor/Core/CameraCtrl.h"
#include "SEditor/Panels/SceneView.h"
#include "SEditor/Panels/TestView.h"
#include "SGUI/Core/UImanager.h"
#include "SMath/Quaternion.h"
#include "SRender/Core/EntityRenderer.h"
#include "SRender/Passes/SimpleRenderPipeline.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/matrix.hpp"
#include "spdlog/spdlog.h"
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/cvstd_wrapper.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d/cuda.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <utility>
#include <vector>
#include <random>
namespace VisualLoc{

RenderBasedLocEngine::RenderBasedLocEngine()
:acam_(defaultcam_,defaultex_),
renderer_(SANASERVICE(SRender::Core::EntityRenderer)){
    renderpipline_.Init(&acam_, &fbo_, 4000, 3000);
}

cv::Mat RenderBasedLocEngine::GetIntrinsicMat(double fx,double fy,double cx,double cy){
    cv::Mat A_matrix_ = cv::Mat::zeros(3, 3, CV_64FC1);   // intrinsic camera parameters
    A_matrix_.at<double>(0, 0) = fx;       //      [ fx   0  cx ]
    A_matrix_.at<double>(1, 1) = fy;       //      [  0  fy  cy ]
    A_matrix_.at<double>(0, 2) = cx;       //      [  0   0   1 ]
    A_matrix_.at<double>(1, 2) = cy;
    A_matrix_.at<double>(2, 2) = 1;
    return A_matrix_;
}


cv::Mat RenderBasedLocEngine::TestRenderCapture(ECS::Actor& initialcam){
    acam_.InitFromActor(initialcam);
    renderpipline_.RenderTick();
    
    return Fbo2Cvmat();
}

matchpairvec RenderBasedLocEngine::TestFeatureMatch(const cv::Mat& ref_img,ECS::Actor& initialcam,const LocPipelineSetting& setting){
    acam_.InitFromActor(initialcam);
    renderpipline_.RenderTick();
    
    auto res = SURFFeatureMatch(ref_img, Fbo2Cvmat(),setting.fm_lowesratio);
    return res;
}


void RenderBasedLocEngine::LocPipelineMultiRandom(const cv::Mat& ref_img,ECS::Actor& initialcam,const LocPipelineSetting& setting){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> distrib(-1.0,1.0);
    acam_.InitFromActor(initialcam);
    auto pos0 = acam_.GetPos();
    auto orien0 = acam_.GetOrien();
    for (int i=0;i<30;++i){
        double transStep=10;
        double rotatStep=30;
        double dx = distrib(mt)*transStep;
        double dy = distrib(mt)*transStep;
        double dz = distrib(mt)*transStep;
        double dh = distrib(mt)*rotatStep;
        double dv = distrib(mt)*rotatStep;
        acam_.SetExtrinsic(orien0, pos0);
        acam_.translate({dx,dy,dz});
        acam_.FpsRotate(dh, dv);
        int ret = RunVisLocSingle(ref_img,setting);
        if (ret) return;
    }
    acam_.SetExtrinsic(orien0, pos0);
}
void RenderBasedLocEngine::LocPipeline(const cv::Mat& ref_img,ECS::Actor& initialcam,const LocPipelineSetting& setting){

    acam_.InitFromActor(initialcam);
    RunVisLocSingle(ref_img,setting);
}

static void VisualizeCtrlPoints(const std::vector<cv::Point3f>& objpts){
    auto& testview =  SANASERVICE(SGUI::Core::UImanager).GetPanel<SEditor::Panels::TestView>("Test View");
    auto& sceneview =  SANASERVICE(SGUI::Core::UImanager).GetPanel<SEditor::Panels::SceneView>("Scene View");
    sceneview.ctlpts.clear();
    for (auto& i:objpts){
        sceneview.ctlpts.push_back({i.x,i.y,i.z});
    }
}

int RenderBasedLocEngine::RunVisLocSingle(const cv::Mat& ref_img,const LocPipelineSetting& setting){
    renderpipline_.RenderTick();
    
    auto res = SURFFeatureMatch(ref_img, Fbo2Cvmat(),setting.fm_lowesratio);
    std::vector<cv::Point3f> objpts;
    std::vector<cv::Point2f> imgpts;
    if (res.size()<4) return 0;
    for (auto& i:res){
        imgpts.push_back(i.pt1.pt);
        auto tmp = RetrievPosFromPixel(i.pt2.pt.x, i.pt2.pt.y);
        objpts.push_back({tmp.x,tmp.y,tmp.z});
    }
    VisualizeCtrlPoints(objpts);
    double fpix = (double)acam_.cam_->Getfocal_length()*ref_img.size().height/acam_.cam_->Getsensor_size_h();
    
    auto inmat = GetIntrinsicMat(fpix, fpix, ref_img.size().width/2.0 , ref_img.size().height/2.0);
    
    cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64FC1);
    
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64FC1);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64FC1);

    bool useExtrinsicGuess = false;
    int iterationscount=setting.pnp_iterationscount;
    float reprojectionerror=setting.pnp_reprojectionerror;
    double confidence=setting.pnp_confidence;
    
    auto[orien,trans,inliers] = RansacPnpPass(objpts,imgpts,inmat,distCoeffs,useExtrinsicGuess,iterationscount,reprojectionerror,confidence);
    if (inliers.size().width==0){ //failed
        spdlog::warn("[VISLOC] Pipeline Failed");
        return 0;
    }
    auto tmpeul = sm::Quat2Eul( acam_.GetOrien());
    spdlog::info("[VISLOC] Pipeline Success, Inliers: {} trans: {} {} {} rotat: {} {} {}",
        inliers.size().height, acam_.GetPos().x,acam_.GetPos().y,acam_.GetPos().z,
        tmpeul.x,tmpeul.y,tmpeul.z);
    acam_.SetExtrinsic(orien, trans);
    return inliers.size().width;
}

std::tuple<glm::quat,glm::vec3,cv::Mat> RenderBasedLocEngine::RansacPnpPass(const std::vector<cv::Point3f>& objpts,const std::vector<cv::Point2f>& imgpts,
const cv::Mat& inmat,const cv::Mat& distCoeffs,
 bool useExtrinsicGuess ,
    int iterationscount,
    float reprojectionerror,
    double confidence){
    
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_64FC1);
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_64FC1);

    cv::Mat inliers;
    cv::solvePnPRansac(
        objpts,imgpts,inmat,
        distCoeffs,rvec,tvec,useExtrinsicGuess,iterationscount,
        reprojectionerror,confidence,inliers,cv::SOLVEPNP_EPNP);
    cv::Mat R_matrix_ = cv::Mat::zeros(3, 3, CV_64FC1);
    Rodrigues(rvec, R_matrix_); // converts Rotation Vector to Matrix
    auto glmvec = glm::make_vec3(reinterpret_cast<double*>(tvec.data));
    auto glmmat =  glm::make_mat3(reinterpret_cast<double*>(R_matrix_.data));
    glmmat[1]*=-1;
    glmmat[2]*=-1;
    glmmat = glm::transpose(glmmat);
    //opencv cam model convert 2 opengl (rotate around x axis)
    glmvec[1]*=-1;
    glmvec[2]*=-1;
    auto orien = glm::conjugate( glm::quat_cast(glmmat));
    return {orien,orien*-glmvec,inliers};
}

cv::Mat RenderBasedLocEngine::Fbo2Cvmat(){
    cv::Mat res( fbo_.buf_size_.second, fbo_.buf_size_.first,CV_8UC3);
    fbo_.Bind();
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    renderer_.ReadPixels(0, 0, fbo_.buf_size_.first, fbo_.buf_size_.second, GL_BGR, GL_UNSIGNED_BYTE, res.data);
    glPixelStorei(GL_PACK_ALIGNMENT,4);
    fbo_.Unbind();
    cv::flip(res, res, 0);
    return res;
}
// topleft origin point
glm::vec3 RenderBasedLocEngine::RetrievPosFromPixel(int x,int y){
    float depth=0;
    fbo_.Bind();
    y=fbo_.buf_size_.second-y-1;
    renderer_.ReadPixels(x,y, 
        1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    fbo_.Unbind();
    
    if (depth==1) {
        return glm::vec3(0,0,0);
    }
    glm::vec4 ndcpos(1);
    ndcpos.x = 2.0f*x/fbo_.buf_size_.first+1.0f/fbo_.buf_size_.first-1.0f;
    ndcpos.y = 2.0f*y/fbo_.buf_size_.second+1.0f/fbo_.buf_size_.second-1.0f;
    ndcpos.z = 2.0f*depth-1.0f;
    auto tmp =glm::inverse(acam_.cam_->GetProjectionMat()*acam_.cam_->GetViewMat())*ndcpos;
    return tmp/tmp.w;
}

matchpairvec RenderBasedLocEngine::SURFFeatureMatch(const cv::Mat& img1,const cv::Mat& img2,float lowe_nnr){
    auto bfmatcher = cv::cuda::DescriptorMatcher::createBFMatcher(cv::NORM_L2);
    auto surf = cv::cuda::SURF_CUDA::create(400);

    cv::Mat cpuimggray1;
    cv::Mat cpuimggray2;
    img1.copyTo(cpuimggray1);
    img2.copyTo(cpuimggray2);
    cv::cvtColor(cpuimggray1, cpuimggray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(cpuimggray2, cpuimggray2, cv::COLOR_BGR2GRAY);
    cv::cuda::GpuMat imggray1;
    imggray1.upload(cpuimggray1);
    cv::cuda::GpuMat keys1;
    cv::cuda::GpuMat desc1;
    cv::cuda::GpuMat mask1;
    std::vector<cv::KeyPoint> cpukeys1;
    surf->detectWithDescriptors(imggray1, mask1, keys1, desc1);
    surf->downloadKeypoints(keys1, cpukeys1);

    cv::cuda::GpuMat imggray2;
    imggray2.upload(cpuimggray2);
    cv::cuda::GpuMat keys2;
    cv::cuda::GpuMat desc2;
    cv::cuda::GpuMat mask2;
    std::vector<cv::KeyPoint> cpukeys2;
    surf->detectWithDescriptors(imggray2, mask2, keys2, desc2);
    surf->downloadKeypoints(keys2, cpukeys2);

    std::vector<std::vector<cv::DMatch>> cpuknnmatches;
    cv::cuda::GpuMat gpuknnmatches;
    bfmatcher->knnMatchAsync(desc1,desc2,gpuknnmatches,2);
    cv::cuda::Stream::Null().waitForCompletion();
    bfmatcher->knnMatchConvert(gpuknnmatches, cpuknnmatches);
    std::vector<cv::DMatch> resmatches;
    
    // lowe's ratio test
    
    for (auto& top2match:cpuknnmatches){
        if (top2match.size()>1&&top2match[0].distance/top2match[1].distance < lowe_nnr){
            resmatches.push_back(top2match[0]); //good match
        }
    }
    
    matchpairvec res;
    for (auto& i:resmatches){
        res.push_back({cpukeys1[i.queryIdx],cpukeys2[i.trainIdx],i.distance});
    }
    return res;

}


matchpairvec RenderBasedLocEngine::ORBFeatureMatch(const cv::Mat& img1,const cv::Mat& img2){
    auto orb = cv::cuda::ORB::create(500, 1.2f, 8, 31, 0, 2, 0, 31, 20, true);
    auto bfmatcher = cv::cuda::DescriptorMatcher::createBFMatcher(cv::NORM_HAMMING);
   
    // cv::Mat img1 = cv::imread(R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\GovFacility\DJI_0314.JPG)");
    // cv::Mat img2 = cv::imread(R"(C:\Users\cnt0\BUAA\DATA\ExperimentsData\Photos\GovFacility\DJI_0315.JPG)");
    cv::Mat cpuimggray1;
    cv::Mat cpuimggray2;
    img1.copyTo(cpuimggray1);
    img2.copyTo(cpuimggray2);
    cv::cvtColor(cpuimggray1, cpuimggray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(cpuimggray2, cpuimggray2, cv::COLOR_BGR2GRAY);
    cv::cuda::GpuMat imggray1;
    imggray1.upload(cpuimggray1);
    cv::cuda::GpuMat keys1;
    cv::cuda::GpuMat desc1;
    cv::cuda::GpuMat mask1;
    std::vector<cv::KeyPoint> cpukeys1;
    orb->detectAndComputeAsync(imggray1, mask1, keys1, desc1);
    cv::cuda::Stream::Null().waitForCompletion();
    orb->convert(keys1, cpukeys1);

    cv::cuda::GpuMat imggray2;
    imggray2.upload(cpuimggray2);
    cv::cuda::GpuMat keys2;
    cv::cuda::GpuMat desc2;
    cv::cuda::GpuMat mask2;
    std::vector<cv::KeyPoint> cpukeys2;
    orb->detectAndComputeAsync(imggray2, mask2, keys2, desc2);
    cv::cuda::Stream::Null().waitForCompletion();
    orb->convert(keys2, cpukeys2);

    std::vector<std::vector<cv::DMatch>> cpuknnmatches;
    cv::cuda::GpuMat gpuknnmatches;
    bfmatcher->knnMatchAsync(desc1,desc2,gpuknnmatches,2);
    cv::cuda::Stream::Null().waitForCompletion();
    bfmatcher->knnMatchConvert(gpuknnmatches, cpuknnmatches);
    std::vector<cv::DMatch> resmatches;
    
    // lowe's ratio test
    constexpr float nnr=0.95;
    for (auto& top2match:cpuknnmatches){
        if (top2match.size()>1&&top2match[0].distance/top2match[1].distance < nnr){
            resmatches.push_back(top2match[0]); //good match
        }
    }
    
    matchpairvec res;
    for (auto& i:resmatches){
        res.push_back({cpukeys1[i.queryIdx],cpukeys2[i.trainIdx],i.distance});
    }
    return res;

}




}