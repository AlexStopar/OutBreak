#version 430 core
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D image;

void main()
{
    color = mix(texture(image, TexCoords), ParticleColor, 0.5);
	color = mix(color, vec4(1.0, 0.0, 0.0, 1.0), 0.4);
}  