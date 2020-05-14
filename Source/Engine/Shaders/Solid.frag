#version 330 core

out vec4 outColor;
in vec4 primitiveColor;

void main()
{
	outColor = primitiveColor;
}