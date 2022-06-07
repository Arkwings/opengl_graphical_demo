#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 7) out;


in VS_OUT {
	float waveT;
	float waveW;
	float waveH;
	mat4 MVP;
}
gs_in[];




void main(){
	vec4 position=gl_in[0].gl_Position;
	gl_Position=gs_in[0].MVP*vec4(position.x,position.y+sin(gs_in[0].waveW * position.x*4 + gs_in[0].waveT) * cos(gs_in[0].waveW * position.z*4 + gs_in[0].waveT) * gs_in[0].waveH,position.z,1.0f);
	EmitVertex();

	position=(gl_in[2].gl_Position+gl_in[0].gl_Position)/2;
	gl_Position=gs_in[0].MVP*vec4(position.x,position.y+sin(gs_in[0].waveW * position.x*4 + gs_in[0].waveT) * cos(gs_in[0].waveW * position.z*4 + gs_in[0].waveT) * gs_in[0].waveH,position.z,1.0f);
	EmitVertex();

	position=(gl_in[0].gl_Position+gl_in[1].gl_Position)/2;
	gl_Position=gs_in[0].MVP*vec4(position.x,position.y+sin(gs_in[0].waveW * position.x*4 + gs_in[0].waveT) * cos(gs_in[0].waveW * position.z*4 + gs_in[0].waveT) * gs_in[0].waveH,position.z,1.0f);
	EmitVertex();

	position=(gl_in[1].gl_Position+gl_in[2].gl_Position)/2;
	gl_Position=gs_in[0].MVP*vec4(position.x,position.y+sin(gs_in[0].waveW * position.x*4 + gs_in[0].waveT) * cos(gs_in[0].waveW * position.z*4 + gs_in[0].waveT) * gs_in[0].waveH,position.z,1.0f);
	EmitVertex();

	position=gl_in[1].gl_Position;
	gl_Position=gs_in[0].MVP*vec4(position.x,position.y+sin(gs_in[0].waveW * position.x*4 + gs_in[0].waveT) * cos(gs_in[0].waveW * position.z*4 + gs_in[0].waveT) * gs_in[0].waveH,position.z,1.0f);
	EmitVertex();
	
	position=gl_in[2].gl_Position;
	gl_Position=gs_in[0].MVP*vec4(position.x,position.y+sin(gs_in[0].waveW * position.x*4 + gs_in[0].waveT) * cos(gs_in[0].waveW * position.z*4 + gs_in[0].waveT) * gs_in[0].waveH,position.z,1.0f);
	EmitVertex();

	position=(gl_in[0].gl_Position+gl_in[2].gl_Position)/2;
	gl_Position=gs_in[0].MVP*vec4(position.x,position.y+sin(gs_in[0].waveW * position.x*4 + gs_in[0].waveT) * cos(gs_in[0].waveW * position.z*4 + gs_in[0].waveT) * gs_in[0].waveH,position.z,1.0f);
	EmitVertex();

}