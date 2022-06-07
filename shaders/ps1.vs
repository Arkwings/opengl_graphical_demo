#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 pos;


void main(){
	vec4 position = ( P * V * M * vec4(vPosition.xyz,1.0f)).xyzw;
	gl_Position=position;
	pos=position.xyz;
}

