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
        return Distortion_Division(pos,DistInfo.dist_para[0]);    
    }
    return pos;
}



const vec3 panel[4]=vec3[4]{
    vec3(1,1,0),
    vec3(-1,1,0),
    vec3(-1,-1,0),
    vec3(1,-1,0)
};

const int panelidx[6]=int[6]{0,1,2,2,3,0};

void main(){
    gl_Position = vec4(panel[panelidx[gl_VertexID]],1);
}

#FRAGMENT


#version 430 core
layout(binding = 0) uniform sampler2D dist_img;
out vec4 FRAGMENT_COLOR;

uniform vec4 diffuse_color=vec4(1,1,1,1);

uniform float aspect_ratio=1;
uniform float norm_fh=1;

vec2 uv2norm(vec2 pos){
    return vec2(pos.x*aspect_ratio,pos.y)/norm_fh;
}
vec2 norm2uv(vec2 pos){
    pos*=norm_fh;
    return vec2(pos.x/aspect_ratio,pos.y);
}


void main(){
    gl_FragCoord.xy
    FRAGMENT_COLOR=texture(diff_tex,fs_in.tex_coord)*diffuse_color;
  
}

