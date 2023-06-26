#VERTEX
#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 


#include "./UBOlayout.h"

layout(location = 0) uniform mat4 ModelMat;


void main(){
    gl_Position = vec4(pos,1);
}

#FRAGMENT


#version 430 core
layout(binding = 0) uniform sampler2D dist_img;
out vec4 FRAGMENT_COLOR;

struct DistortionInfo{
    float dist_para[3];
    int dist_type;
};
uniform DistortionInfo DistInfo;


uniform vec4 diffuse_color=vec4(1,1,1,1);


uniform float norm_fh=1;
uniform float picwidth;
uniform float picheight;
uniform float sensorscale=1;//  undistsensor size/realsensor size


#include "./distortionfunc.h"
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
    vec3 pos = vec3(gl_FragCoord.x-picwidth/2,gl_FragCoord.y-picheight/2,picheight/sensorscale*norm_fh);
    vec3 dpos = Get_Distorted(pos);
    
    //cvt fragpos 2 uvpos
    float distpicwidth = picwidth/sensorscale;
    float distpicheight = picheight/sensorscale;
    vec2 uv_on_dist = vec2((dpos.x+distpicwidth/2)/distpicwidth,(dpos.y+distpicheight/2)/distpicheight);
    if (uv_on_dist.x>=1||uv_on_dist.x<=0||uv_on_dist.y>=1||uv_on_dist.y<=0||isnan(uv_on_dist.x)){
        FRAGMENT_COLOR=vec4(0,0,0,0);
    }
    else FRAGMENT_COLOR=texture(dist_img,uv_on_dist)*diffuse_color;
  
}

