#VERTEX
#version 430 core
#include "./distortionfunc.h"
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 


#include "./UBOlayout.h"

layout(location = 0) uniform mat4 ModelMat;

struct DistortionInfo{
    float dist_para[3];
    int dist_type;
};
uniform DistortionInfo DistInfo;

out VS_OUT{
    vec3 norm;
    vec3 world_pos;
    vec2 tex_coord;
} vs_out;


vec3 Get_Distorted(vec3 pos){
    switch(DistInfo.dist_type){
    case 1:
        return Distortion_Index(pos,DistInfo.dist_para[0]);    
    case 2:
        return Distortion_Poly3(pos,DistInfo.dist_para[0]);
    case 3:
        return Distortion_Poly5(pos,DistInfo.dist_para[0],DistInfo.dist_para[1]);
    case 4:
        return Distortion_Ptlens(pos,DistInfo.dist_para[0],DistInfo.dist_para[1],DistInfo.dist_para[2]);
    case 5:
        return Distortion_Division_inv(pos,DistInfo.dist_para[0]);    
    }
    return pos;
}


void main(){
    vs_out.norm = mat3(ModelMat)*normal;
    vs_out.tex_coord = tex_coord;
    vs_out.world_pos = (ModelMat*vec4(pos,1.0)).xyz;
    gl_Position = ubo_PrjMat * vec4(Get_Distorted((ubo_ViewMat*vec4(vs_out.world_pos,1.0)).xyz),1.0);
}

#FRAGMENT


#version 430 core
layout(binding = 0) uniform sampler2D diff_tex;
out vec4 FRAGMENT_COLOR;
in VS_OUT{
    vec3 norm;
    vec3 world_pos;
    vec2 tex_coord;
} fs_in;


uniform vec4 diffuse_color=vec4(1,1,1,1);



void main(){
    
    FRAGMENT_COLOR=texture(diff_tex,fs_in.tex_coord)*diffuse_color;
  
}

