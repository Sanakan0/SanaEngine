#VERTEX
#version 430 core

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

out VS_OUT{
    vec3 pos;
    vec2 tex_coord;
} vs_out;


void main(){
    vs_out.tex_coord = tex_coord;
    gl_Position = ubo_PrjViewMat*ModelMat*vec4(pos,1.0);
}

#FRAGMENT
#version 430 core
layout(binding = 0) uniform sampler2D diff_tex;
out vec4 FRAGMENT_COLOR;
in VS_OUT{
    vec3 pos;
    vec2 tex_coord;
} fs_in;


void main(){
    FRAGMENT_COLOR=vec4(vec3(texture(diff_tex,fs_in.tex_coord)),1.0);
}