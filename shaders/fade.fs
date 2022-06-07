#version 330 core

uniform sampler2D samp;
uniform float fade;

in vec2 tex;

out vec4 color;

void main(){
	vec4 texel=texture(samp,tex);
	float f=fade*(sqrt(tex.x*tex.x+tex.y*tex.y)/2)-0.1;
	color=vec4(f,f,f,f*2.0+fade*1.5);
}