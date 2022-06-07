#version 330 core

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;

uniform bool shadows;
uniform bool dID;
uniform int gemColor;

in vec2 tex;

out vec4 fragColor;

void main(){
	vec4 texColor = texture(diffuseTexture,tex).rgba;
	if(texColor.a<0.01)discard;
	fragColor= texColor;
}