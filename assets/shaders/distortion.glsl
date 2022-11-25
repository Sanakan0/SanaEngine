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
uniform float k;
out VS_OUT{
    vec3 norm;
    vec3 pos;
} vs_out;

vec3 Get_Distorted(vec3 pos,float k){
    vec2 uvpos=pos.xy/pos.z;
    float r2 = uvpos.x*uvpos.x + uvpos.y*uvpos.y;
    float a = sqrt(1.0/(k*r2+1.0));
    pos.xy*=a;
    return pos;
}

void main(){
    vs_out.norm = mat3(ModelMat)*normal;
    gl_Position = ubo_PrjMat * vec4(Get_Distorted((ubo_ViewMat*ModelMat*vec4(pos,1.0)).xyz,k),1.0);
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
    FRAGMENT_COLOR=vec4(1,0,0,1.0);
}