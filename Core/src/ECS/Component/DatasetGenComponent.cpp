#include "ECS/Component/DatasetGenComponent.h"
#include "SEditor/Core/CameraCtrl.h"
#include "SEditor/Core/RuntimeContext.h"
#include "SEditor/Util/NfdDialog.h"
#include "SMath/Transform.h"
#include "SRender/LowRenderer/Camera.h"
#include "SResourceManager/Util.h"
#include "SceneSys/SceneManager.h"
#include "Serialize/Serializer.h"
#include "glm/fwd.hpp"
#include "imgui/imgui.h"
#include "ECS/Actor.h"
#include "ECS/Component/TransformComponent.h"
#include "SRender/Core/EntityRenderer.h"
#include "SCore/Global/ServiceLocator.h"
#include "nfd.h"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdint.h>
#include <string>
#include <tuple>
#include <vector>
#include "spdlog/spdlog.h"
#include "stb_image/stb_image_write.h"
namespace ECS::Components {

DatasetGenComponent::DatasetGenComponent(Actor& parentactor):
Component(parentactor),dist_fbo_(0,0,0,1),acam_(defaultCam_,defaultTran_),
scenemanager_(SANASERVICE(SceneSys::SceneManager)){
    threadpool_.init();
}

DatasetGenComponent::~DatasetGenComponent(){
    threadpool_.shutdown();
}

void DatasetGenComponent::Tick(float delta_t){

}

void DatasetGenComponent::OnUpdate(float delta_t){
    if (cnt_<totalImgCnt_&&camStartGen){
        if (threadpool_.queueSize()>500) return;
        if (genType_==0){
            AutoGenrate();
        }
        else if (genType_==1){
            auto& tmp = scenemanager_.GetScene()->GetBasicRenderComponent().camcomps;
            auto camcomp = tmp[cnt_/lens_.size()];
            auto tmpcam = camcomp->cam_;
            auto[f,k] = lens_[cnt_%lens_.size()];k+=0.3;
            tmpcam.Setfocal_length(f);
            tmpcam.Setsensor_size_h(24);
            tmpcam.Setaspect_ratio(1.5);
            tmpcam.distortion_.dist_para[0]=k;
            tmpcam.distortion_.dist_type=SRender::LowRenderer::DistortionModel::INDEX;
            RenderImg(tmpcam, camcomp->parentactor_.GetTransformComponent().trans_,std::to_string(cnt_));
            cnt_++;
        }
        
    }else {
        camStartGen=0;
    }
   
}

void DatasetGenComponent::DrawInspector() {
    if(ImGui::CollapsingHeader("Dataset")){
        
        ImGui::DragFloat("extends x", &extends_.x,1,1);
        ImGui::DragFloat("extends y", &extends_.y,1,1);
        ImGui::DragFloat("extends z", &extends_.z,1,1);
        ImGui::Separator();
        ImGui::DragFloat("步长", &step_,1,1,FLT_MAX/INT_MAX);
        ImGui::DragFloat("fovt",&fovy_,0.1);
        ImGui::DragFloat("aspe",&aspect_,0.1);
        ImGui::Separator();
        ImGui::InputInt("Resolution:", &resolution_);
        ImGui::Text(("save_path: "+save_pth_).c_str());
        ImGui::SameLine();
        if (ImGui::Button("打开...")){
        
            auto savepth =SEditor::Util::NfdDialog::OpenFolderDlg(ResourceManager::PathManager::GetProjectPath());
            auto relative = std::filesystem::relative(std::filesystem::u8path(savepth),
            std::filesystem::u8path(ResourceManager::PathManager::GetProjectPath()));
            //std::cout <<"fk" << relative.generic_u8string()<<std::endl;
            save_pth_="@"+relative.generic_u8string();
        }
        ImGui::Text(("LensDatafile_path: "+lensdata_pth_).c_str());
        ImGui::SameLine();
        if (ImGui::Button("打开...##second")){
            auto savepth = SEditor::Util::NfdDialog::OpenFileDlg({{"Lens config","txt"}},ResourceManager::PathManager::GetProjectPath());
            auto relative = std::filesystem::relative(std::filesystem::u8path(savepth),
            std::filesystem::u8path(ResourceManager::PathManager::GetProjectPath()));
            //std::cout <<"fk" << relative.generic_u8string()<<std::endl;
            lensdata_pth_="@"+relative.generic_u8string();
            LoadLensData();
        } 
        ImGui::SameLine();
        if (ImGui::Button("加载")){
            LoadLensData();
        }

        // if (ImGui::Button("save img")){
        //     auto& trans = parentactor_.GetTransformComponent().trans_;
        //     defaultTran_=trans;
        //     RenderImg(defaultCam_,defaultTran_);
        // }
        if (ImGui::Button("通过阵列生成")){
            auto tmpnum =  extends_*2.0f/step_;
            auto camnum = ((int)tmpnum.x+1)*((int)tmpnum.y+1)*((int)tmpnum.z+1);
            int roundsamplecnt_=3;
            totalImgCnt_=lens_.size()*camnum*roundsamplecnt_;
            cnt_=0;
            camStartGen=1;
            genType_=0;
            // spdlog::info("[DatasetGen] st");
            // auto& tmp = scenemanager_.GetScene()->GetBasicRenderComponent().camcomps;
            // auto camcomp = tmp[0];
            // for (int i=0;i<10;++i){
            //     RenderImg(camcomp->cam_, camcomp->parentactor_.GetTransformComponent().trans_,std::to_string(i));
            // }
            
            // spdlog::info("[DatasetGen] ed");
        }
        if (ImGui::Button("通过虚拟相机生成")){
            auto camnum= scenemanager_.GetScene()->GetBasicRenderComponent().camcomps.size();
            totalImgCnt_=lens_.size()*camnum;
            cnt_=0;
            camStartGen=1;
            genType_=1;
        }
        
        if (totalImgCnt_){
            ImGui::Separator();
            char buf[32];
            sprintf(buf, "%d/%d", (int)cnt_, totalImgCnt_);
            ImGui::ProgressBar(1.0*cnt_/totalImgCnt_, ImVec2(0.f, 0.f), buf);
            if (ImGui::Button(camStartGen?"Stop":"Continue")){
                camStartGen^=1;
            }
            if (cnt_>=totalImgCnt_){
                ImGui::SameLine();
                ImGui::TextColored({1,0.8,0,1}, "Done!");
            }
            ImGui::Text("Thread Pool Queue Size: %d", threadpool_.queueSize());
        }
    }
}
void DatasetGenComponent::Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    SCore::Serializer::SerializeInt(p_doc, p_node, "resolution", resolution_);
    SCore::Serializer::SerializeVec3(p_doc, p_node, "extends", extends_);
    SCore::Serializer::SerializeFloat(p_doc,p_node, "step", step_);
    SCore::Serializer::SerializeString(p_doc, p_node, "save_pth", save_pth_);
    SCore::Serializer::SerializeString(p_doc, p_node, "lensdata_pth", lensdata_pth_);
}

void DatasetGenComponent::Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) {
    SCore::Serializer::DeserializeInt(p_doc, p_node, "resolution", resolution_);
    SCore::Serializer::DeserializeVec3(p_doc, p_node, "extends", extends_);
    SCore::Serializer::DeserializeFloat(p_doc,p_node, "step", step_);
    SCore::Serializer::DeserializeString(p_doc, p_node, "save_pth", save_pth_);
    SCore::Serializer::DeserializeString(p_doc, p_node, "lensdata_pth", lensdata_pth_);
    LoadLensData();
}


void DatasetGenComponent::AutoGenrate(){
    static sm::Transform trans;
    static glm::vec3 pos,num;
    static int i=0,j=0,k=0;
    static int lenscnt=0;
    if (cnt_==0){
        trans = parentactor_.GetTransformComponent().trans_;
        pos = trans.GetPosW()+trans.GetPosL()-extends_;
        num =  extends_*2.0f/step_;
        i=0;j=0;k=0;
        lenscnt=0;
    }
    auto drawaround = [&](glm::vec3 pos){
        sm::Transform trans;
        trans.SetPosW(pos);
        glm::vec3 euler{0,0,0};
        for (int i=0;i<1;++i){
            euler.x = i*45+45;
            for (int j=0;j<3;++j){
                euler.z = j*120;
                trans.SetEulerW(euler);
                SRender::LowRenderer::Camera tmpcam;
                auto[f,k] = lens_[lenscnt];
                tmpcam.Setfocal_length(f);
                tmpcam.Setsensor_size_h(24);
                tmpcam.Setaspect_ratio(1.5);
                tmpcam.distortion_.dist_para[0]=k;
                tmpcam.distortion_.dist_type=SRender::LowRenderer::DistortionModel::INDEX;
                RenderImg(tmpcam, trans,std::to_string(cnt_));
                cnt_++;
            }
        }
    };

    for (;i<=(int)num.x;++i){
        for (;j<=(int)num.y;++j){
            for (;k<=(int)num.z;++k){
                for (;lenscnt<lens_.size();){
                    auto curpos = pos+glm::vec3(i*step_,j*step_,k*step_);
                    //renderer.GetShapeDrawer()->DrawPoint(curpos, {1,1,1,1});
                    drawaround(curpos);
                    lenscnt++;
                    return;
                }
                lenscnt=0;
            }
            k=0;
        }
        j=0;
    }
    i=0;
}

void DatasetGenComponent::OnDrawGizmo(float delta_t){
    

    static auto& rtcontext = SANASERVICE(SEditor::Core::RuntimeContext);
    if (rtcontext.scene_manager_->GetSelectedActorID()!=parentactor_.GetID()) return;
    auto& renderer = *rtcontext.core_renderer_;
    
    auto drawaround = [&](glm::vec3 pos){
        sm::Transform trans;
        trans.SetPosW(pos);
        glm::vec3 euler{0,0,0};
        for (int i=0;i<3;++i){
            euler.x = i*45+45;
            for (int j=0;j<4;++j){
                euler.z = j*90;
                trans.SetEulerW(euler);
                renderer.GetShapeDrawer()->DrawCamFrame(trans.GetMat(),fovy_,aspect_,{1,1,1,1});
            }
        }
    };
    
    auto& trans = parentactor_.GetTransformComponent().trans_;
    auto pos = trans.GetPosW()+trans.GetPosL()-extends_;
    auto num =  extends_*2.0f/step_;
    
    for (int i=0;i<=num.x;++i){
        for (int j=0;j<=num.y;++j){
            for (int k=0;k<=num.z;++k){
                auto curpos = pos+glm::vec3(i*step_,j*step_,k*step_);
                //renderer.GetShapeDrawer()->DrawPoint(curpos, {1,1,1,1});
                drawaround(curpos);
            }
        }
    }
}

static void SaveFbothreads(std::vector<uint8_t> color,std::vector<float> depth,std::pair<int,int> bufsize, const std::string& save_pth,const std::string& name){
    auto& tmp = color;
    auto& tmpdepth = depth;
    //return;
    auto pth = save_pth;
    auto imgpth = pth+"/"+name+".png";
    auto depthpth = pth + "/"+name+"_depth.png";

    // {
    //     std::vector<uint8_t> newdepth;
    //     auto nearr = renderpipeline_.GetCam()->cam_->near_;
    //     auto farr = renderpipeline_.GetCam()->cam_->far_;

    //     for (auto &i:tmpdepth) {
    //         auto lineardepth = 2*nearr/((farr+nearr)-(2*i-1)*(farr-nearr));
    //         uint8_t cr = (1.0-lineardepth)*255;
    //         newdepth.push_back(cr);
    //         newdepth.push_back(cr);
    //         newdepth.push_back(cr);
    //         newdepth.push_back(255);
    //     }
    //     auto c1depthpth = pth + "/"+name+"_depth_c1.png";
    //     cv::Mat C1depthA(bufsize.second,bufsize.first,CV_8UC4,newdepth.data());
    //     //cv::cvtColor(depthA,depthA, cv::COLOR_RGBA2GRAY);
    //     //cv::cvtColor(depthA,depthA, cv::COLOR_BGRA2RGBA);
    //     cv::flip(C1depthA,C1depthA,0);
    //     cv::imwrite(c1depthpth, C1depthA,{cv::IMWRITE_PNG_COMPRESSION,0});
    // }


    cv::Mat A(bufsize.second,bufsize.first,CV_8UC3,tmp.data());
    cv::Mat depthA(bufsize.second,bufsize.first,CV_8UC4,tmpdepth.data());
    
    cv::cvtColor(A,A, cv::COLOR_BGR2RGB);
    cv::cvtColor(depthA,depthA, cv::COLOR_BGRA2RGBA);
    cv::flip(A,A,0);
    cv::flip(depthA,depthA,0);

    cv::imwrite(imgpth, A,{cv::IMWRITE_PNG_COMPRESSION,0});
    cv::imwrite(depthpth, depthA,{cv::IMWRITE_PNG_COMPRESSION,0});
    
    return;
    stbi_flip_vertically_on_write(1);
    stbi_write_png_compression_level = 0;
    stbi_write_png(imgpth.c_str(), 
    bufsize.first, bufsize.second, 3, (void*)tmp.data(), bufsize.first*3);
    stbi_write_png(depthpth.c_str(), 
    bufsize.first, bufsize.second, 4, (void*)tmpdepth.data(), bufsize.first*4);
}

void DatasetGenComponent::SaveFbo( const std::string& name){
    auto tmp = dist_fbo_.DownloadColor();
    auto tmpdepth = dist_fbo_.DownloadDepth();
    //return;
    auto pth = ResourceManager::PathManager::GetFullPath(save_pth_);
    auto imgpth = pth+"/"+name+".png";
    auto depthpth = pth + "/"+name+"_depth.png";

    {
        std::vector<uint8_t> newdepth;
        auto nearr = renderpipeline_.GetCam()->cam_->near_;
        auto farr = renderpipeline_.GetCam()->cam_->far_;

        for (auto &i:tmpdepth) {
            auto lineardepth = 2*nearr/((farr+nearr)-(2*i-1)*(farr-nearr));
            uint8_t cr = (1.0-lineardepth)*255;
            newdepth.push_back(cr);
            newdepth.push_back(cr);
            newdepth.push_back(cr);
            newdepth.push_back(255);
        }
        auto c1depthpth = pth + "/"+name+"_depth_c1.png";
        cv::Mat C1depthA(dist_fbo_.buf_size_.second,dist_fbo_.buf_size_.first,CV_8UC4,newdepth.data());
        //cv::cvtColor(depthA,depthA, cv::COLOR_RGBA2GRAY);
        //cv::cvtColor(depthA,depthA, cv::COLOR_BGRA2RGBA);
        cv::flip(C1depthA,C1depthA,0);
        cv::imwrite(c1depthpth, C1depthA,{cv::IMWRITE_PNG_COMPRESSION,0});
    }


    cv::Mat A(dist_fbo_.buf_size_.second,dist_fbo_.buf_size_.first,CV_8UC3,tmp.data());
    cv::Mat depthA(dist_fbo_.buf_size_.second,dist_fbo_.buf_size_.first,CV_8UC4,tmpdepth.data());
    
    cv::cvtColor(A,A, cv::COLOR_BGR2RGB);
    cv::cvtColor(depthA,depthA, cv::COLOR_BGRA2RGBA);
    cv::flip(A,A,0);
    cv::flip(depthA,depthA,0);

    cv::imwrite(imgpth, A,{cv::IMWRITE_PNG_COMPRESSION,0});
    cv::imwrite(depthpth, depthA,{cv::IMWRITE_PNG_COMPRESSION,0});
    
    return;
    stbi_flip_vertically_on_write(1);
    stbi_write_png_compression_level = 0;
    stbi_write_png(imgpth.c_str(), 
    dist_fbo_.buf_size_.first, dist_fbo_.buf_size_.second, 3, (void*)tmp.data(), dist_fbo_.buf_size_.first*3);
    stbi_write_png(depthpth.c_str(), 
    dist_fbo_.buf_size_.first, dist_fbo_.buf_size_.second, 4, (void*)tmpdepth.data(), dist_fbo_.buf_size_.first*4);
}

void DatasetGenComponent::RenderImg(SRender::LowRenderer::Camera& intrinsic,sm::Transform& extrinsic,const std::string& name){
    acam_.SetCamInExParam(intrinsic, extrinsic);
    //acam_.SetExtrinsic(orien, pos);
    auto aspect = acam_.cam_->Getaspect_ratio();
    int width =resolution_*aspect+0.5f;
    renderpipeline_.Init(&acam_, &dist_fbo_, width, resolution_,1);
    
    // acam_.cam_->distortion_.dist_type=SRender::LowRenderer::DistortionModel::INDEX;

    // acam_.cam_->distortion_.dist_para[0]=1.5;
    renderpipeline_.RenderTick();
    //SaveFbo("test");
    threadpool_.submit(SaveFbothreads,dist_fbo_.DownloadColor(),dist_fbo_.DownloadDepth(),dist_fbo_.buf_size_,ResourceManager::PathManager::GetFullPath(save_pth_),name);
    //RENDER undistort
    auto k = acam_.cam_->distortion_.dist_para[0];
    auto focal = acam_.cam_->Getfocal_length();
    auto sensorh = acam_.cam_->Getsensor_size_h();
    auto normr = sensorh*std::sqrt(1.0+aspect*aspect)/2.0/focal;
    auto alpha = 1.0/std::sqrt(1-k*normr*normr);

    acam_.cam_->Setsensor_size_h(alpha*sensorh);
    renderpipeline_.Init(&acam_, &dist_fbo_, width*alpha, resolution_*alpha,0);
    renderpipeline_.RenderTick();
    //SaveFbo("testori");
    threadpool_.submit(SaveFbothreads,dist_fbo_.DownloadColor(),dist_fbo_.DownloadDepth(),dist_fbo_.buf_size_,ResourceManager::PathManager::GetFullPath(save_pth_),name+"_ori");
    acam_.cam_->Setsensor_size_h(sensorh);

}

void DatasetGenComponent::LoadLensData(){
    auto& lens=lens_;
    lens.clear();
    std::ifstream input(ResourceManager::PathManager::GetFullPath(lensdata_pth_));
    if (input.fail()) return;
    std::string line;
    for (int i=0;i<4;++i) input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while(input.peek()!=EOF){
        for (int i=0;i<8;++i){
            if (i==3){
                float recv;
                float focal,k;
                for (int j=0;j<8;++j){
                    input>>recv;
                    if (j==1){
                        focal= recv;
                    }
                    if (j==7){
                        k=recv;
                    }
                }
                lens.push_back({focal,k});
                
            }
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
        }
    }
    

}

}