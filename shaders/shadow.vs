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

out VS_OUT 
{
	vec3 fragPos;
	vec3 norm;
	vec2 tex;
	vec3 lPos;
    vec3 lColor;
    vec3 ePos;
	vec4 fragPosLightSpace;
}
vs_out;


void main(){
	gl_Position = P * V * M * vec4(vPosition,1.0f);
	vs_out.fragPos=vec3(M*vec4(vPosition,1.0f));
	vs_out.norm=transpose(inverse(mat3(M)))*vNorm;
	vs_out.tex=vTex;
	vs_out.lPos=lightPos;
	vs_out.lColor=lightColor;
	vs_out.ePos=eyePos;
	vs_out.fragPosLightSpace=lightSpaceMatrix*vec4(vs_out.fragPos,1.0);
}

