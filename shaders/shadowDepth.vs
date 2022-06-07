#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;

uniform mat4 M;
uniform mat4 lightSpaceMatrix;



void main(){
	gl_Position = lightSpaceMatrix * M *vec4(vPosition,1.0f);
}

