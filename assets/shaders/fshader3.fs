#version 330 core
in vec3 norm,fragpos;
out vec4 fragcolor;

uniform vec3 viewpos;

struct Material{
	vec3 amb,diff,spec;
	float shininess;
};
struct Light{
	vec3 pos;
	vec3 amb,diff,spec;
};
uniform Material material;
uniform Light light;

void main(){
	vec3 amb = material.amb*light.amb;
	
	vec3 lightdir = normalize(light.pos-fragpos);
	vec3 norm = normalize(norm);
	float diff = max(dot(norm,lightdir),0);
	vec3 diffuse = light.diff*(diff*material.diff);

	
	vec3 viewdir = normalize(viewpos-fragpos);
	vec3 refdir = reflect(-lightdir,norm);
	float spec = pow(max(dot(viewdir, refdir), 0.0), material.shininess);
	vec3 specular = light.spec * (spec* material.spec);
	
	
	fragcolor = vec4(amb+diffuse+specular,1.0);

}