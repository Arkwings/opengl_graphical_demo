#version 330 core

uniform sampler2D skybox;

in vec2 tex;

out vec4 color;

void main(){
	vec4 texel=texture(skybox,tex);
	color=vec4(texel.rgba);
}