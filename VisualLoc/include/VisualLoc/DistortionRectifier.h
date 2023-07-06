#include "SRender/LowRenderer/Camera.h"
#include <cmath>
#include <functional>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
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
    DistFuncT division_undistfunc;
    DistFuncT poly5_undistfunc;

    DistortionRectifier(){
        index_undistfunc = [](std::pair<double, double> dpts,float* params)->std::pair<double, double>{
            auto rd2 = dpts.first*dpts.first+dpts.second*dpts.second;
            auto k = params[0];
            auto alpha = 1.0/sqrt(1.0-k*rd2);
            return {dpts.first*alpha,dpts.second*alpha};
        };
        division_undistfunc = [](std::pair<double, double> dpts,float* params)->std::pair<double, double>{
            auto rd2 = dpts.first*dpts.first+dpts.second*dpts.second;
            auto k = params[0];
            auto alpha = 1.0/(1.0+k*rd2);
            return {dpts.first*alpha,dpts.second*alpha};
        };
        poly5_undistfunc = [](std::pair<double, double> dpts,float* params)->std::pair<double, double>{
            auto rd = sqrt( dpts.first*dpts.first+dpts.second*dpts.second);
            auto k1 = params[0];
            auto k2 = params[1];
            std::vector<double> coeff{-rd,1,0,k1,0,k2};
            cv::Mat output;
            auto rev=cv::solvePoly(coeff,output);
            // std::cout<<output<<std::endl;
            // std::cout<<rev<<std::endl;
            double ru=1e5;
            for (int i=0;i<output.rows;++i){
                auto tmp = output.at<std::complex<double>>(i,0);
                if (tmp.imag()==0&&tmp.real()!=0){
                    ru=std::min(ru,tmp.real());
                }
            }

            
            auto alpha = ru/rd;
            return {dpts.first*alpha,dpts.second*alpha};
        };
    }


    double CalcDistMse(LinesT& lines,DistFuncT undistf,float* dist_params){
        std::vector<std::vector<cv::Point2d>> distlines;
        distlines.resize(lines.size());
        double res=0;
        int cntN=0;
         for (int k=0;k<lines.size();++k){
            for (auto& i:lines[k]){
                auto upts=undistf(i,dist_params);
                distlines[k].push_back(cv::Point2d(upts.first,upts.second ));
            }
            cv::Vec4d linep;
            cv::fitLine(distlines[k], linep, cv::DIST_L2,1,1e-8,1e-8);
            cv::Point2d pt0(linep[2],linep[3]);
            cv::Point2d vec0(linep[0],linep[1]);

            auto st=distlines[k][0]-pt0;
            auto ed=distlines[k][distlines[k].size()-1]-pt0;
            //auto scale = std::abs(st.dot(vec0)-ed.dot(vec0));

            for (auto& i:distlines[k]){
                auto vec = i-pt0;
                auto d0 = vec.dot(vec0);
                auto d1 = cv::norm(vec);
                auto dist = sqrt(d1*d1-d0*d0);
                res+=dist;
                cntN++;
            }
        }
        return res/cntN;
    }

    double RectifyWithLines(LinesT& lines,float aspect_ratio,SRender::LowRenderer::RadialDistortion& dist_param){
        poly5_undistfunc({0,3},dist_param.dist_para);
        LinesT lines_halfW_norm;
        lines_halfW_norm.resize(lines.size());
        for (int k=0;k<lines.size();++k){
            for (auto& i:lines[k]){
                //lines_halfW_norm[k].push_back({(i.first-0.5f)*aspect_ratio*2,(i.second-0.5)*2});
                lines_halfW_norm[k].push_back({(i.first-0.5f)*2,(i.second-0.5)/aspect_ratio*2});
            }
        }
        
        //gradiant
        double momentum[3] = {0,0,0};

        double beta = 0.9;
        double learning_rate = 1;
        int parN=1;
        DistFuncT undist_func=index_undistfunc;
        switch (dist_param.dist_type) {
            case SRender::LowRenderer::DistortionModel::INDEX:
                undist_func=index_undistfunc;
                break;
            case SRender::LowRenderer::DistortionModel::DIVISION:
                undist_func=division_undistfunc;
                break;
            case SRender::LowRenderer::DistortionModel::POLY5:
                undist_func=poly5_undistfunc;
                parN=2;
                break;
        }



        auto lossf = std::bind(&DistortionRectifier::LossFunc,this, lines_halfW_norm,undist_func,std::placeholders::_1);
        auto distlossf =  std::bind(&DistortionRectifier::DistmseLossFunc,this, lines_halfW_norm,undist_func,std::placeholders::_1);
        for (int i=0;i<parN;++i){
            for (int it = 0; it < 5000; it++)
            {
            
                double deriv =GetLossDeriv(dist_param.dist_para,3, i, lossf);
                momentum[i] = beta * momentum[i] + (1 - beta) * deriv;
                if (fabs(momentum[i]) > 0.01) momentum[i] = 0.01 * momentum[i] / fabs(momentum[i]);
                
          
                dist_param.dist_para[i] -= learning_rate * momentum[i];

            }
        }

        // for (int i=0;i<parN;++i){
        //     for (int it = 0; it < 5000; it++)
        //     {
            
        //         double deriv =GetLossDeriv(dist_param.dist_para,3, i, distlossf);
        //         momentum[i] = beta * momentum[i] + (1 - beta) * deriv;
        //         if (fabs(momentum[i]) > 0.01) momentum[i] = 0.01 * momentum[i] / fabs(momentum[i]);
                
          
        //         dist_param.dist_para[i] -= learning_rate * momentum[i];

        //     }
        // }


        
        if (parN>1){
            for (int it = 0; it < 1000; it++)
            {
                for (int i=0;i<parN;++i){
                    double deriv =GetLossDeriv(dist_param.dist_para,3, i, lossf);
                    momentum[i] = beta * momentum[i] + (1 - beta) * deriv;
                    if (fabs(momentum[i]) > 0.01) momentum[i] = 0.01 * momentum[i] / fabs(momentum[i]);
                    
                }
                for (int i=0;i<parN;++i){
                    dist_param.dist_para[i] -= learning_rate * momentum[i];
                }
                
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
        return distlossf(dist_param.dist_para);
        //return lossf(dist_param.dist_para);
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

    double DistmseLossFunc(LinesT& lines,DistFuncT undistf,float* dist_params){
        //LinesT distlines;
        std::vector<std::vector<cv::Point2d>> distlines;
        distlines.resize(lines.size());
        double res=0;
        int cntN=0;
         for (int k=0;k<lines.size();++k){
            for (auto& i:lines[k]){
                auto upts=undistf(i,dist_params);
                distlines[k].push_back(cv::Point2d(upts.first,upts.second ));
            }
            cv::Vec4d linep;
            cv::fitLine(distlines[k], linep, cv::DIST_L2,1,1e-8,1e-8);
            cv::Point2d pt0(linep[2],linep[3]);
            cv::Point2d vec0(linep[0],linep[1]);

            auto st=distlines[k][0]-pt0;
            auto ed=distlines[k][distlines[k].size()-1]-pt0;
            auto scale = std::abs(st.dot(vec0)-ed.dot(vec0));

            for (auto& i:distlines[k]){
                auto vec = i-pt0;
                auto d0 = vec.dot(vec0);
                auto d1 = cv::norm(vec);
                auto dist = sqrt(d1*d1-d0*d0)/scale;
                res+=dist;
                cntN++;
            }
        }
        return res/cntN;
    }


};


}