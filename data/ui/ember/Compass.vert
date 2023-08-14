#version 330

uniform mat4 worldViewProj;
uniform mat4 textureMatrix0;
uniform mat4 textureMatrix1;

in vec4 vertex;
in vec4 uv0;
in vec4 uv1;

out vec4 oUv0;
out vec4 oUv0Transformed;
out vec4 oUv1Transformed;

void main()
{
    gl_Position = worldViewProj * vertex;
	oUv0 = uv0;
	oUv0Transformed = textureMatrix0 * uv0;
}
