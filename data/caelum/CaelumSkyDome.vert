#version 330

uniform mat4 worldViewProj;
uniform vec3 sunDirection;

in vec4 vertex;
in vec4 blendIndices;
in vec3 normal;
in vec4 uv0;

out vec4 oUv0;
out float incidenceAngleCos;
out float y;

void main()
{

    float cosine = dot(-normalize(sunDirection), normal);
    incidenceAngleCos = -cosine;

    y = -sunDirection.y;
    gl_Position = worldViewProj * vertex;
    oUv0 = uv0;

} // main end
