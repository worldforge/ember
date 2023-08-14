#version 330

uniform mat4 worldViewProj;

in vec4 vertex;
in vec4 uv0;
in vec4 colour;
in vec3 normal;

out vec4 oUv0;
out vec4 oColour;
out vec3 oNormal;

void main()
{
    gl_Position = worldViewProj * vertex;
	oUv0 = uv0;
	oColour = colour;
	oNormal = normal;
}
