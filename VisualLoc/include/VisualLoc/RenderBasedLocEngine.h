#pragma once
#include "SRender/Buffers/GLFrameBuffer.h"
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
    matchpairvec ORBFeatureMatch(const cv::Mat& img1,const cv::Mat& img2);
    matchpairvec SURFFeatureMatch(const cv::Mat& img1,const cv::Mat& img2);
private:
    //SRender::Buffers::GLFrameBuffer fbo_;


};

}