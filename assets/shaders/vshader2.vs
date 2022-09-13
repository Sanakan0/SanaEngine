#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec2 atexpos;

out vec2 texpos;

uniform mat4 model,view,projection;
void main(){
	gl_Position = projection*view*model*vec4(apos,1.0);
	texpos=atexpos;
}