#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 eyePos;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 lightSpaceMatrix;

out vec2 tex;

void main(){
	gl_Position = P * V * M * vec4(vPosition.x,vPosition.y,vPosition.z,1.0f);
	tex=vTex;
}
