#include<SEditor/Core/Application.h>
#include <iterator>
#include<windows.h>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cuda.hpp>
int main(void){
    //cuda test
    int num_devices = cv::cuda::getCudaEnabledDeviceCount();
	int gpu_device_id=0;
    if (num_devices > 0)
	{
		for (int i = 0; i < num_devices; i++)
		{
			cv::cuda::DeviceInfo dev_info(i);
			if (dev_info.isCompatible()) 
			{
                gpu_device_id = i;
			}
		}
	}
	cv::cuda::setDevice(gpu_device_id);
    cv::cuda::printCudaDeviceInfo(0);
    cv::Mat tmp = cv::imread(R"(C:\Users\China\Pictures\Saved Pictures\94704615_p0.jpg)");

    cv::cuda::GpuMat gpu_frame,tmp_frame;
    gpu_frame.upload(tmp);
    
    cv::cuda::cvtColor(gpu_frame,gpu_frame,cv::COLOR_RGB2BGR);
    cv::cuda::threshold(gpu_frame, gpu_frame, 105, 255, cv::THRESH_BINARY_INV);
    cv::cuda::resize(gpu_frame, gpu_frame, {720,720*gpu_frame.size().height/gpu_frame.size().width});
    gpu_frame.download(tmp);
    cv::namedWindow("tmp");
    cv::imshow("tmp",tmp);
    cv::waitKey();

    SEditor::Core::Application app;
    app.Run();
}
