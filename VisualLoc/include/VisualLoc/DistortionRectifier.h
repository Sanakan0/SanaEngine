#include "SRender/LowRenderer/Camera.h"
#include <vector>
namespace VisualLoc{
using LinesT = std::vector<std::vector<std::pair<float,float>>>;
class DistortionRectifier{

void RectifyWithLines(LinesT& lines,float aspect_ratio,SRender::LowRenderer::DistortionModel dist){
    LinesT norm;
    for (auto& line:lines){
        norm.push_back({});
        for (auto& i:line){
            
        }
    }
}


};


}