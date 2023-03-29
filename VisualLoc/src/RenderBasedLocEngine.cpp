#include "VisualLoc/RenderBasedLocEngine.h"
#include <opencv2/core/base.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/cvstd_wrapper.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/xfeatures2d/cuda.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <utility>
#include <vector>
namespace VisualLoc{


matchpairvec RenderBasedLocEngine::SURFFeatureMatch(const cv::Mat& img1,const cv::Mat& img2){
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
    constexpr float nnr=0.5;
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