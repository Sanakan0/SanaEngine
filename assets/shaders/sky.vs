#version 330 core
layout(location = 0) in vec3 apos;

out vec3 worldpos;
uniform mat4 mvp;

void main(){
	gl_Position = mvp*vec4(apos,1);
	worldpos = apos;
}