#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include <SRender/Core/EntityRenderer.h>
#include <memory>

namespace SRender::Core{

void EntityRenderer::DrawSkeleton(Resources::SModel& model){
    glm::mat4 scale=  glm::scale(glm::mat4(1),glm::vec3(0.1));
    for (int i=0;i<model.GetJoints().size();++i){
        shapedrawer_.DrawArrow(model.palette_[i]*glm::inverse(model.GetJoints()[i].inverse_bind_mat)*scale);
    }
}



}