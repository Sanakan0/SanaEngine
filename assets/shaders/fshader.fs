#version 330 core
in vec3 outC;
out vec4 FragColor;
void main()
{
   FragColor = vec4(outC,1.0);
}
