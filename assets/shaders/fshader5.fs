#version 330 core
in vec3 norm,fragpos;
in vec2 texcoord;
out vec4 fragcolor;

uniform vec3 viewpos;

struct Material{
	sampler2D diff0,diff1;
	sampler2D spec0,spec1;
	float shininess;
};
struct Light{
	vec3 pos;
	vec3 amb,diff,spec;
};
uniform Material material;
uniform Light light;

void main(){
	vec3 texcolor = vec3(texture(material.diff0,texcoord));
	vec3 amb = texcolor*light.amb;
	
	vec3 lightdir = normalize(light.pos-fragpos);
	vec3 norm = normalize(norm);
	float diff = max(dot(norm,lightdir),0);
	vec3 diffuse = light.diff*(diff*texcolor);

	
	vec3 viewdir = normalize(viewpos-fragpos);
	vec3 refdir = reflect(-lightdir,norm);
	float spec = pow(max(dot(viewdir, refdir), 0.0), material.shininess);
	vec3 specular = light.spec * (spec* vec3(texture(material.spec0,texcoord)));
	
	
	fragcolor = vec4(amb+diffuse+specular,1.0);
	//fragcolor = vec4(1,0,0,1);
}