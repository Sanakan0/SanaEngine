#VERTEX
#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 

#include "./UBOlayout.h"

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