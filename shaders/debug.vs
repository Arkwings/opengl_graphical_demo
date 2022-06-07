#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTex;

out vec2 tex;

void main()
{
    gl_Position = vec4(vPosition, 1.0f);
    tex = vTex;
}

