#version 330

uniform mat4 worldViewProj;
uniform mat4 worldMatrix;

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
    //Get the UV for the texture from the world horizontal coords.
	oUv0 = worldMatrix * vertex * 0.15;
	oUv0.y = oUv0.z;
	oColour = colour;
	oNormal = normal;
}
