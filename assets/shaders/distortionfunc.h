vec3 Distortion_Index(vec3 pos,float k){
    vec2 normpos=pos.xy/pos.z;
    float ru2 = normpos.x*normpos.x + normpos.y*normpos.y;
    float alpha = sqrt(1.0/(k*ru2+1.0));
    pos.xy*=alpha;
    return pos;
}

vec3 Distortion_Poly3(vec3 pos,float k1){
    vec2 normpos=pos.xy/pos.z;
    float ru2 = normpos.x*normpos.x + normpos.y*normpos.y;
    float alpha = 1.0-k1+k1*ru2;
    pos.xy*=alpha;
    return pos;
}

vec3 Distortion_Poly5(vec3 pos,float k1,float k2){
    vec2 normpos=pos.xy/pos.z;
    float ru2 = normpos.x*normpos.x + normpos.y*normpos.y;
    float alpha = 1.0+k1*ru2+k2*ru2*ru2;
    pos.xy*=alpha;
    return pos;
}

vec3 Distortion_Ptlens(vec3 pos,float a,float b,float c){
    vec2 normpos=pos.xy/pos.z;
    float ru2 = normpos.x*normpos.x + normpos.y*normpos.y;
    float ru = sqrt(ru2);
    float alpha = 1.0-a-b-c+a*ru2*ru+b*ru2+c*ru;
    pos.xy*=alpha;
    return pos;
}

vec3 Distortion_Division(vec3 pos,float k){ // change
    vec2 normpos=pos.xy/pos.z;
    float ru2 = normpos.x*normpos.x + normpos.y*normpos.y;
    float a = 1.0/(k*ru2+1.0);
    pos.xy*=a;
    return pos;
}

