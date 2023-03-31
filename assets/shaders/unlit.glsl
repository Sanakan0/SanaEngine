#VERTEX
#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal; 

layout (std140,binding = 0) uniform EngineUBO{
    mat4 ubo_ViewMat; //0
    mat4 ubo_PrjMat; //64
    mat4 ubo_PrjViewMat; //128
    vec3 ubo_ViewPos; //192
    mat4 imgprj_PrjViewMat; //208
    int ubo_mask;//272

    
    
    
};


uniform mat4 ModelMat;

out VS_OUT{
    vec3 world_pos;
    vec2 tex_coord;
} vs_out;


void main(){
    vs_out.tex_coord = tex_coord;
    vs_out.world_pos =vec3(ModelMat*vec4(pos,1.0));
    gl_Position = ubo_PrjViewMat*vec4(vs_out.world_pos,1.0);
    
}

#FRAGMENT
#version 430 core
layout(binding = 0) uniform sampler2D diff_tex;
layout(binding = 5) uniform sampler2D img_depthtex;
layout(binding = 6) uniform sampler2D img_colortex;
out vec4 FRAGMENT_COLOR;
in VS_OUT{
    vec3 world_pos;
    vec2 tex_coord;
} fs_in;

layout (std140,binding = 0) uniform EngineUBO{
    mat4 ubo_ViewMat; //0
    mat4 ubo_PrjMat; //64
    mat4 ubo_PrjViewMat; //128
    vec3 ubo_ViewPos; //192
    mat4 imgprj_PrjViewMat; //208
    int ubo_mask;//272
    
    
    
};

uniform vec4 diffuse_color=vec4(1,1,1,1);

void calcimgprj(){
    if (ubo_mask==0)return; 
    vec4 tmppos = imgprj_PrjViewMat*vec4(fs_in.world_pos,1.0);
    vec3 ndccoordinimg = vec3(tmppos/tmppos.w);
    vec3 imgcoordinimg = (ndccoordinimg+1.0)*0.5;
    float depth = texture2D(img_depthtex,imgcoordinimg.xy).r;
    const float bias = 0.000005;

    if (imgcoordinimg.z<depth+bias){
        if (imgcoordinimg.x>1||imgcoordinimg.x<0||imgcoordinimg.y>1||imgcoordinimg.y<0) return;
        FRAGMENT_COLOR = texture2D(img_colortex,imgcoordinimg.xy)*diffuse_color;
        //FRAGMENT_COLOR = depth*diffuse_color;
    }
}

void main(){
    
    FRAGMENT_COLOR=texture(diff_tex,fs_in.tex_coord)*diffuse_color;
    calcimgprj();
}