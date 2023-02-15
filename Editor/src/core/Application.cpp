#include "SEditor/Core/Application.h"
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cuda.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
namespace SEditor::Core{

Application::Application():editor_(rtcontext_){}

Application::~Application(){}

void Application::Run(){
    spdlog::info("[SANAEngine] Engine start!");
    cv::cuda::printCudaDeviceInfo(0);
    editor_.Run();
}

}