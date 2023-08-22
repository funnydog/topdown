#version 330 core
in vec2 FragUV;
in vec4 FragColor;

uniform sampler2D Texture;

layout (location = 0) out vec4 OutColor;

void main()
{
	OutColor = FragColor * texture(Texture, FragUV.st);
}
