vec3 Get_Distorted(vec3 pos,float k){
    vec2 uvpos=pos.xy/pos.z;
    float r2 = uvpos.x*uvpos.x + uvpos.y*uvpos.y;
    float a = sqrt(1.0/(k*r2+1.0));
    pos.xy*=a;
    return pos;
}
