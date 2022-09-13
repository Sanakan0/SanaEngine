#version 330 core
in vec2 texpos;
out vec4 fragcolor;
uniform sampler2D tex;

void main(){
	fragcolor = texture(tex,texpos);
}