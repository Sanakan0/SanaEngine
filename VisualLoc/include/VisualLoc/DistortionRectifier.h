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
    DistFuncT poly5_undistfunc_bi;

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
            auto rev=cv::solvePoly(coeff,output,1000);
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

        auto poly5_distfunc=[](double ru,double k1,double k2)->double{
            return ru + k1*ru*ru*ru+k2*ru*ru*ru*ru*ru;
        };

        poly5_undistfunc = [&](std::pair<double, double> dpts,float* params)->std::pair<double, double>{
             auto rd = sqrt( dpts.first*dpts.first+dpts.second*dpts.second);
            auto k1 = params[0];
            auto k2 = params[1];
            auto b24ac = 9.0*k1*k1-20.0*k2;
             double maxRu=1e8;
            if (b24ac>=0){
                if (k2>0){
                     auto leftx = (-3.0*k1-std::sqrt( b24ac))/(10.0*k2);
                     auto rightx = (-3.0*k1+std::sqrt( b24ac))/(10.0*k2);
                    if (leftx>0) maxRu=std::min(maxRu,leftx);
                    if (rightx>0) maxRu=std::min(maxRu,rightx);
                    if (maxRu<1e8) maxRu=sqrt(maxRu);
                }else if(k1<0){
                    maxRu=-1.0/(3*k1);
                    maxRu=sqrt(maxRu);
                }
            }

             auto maxRd = std::min(poly5_distfunc(maxRu, k1, k2),1e5);
            double ru=1e5;
            if (rd<=maxRd){
                double ll=0,rr=maxRu;
                while(1){
                    ru=(ll+rr)/2;
                    auto currd=poly5_distfunc(ru, k1,k2);
                    if (currd<rd){
                        ll=ru;
                    }else{
                        rr=ru;
                    }
                    if (abs(currd-rd)<1e-6) break;
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

    double RectifyWithLines(LinesT& lines,float aspect_ratio,SRender::LowRenderer::RadialDistortion& dist_param,bool use_distance_loss=false){
       
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
        double deriv_delta=1e-7;
        if (use_distance_loss){
            lossf = distlossf;
            deriv_delta=0.0001;
        }
        for (int i=0;i<parN;++i){
            for (int it = 0; it < 5000; it++)
            {
            
                double deriv =GetLossDeriv(dist_param.dist_para,3, i, lossf,deriv_delta);
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
                    double deriv =GetLossDeriv(dist_param.dist_para,3, i, lossf,deriv_delta);
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

    std::tuple<double,double,double,double> GetCurLoss(LinesT& lines,float aspect_ratio,SRender::LowRenderer::RadialDistortion& dist_param,LinesT* distlines=nullptr){
        if (lines.empty()) return{-1,-1,-1,-1};
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

        if (distlines!=nullptr){
            distlines->clear();
            for (int k=0;k<lines_halfW_norm.size();++k){
                distlines->push_back({});
                for (int i=0;i<lines_halfW_norm[k].size();++i){
                    auto upts=undist_func(lines_halfW_norm[k][i],dist_param.dist_para);
                    (*distlines)[k].push_back(upts);
                }
                
            }
        }

        auto lossf = std::bind(&DistortionRectifier::LossFunc,this, lines_halfW_norm,undist_func,std::placeholders::_1);
        auto distlossf =  std::bind(&DistortionRectifier::DistmseLossFunc,this, lines_halfW_norm,undist_func,std::placeholders::_1);
        
        return {distlossf(dist_param.dist_para),lossf(dist_param.dist_para),
        GetLossDeriv(dist_param.dist_para,3, 0, distlossf,0.0005),GetLossDeriv(dist_param.dist_para,3, 0, lossf)};
        //return lossf(dist_param.dist_para);
    }

    double GetLossDeriv(float* params,int paraN,int target_idx,std::function<double(float*)> loss,double delta=1e-7){
        auto curloss=loss(params);
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
            if (lines[k].empty()) continue;
            for (auto& i:lines[k]){
                auto upts=undistf(i,dist_params);
                distlines[k].push_back(cv::Point2d(upts.first,upts.second ));
            }
            cv::Vec4d linep;
            cv::fitLine(distlines[k], linep, cv::DIST_WELSCH,0,1e-10,1e-10);
            cv::Point2d pt0(linep[2],linep[3]);
            cv::Point2d vec0(linep[0],linep[1]);

            auto st=distlines[k][0]-pt0;
            auto ed=distlines[k][distlines[k].size()-1]-pt0;
            auto scale = std::abs(st.dot(vec0)-ed.dot(vec0));

            for (auto& i:distlines[k]){
                auto vec = i-pt0;
                auto d0 = vec.dot(vec0);
                auto d1 = cv::norm(vec);
                auto dist = sqrt(std::max( d1*d1-d0*d0,0.0));
                res+=dist;
                cntN++;
            }
        }
        if (cntN==0) return 0;
        return res/cntN;
    }


};


}