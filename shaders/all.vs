#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

const int MAX_BONES=100;
const int MAX_DEP=4;


uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 eyePos;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 lightSpaceMatrix;
uniform mat4 gBones[MAX_BONES];


out vec3 norm;
out vec2 tex;
out vec3 fragPos;
out vec3 lPos;
out vec3 lColor;
out vec3 ePos;
out vec4 fragPosLightSpace;


void main(){
	mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
	for(int i=1;i<MAX_DEP;++i){
		BoneTransform += gBones[BoneIDs[i]] * Weights[i];
	}
    vec4 posL=BoneTransform * vec4(vPosition,1.0);
	gl_Position = P* V * M * posL;
	

	fragPos=vec3(M*vec4(posL));
	norm=transpose(inverse(mat3(M)))*vNorm;
	tex=vTex;
	lPos=lightPos;
	lColor=lightColor;
	ePos=eyePos;
	fragPosLightSpace=lightSpaceMatrix*vec4(fragPos,1.0);

}

