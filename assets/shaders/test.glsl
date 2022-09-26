#VERTEX
#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 
layout (std140,binding = 0) uniform EngineUBO{
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};
void main(){
    gl_Position = ubo_Projection*ubo_View*vec4(pos,1.0);
}

#FRAGMENT
#version 430 core
out vec4 FRAGMENT_COLOR;
void main(){
    FRAGMENT_COLOR=vec4(0.7,0.7,0.7,1.0);
}