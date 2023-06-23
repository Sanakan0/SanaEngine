#include "SRender/LowRenderer/Camera.h"
#include <cmath>
#include <functional>
#include <opencv2/core.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <utility>
#include <vector>
#include <iostream>
namespace VisualLoc{
using LinesT = std::vector<std::vector<std::pair<double,double>>>;
using DistFuncT = std::function<std::pair<double, double>(std::pair<double, double>,float*)>;

class DistortionRectifier{
public:

    DistFuncT index_undistfunc;

    DistortionRectifier(){
        index_undistfunc = [](std::pair<double, double> dpts,float* params)->std::pair<double, double>{
            auto rd2 = dpts.first*dpts.first+dpts.second*dpts.second;
            auto k = params[0];
            auto alpha = 1.0/sqrt(1.0-k*rd2);
            return {dpts.first*alpha,dpts.second*alpha};
        };
    }


    void RectifyWithLines(LinesT& lines,float aspect_ratio,SRender::LowRenderer::RadialDistortion& dist_param){
        
        LinesT lines_halfH_norm;
        lines_halfH_norm.resize(lines.size());
        for (int k=0;k<lines.size();++k){
            for (auto& i:lines[k]){
                lines_halfH_norm[k].push_back({(i.first-0.5f)*aspect_ratio*2,(i.second-0.5)*2});
            }
        }
        
        //gradiant
        double momentum = 0;

        double beta = 0.9;
        double learning_rate = 1;
        
        auto lossf = std::bind(&DistortionRectifier::LossFunc,this, lines_halfH_norm,index_undistfunc,std::placeholders::_1);
        
        for (int it = 0; it < 5000; it++)
        {
            double deriv =GetLossDeriv(dist_param.dist_para,1, 0, lossf);
            momentum = beta * momentum + (1 - beta) * deriv;
            if (fabs(momentum) > 0.01) momentum = 0.01 * momentum / fabs(momentum);
            dist_param.dist_para[0] -= learning_rate * momentum;
            // current_mse = getEstimationMse(X_sub, Y_sub, coeff,
            // 	&FocalIndexDistortion_tan::radialDistortionFunction);
            // update result
            // if (current_mse < min_mse)
            // {
            // 	min_mse = current_mse;
            // 	best_k = coeff[1];
            // }
            // if (current_mse < 1e-10)
            // {
            // 	break;
            // }
        }
    }

    double GetLossDeriv(float* params,int paraN,int target_idx,std::function<double(float*)> loss){
        auto curloss=loss(params);
        double delta = 1e-7;
        std::vector<float>newp;
        
        for (int i=0;i<paraN;++i)newp.push_back(params[i]);
        newp[target_idx] +=delta;

        double newloss = loss(&newp[0]);
        double deriv = (newloss-curloss) / delta;
        //std::cout << deriv << std::endl;
        return deriv;
    }

    double LossFunc(LinesT& lines,DistFuncT undistf,float* dist_params){
        //LinesT distlines;
        std::vector<std::vector<cv::Point2d>> distlines;
        distlines.resize(lines.size());
        double res=0;
        for (int k=0;k<lines.size();++k){
            // for (auto& i:lines[k]){
            //     auto upts=undistf(i,dist_params);
            //     distlines[k].push_back(cv::Point2d(upts.first,upts.second ));
            // }
            // cv::Vec4d linep;
            // cv::fitLine(distlines[k], linep, cv::DIST_L2,1,1e-8,1e-8);
            // cv::Point2d pt0(linep[2],linep[3]);
            // cv::Point2d vec0(linep[0],linep[1]);

            // auto st=distlines[k][0]-pt0;
            // auto ed=distlines[k][distlines[k].size()-1]-pt0;
            // auto scale = std::abs(st.dot(vec0)-ed.dot(vec0));

            // for (auto& i:distlines[k]){
            //     auto vec = i-pt0;
            //     auto d0 = vec.dot(vec0);
            //     auto d1 = cv::norm(vec);
            //     auto dist = sqrt(d1*d1-d0*d0);
            //     res+=dist/scale;
            // }
            for (int i=0;i<lines[k].size();++i){
                auto upts=undistf(lines[k][i],dist_params);
                distlines[k].push_back(cv::Point2d(upts.first,upts.second ));
                if (i>=2){
                    auto vec0=distlines[k][i-1]-distlines[k][i-2];
                    auto vec1=distlines[k][i]-distlines[k][i-1];
                    auto cosa=vec0.dot(vec1)/(cv::norm(vec0)*cv::norm(vec1));
                    res+=std::acos(cosa);
                }
                
            }
        }
        
        return res;
    }


};


}