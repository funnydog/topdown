#version 330 core

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec4 Color;

uniform mat4 Projection;

out vec2 FragUV;
out vec4 FragColor;

void main()
{
	FragUV = UV;
	FragColor = Color;
	gl_Position = Projection * vec4(Position, 0, 1);
}
