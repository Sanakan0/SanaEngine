#VERTEX
#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 
layout(location = 3) in uvec4 joint_idx;
layout(location = 4) in vec4 joint_weight;
layout (std140,binding = 0) uniform EngineUBO{
    mat4    ubo_PrjViewMat;
    vec3    ubo_ViewPos;
};

layout(std140,binding=0)buffer JointMatSSBO{
    mat4 joint_Mat[];
};


uniform mat4 ModelMat;
out VS_OUT{
    vec3 norm;
    vec3 pos;
} vs_out;

mat4 blend_mat(){
    mat4 res=mat4(0);
    for (int i=0;i<4;++i){
        res+=joint_Mat[joint_idx[i]]*joint_weight[i];
    }
    return res;
}


void main(){
    mat4 animat=blend_mat();
    vs_out.norm = mat3(animat)*normal;
    vs_out.pos = (animat*vec4(pos,1)).xyz;
    gl_Position = ubo_PrjViewMat*vec4(vs_out.pos,1.0);
}

#FRAGMENT
#version 430 core
out vec4 FRAGMENT_COLOR;
in VS_OUT{
    vec3 norm;
    vec3 pos;
} fs_in;
const vec3 Lightpos=vec3(200,200,200);

void main(){
    vec3 lightdir = normalize(Lightpos-fs_in.pos);
    vec3 normal =  normalize(fs_in.norm);
    float cosa = dot(lightdir,normal);
    vec3 color = vec3(0.5,0.5,0.5)+cosa*0.3;
    FRAGMENT_COLOR=vec4(color,1.0);
}