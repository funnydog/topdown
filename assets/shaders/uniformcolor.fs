#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 uniformColor;

void main()
{
	color = uniformColor * texture(image, TexCoords);
}
