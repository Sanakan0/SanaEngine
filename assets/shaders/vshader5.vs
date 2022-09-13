#version 330 core
layout(location=0) in vec3 apos;
layout(location=1) in vec3 anorm;
layout(location=2) in vec2 atexcoord;
uniform mat4 model,view,projection;
out vec3 norm,fragpos;
out vec2 texcoord;
void main(){
	gl_Position = projection*view*model*vec4(apos,1.0);
	norm = mat3(model)*anorm;
	fragpos = vec3(model*vec4(apos,1));
	texcoord = atexcoord;
}