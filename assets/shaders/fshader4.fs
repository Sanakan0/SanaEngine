#version 330 core
#define LIGHT_CNT 1
in vec3 norm,fragpos;
in vec2 texcoord;
out vec4 fragcolor;

uniform vec3 viewpos;

struct Material{
	sampler2D diff;
	sampler2D spec;
	float shininess;
};
struct Light{
	vec3 pos;
	vec3 amb,diff,spec;
	float cons,linear,quad;
	float cutoff,innercutoff;
	vec3 dir;
};
uniform Material material;
uniform Light light[LIGHT_CNT];

vec3 calclight(Light light, vec3 texcolor,vec3 norm,vec3 viewdir){
	vec3 lightdir = normalize(light.pos-fragpos);
	//attenu
	float dis = length(light.pos-fragpos);
	float attenuation = 1.0/(light.cons+light.linear*dis+light.quad*dis*dis);
	//ambdiffspec
	vec3 amb = texcolor*light.amb;
	float diff = max(dot(norm,lightdir),0);
	vec3 diffuse = light.diff*(diff*texcolor);
	vec3 refdir = reflect(-lightdir,norm);
	float spec = pow(max(dot(viewdir, refdir), 0.0), material.shininess);
	vec3 specular = light.spec * (spec* vec3(texture(material.spec,texcoord)));
	
	

	//spotlight
	float inten=1;
	float costheta = dot(-lightdir,normalize(light.dir));
	if (costheta<light.cutoff){
		inten=0;
	}else if (light.cutoff>-1){
		inten = clamp((costheta-light.cutoff)/(light.innercutoff-light.cutoff),0,1);
	}
	vec3 res=amb+(diffuse+specular)*inten;
	res*=attenuation;
	return res;
}


void main(){	
	vec3 texcolor = vec3(texture(material.diff,texcoord));
	vec3 norm = normalize(norm);
	vec3 viewdir = normalize(viewpos-fragpos);
	vec3 res=vec3(0,0,0);

	for (int i=0;i<LIGHT_CNT;i++){
		res+=calclight(light[i],texcolor,norm,viewdir);
	}
	fragcolor = vec4(res,1.0);

}
