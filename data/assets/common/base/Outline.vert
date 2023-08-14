#version 330

uniform mat4 worldViewProj;

uniform float size_value;

in vec4 vertex;
in vec3 normal;
in vec4 uv0;

out vec4 oUv0;

void main()
{
    vec3 pos = vertex.xyz + (size_value * (1.0 + (1.0 / 15.0)) * normal);

    gl_Position = worldViewProj * vec4(pos, 1.0);
   	oUv0 = uv0;
}
