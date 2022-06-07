#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 V;
uniform mat4 P;

out vec2 tex;

void main(){
	gl_Position = ( P * vec4(vPosition.xy,vPosition.z-1,1.0f)).xyzw;
	tex=vPosition.xy;
}

