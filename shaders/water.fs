#version 330 core

in float height;

out vec4 fragColor;

void main(){

	vec4 texColor=vec4(0.0,0.0,1.5,0.05);

	//AMBIANTE
    float aPower=0.15f;  //set vers 0.15 quand non seul 
    vec3 a=aPower* (height*2+texColor.xyz);
    
    vec3 lighting= a * texColor.rgb;

	fragColor=vec4(lighting,texColor.a);
}