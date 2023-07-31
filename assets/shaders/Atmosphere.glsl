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
    uint ubo_ctl_mask;//272
};



out vec3 worldpos;


void main(){
	gl_Position = vec4(pos,1.0);
    vec4 tmp = inverse(ubo_PrjViewMat)*vec4(pos,1);
    worldpos = tmp.xyz/tmp.w;
	//worldpos = vec3(inverse(ubo_PrjViewMat)*vec4(pos,1));
}


#FRAGMENT

#version 430 core
#define PII 3.141592653
#define M_e 2.718281828
#define BMR 3.2518262207319404e-05
#define BMG 4.891366975006311e-05
#define BMB 7.507490794050062e-05
in vec3 worldpos;
out vec4 fragcolor;

#include "./UBOlayout.h"

uniform vec3 sunDir;
uniform float earthR,atmosR,aerosolR;
uniform float betaR[3],betaM[3];
float calc_S(float theta,float r){
	return earthR*cos(theta) + sqrt(r*r - earthR*earthR * sin(theta)*sin(theta));
}

float calc_phi(float theta){
	return 3.0/(16*PII) * (1+cos(theta)*cos(theta));
}

float calc_phiM(float theta,float g){
	return (1.0-g)*(1.0-g)/(4*PII*pow(1+g*g-2*g*cos(theta),1.5));
}

float calc_RayliSC(float S,float PHI,float betaR){
    float betaSC = betaR;
    return (1.0-pow(M_e,-betaSC*S))*PHI;
}

float calc_RMSC(float S,float phiR,float phiM,float betaR,float betaM){

	return (phiR*betaR+phiM*betaM)/(betaR+betaM) * (1.0-pow(M_e,-(betaR+betaM)*S));
}
float lightL;
float calc_2level(float phiR,float phiM,float betaR,float betaM,float thetas){
	float ESUN = 20;
	
	float Satmo = calc_S(thetas,atmosR);
	float Saero = calc_S(thetas,aerosolR);
	float Sray = Satmo - Saero;
	float I_0 = calc_RayliSC(Sray,phiR,betaR);
	float res = I_0*pow(M_e,-(betaR+betaM)*Saero) + calc_RMSC(Saero,phiR,phiM,betaR,betaM);
	
	//ESUN*=pow(M_e,-betaR*Sray)*pow(M_e,-(betaR+betaM)*Saero);

	if (lightL<0) ESUN=0;
	else {
		ESUN*=pow(M_e,-betaR*lightL);
		//ESUN*=lightL;
	}
	
	return res*ESUN;
}

float sun_color(float betaR,float betaM,float thetas){
	float ESUN = 15;
	float Satmo = calc_S(thetas,atmosR);
	float Saero = calc_S(thetas,aerosolR);
	float Sray = Satmo - Saero;
	return ESUN*pow(M_e,-betaR*Sray)*pow(M_e,-(betaR+betaM)*Saero);
}

float calc_lightL(vec3 sundirout,vec3 lightdirin,float S){
	vec3 p2ecore = vec3(0,0,-earthR) + S*lightdirin;
	p2ecore = normalize(p2ecore);
	float theta = acos(dot(sundirout,p2ecore));
	float pth = cos(theta)*atmosR*2;
	float theta2e = acos(dot(sundirout,vec3(0,0,-1)));
	float Ssun = calc_S(theta2e,atmosR);

	if (pth<0){
		pth=0;
	}
	float dis = sin(theta)*atmosR;
	if (pth>0&&dis <= earthR){
		return -1;
	}
	//float theta2 = acos(dot(sundirout,-lightdirin));
	//if (theta2<PII/2) return 1;
	//return abs(PII-theta2)*2/PII;
	return (pth+Ssun)/2;
}

void main(){
	
	vec3 lightdir = normalize(ubo_ViewPos - worldpos);
	vec3 sundir = normalize(-sunDir);
	float theta2e = acos(dot(-lightdir,vec3(0,0,-1)));
	float theta2sun = acos(dot(sundir,lightdir));
	float S = calc_S(theta2e,atmosR);
	float phi = calc_phi(theta2sun);
	float phiM = calc_phiM(theta2sun,5);

	lightL = calc_lightL(-sundir,lightdir,S);

	float red = calc_2level(phi,phiM,betaR[0],betaM[0],theta2e);
	float green = calc_2level(phi,phiM,betaR[1],betaM[1],theta2e);
	float blue = calc_2level(phi,phiM,betaR[2],betaM[2],theta2e);
	fragcolor = vec4(red,green,blue,1.0);

	//fragcolor = vec4(calc_RayliSC(S,phi,betaR[0]),calc_RayliSC(S,phi,betaR[1]),calc_RayliSC(S,phi,betaR[2]),1);
	//fragcolor = vec4(calc_RMSC(S,phi,phiM,betaR[0],BMR),calc_RMSC(S,phi,phiM,betaR[1],BMG),calc_RMSC(S,phi,phiM,betaR[2],BMB),1);
	vec4 fragcolorsun;
	if (theta2sun<0.1) {
		red = sun_color(betaR[0],betaM[0],theta2e);
		green = sun_color(betaR[1],betaM[1],theta2e);
		blue = sun_color(betaR[2],betaM[2],theta2e);
		fragcolorsun = vec4(red,green,blue,1.0);
	
		if (theta2sun>0.02){
			fragcolor=mix(fragcolorsun,fragcolor,0.99 + 0.01*(theta2sun-0.02)/0.08);
		}else{
			fragcolor = fragcolorsun;
		}
	}
	theta2e-=PII/2;
	if (theta2e<0 ) fragcolor = vec4(0.25,0.21,0.19,1);
   
}
