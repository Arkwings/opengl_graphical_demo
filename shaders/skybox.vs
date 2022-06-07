#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;
layout(location = 2) in vec2 vTex;

uniform mat4 V;
uniform mat4 P;

out vec2 tex;

void main(){
	gl_Position = (P * V * vec4(vPosition.x*1.4,vPosition.y*1.6-20,vPosition.z*1.4,1.0f)).xyzw;
	tex=vTex;
}

