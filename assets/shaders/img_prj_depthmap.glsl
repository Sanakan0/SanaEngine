#VERTEX
#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 

layout (std140,binding = 0) uniform EngineUBO{
    mat4 ubo_ViewMat; //0
    mat4 ubo_PrjMat; //64
    mat4 ubo_PrjViewMat; //128
    vec3 ubo_ViewPos; //192
    mat4 imgprj_PrjViewMat; //208
    int ubo_mask;//272
};


uniform mat4 ModelMat;




void main(){
    gl_Position = imgprj_PrjViewMat*ModelMat*vec4(pos,1.0);
}

#FRAGMENT
#version 430 core

layout(location = 0) out float FRAGMENT_DEPTH;

void main(){
    FRAGMENT_DEPTH=gl_FragCoord.z;
}