#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include <SRender/Core/EntityRenderer.h>
#include <memory>

namespace SRender::Core{

void EntityRenderer::DrawSkeleton(Resources::SModel& model){
    glm::mat4 scale=  glm::scale(glm::mat4(1),glm::vec3(0.1));
    for (const auto& i:model.GetJoints()){
        shapedrawer_.DrawArrow(model.modelmat_*glm::inverse(i.inverse_bind_mat)*scale);
    }
}



}