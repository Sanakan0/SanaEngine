#VERTEX
#version 430 core
#include "./distortionfunc.h"
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 

layout (std140,binding = 0) uniform EngineUBO{
    mat4 ubo_ViewMat;
    mat4 ubo_PrjMat;
    mat4 ubo_PrjViewMat;
    vec3 ubo_ViewPos;
};


uniform mat4 ModelMat;
uniform float k;
out VS_OUT{
    vec3 norm;
    vec3 pos;
    vec2 tex_coord;
} vs_out;


void main(){
    vs_out.norm = mat3(ModelMat)*normal;
    vs_out.tex_coord = tex_coord;
    gl_Position = ubo_PrjMat * vec4(Get_Distorted((ubo_ViewMat*ModelMat*vec4(pos,1.0)).xyz,k),1.0);
}

#FRAGMENT
#version 430 core
layout(binding = 0) uniform sampler2D diff_tex;
out vec4 FRAGMENT_COLOR;
in VS_OUT{
    vec3 norm;
    vec3 pos;
    vec2 tex_coord;
} fs_in;
const vec3 Lightpos=vec3(200,200,200);

void main(){
    vec3 ambcolor = vec3(texture(diff_tex,fs_in.tex_coord))*0.5;
    vec3 lightdir = normalize(Lightpos-fs_in.pos);
    vec3 normal =  normalize(fs_in.norm);
    float cosa = dot(lightdir,normal);
    vec3 color = ambcolor+cosa*0.3;
    FRAGMENT_COLOR=vec4(color,1.0);
}