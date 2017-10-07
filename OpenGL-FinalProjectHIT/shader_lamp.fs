#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 colorTint;
uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords) * vec4(colorTint ,1.0f);
}