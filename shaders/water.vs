#version 330 core

layout(location = 0) in vec3 vPosition;

uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 eyePos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out float height;

void main(){
	vec4 position = vec4(vPosition.x,vPosition.y+sin(waveWidth * vPosition.x*4 + waveTime) * cos(waveWidth * vPosition.z*4 + waveTime) * waveHeight,vPosition.z,1.0f);
	position=vec4(position.x,position.y,position.z,1.0);
	gl_Position = P * V * M * position;
	height = position.y;
}