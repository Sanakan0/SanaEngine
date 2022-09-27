#VERTEX
#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 
layout (std140,binding = 0) uniform EngineUBO{
    mat4    ubo_PrjViewMat;
    vec3    ubo_ViewPos;
};
out VS_OUT{
    vec3 norm;
    vec3 pos;
} vs_out;

void main(){
    vs_out.norm = normal;
    vs_out.pos = pos;
    gl_Position = ubo_PrjViewMat*vec4(pos,1.0);
}

#FRAGMENT
#version 430 core
out vec4 FRAGMENT_COLOR;
in VS_OUT{
    vec3 norm;
    vec3 pos;
} fs_in;
const vec3 Lightpos=vec3(10,10,10);

void main(){
    vec3 lightdir = normalize(Lightpos-fs_in.pos);
    float cosa = dot(lightdir,fs_in.norm);
    vec3 color = vec3(0.5,0.5,0.5)+cosa*0.3;
    FRAGMENT_COLOR=vec4(color,1.0);
}