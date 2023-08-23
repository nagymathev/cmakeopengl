#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColors;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform float topHeight;
uniform float xMovement;

void main()
{
	gl_Position = vec4(aPos.x + xMovement, aPos.y * topHeight, aPos.z, 1.0);
	vertexColor = vec4(aPos.x + xMovement, aPos.y * topHeight, sin(xMovement * 4), 1.0);
	TexCoord = aTexCoord;
}