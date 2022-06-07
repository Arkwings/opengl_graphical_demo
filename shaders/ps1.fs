#version 330 core

uniform int type;
uniform float appear;
uniform float effect;

in vec3 pos;

out vec4 color;



void main(){
	if(type==0){       //background
		color=vec4(mix(vec3(0.0,0.0,0.0),vec3(1.0,1.0,1.0),appear),1.0);
	}
	else if(type==1){  //next 1
		color=vec4(mix(vec3(0.0,0.0,0.0),vec3(1.0,1.0-abs(pos.x),0.0),appear),1.0);
	}
	else if(type==2){  //next 1
		color=vec4(mix(vec3(0.0,0.0,0.0),vec3(1.0,1.0-abs(pos.x)*effect*20/9,0.0),appear),1.0);
	}
}