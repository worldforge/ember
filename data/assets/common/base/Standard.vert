#version 330

in      vec4    vertex;
in      vec3    normal;
in      vec4    uv0;
in      vec3    tangent;

uniform mat4    viewProjMatrix;
uniform mat4    worldMatrix;

uniform vec3    eyePosition;   // object space

// Shadow view projection matrix
uniform mat4    shadowViewProjectionMatrix0;
uniform mat4    shadowViewProjectionMatrix1;
uniform mat4    shadowViewProjectionMatrix2;
uniform mat4    shadowViewProjectionMatrix3;
uniform mat4    shadowViewProjectionMatrix4;

out     vec4    oUv0;
out     vec3    oVNormal;
out     vec4    oVPos;
out     vec3    oVTangent;
out     vec3    oNormal;
out     vec3    oTangent;

// Shadow texture coordinates
out     vec4    oShadowTexCoord0;
out     vec4    oShadowTexCoord1;
out     vec4    oShadowTexCoord2;
out     vec4    oShadowTexCoord3;
out     vec4    oShadowTexCoord4;

out     vec3    oEyeDir;
out     mat4    oWorldMatrix;

void main(void)
{
	vec4 worldPos = worldMatrix * vertex;
	vec3 worldNorm = mat3(worldMatrix) * normal;

	gl_Position	= viewProjMatrix * worldPos;

	oUv0		= uv0;
	oNormal     = normal;
	oVNormal	= worldNorm;
	oVPos		= worldPos;
	oWorldMatrix= worldMatrix;
	oVTangent   = mat3(worldMatrix) * tangent;
	oTangent    = tangent;

	// Set coordinates on shadow textures
	oShadowTexCoord0 = shadowViewProjectionMatrix0 * worldPos;
	oShadowTexCoord1 = shadowViewProjectionMatrix1 * worldPos;
	oShadowTexCoord2 = shadowViewProjectionMatrix2 * worldPos;
	oShadowTexCoord3 = shadowViewProjectionMatrix3 * worldPos;
	oShadowTexCoord4 = shadowViewProjectionMatrix4 * worldPos;

    vec3 oEyeDir = eyePosition - vertex.xyz;
/*
    vec3 localbinormal = cross(tangent, normal);

	mat3 TBN = mat3(vec3(tangent[0], localbinormal[0], normal[0]),
						vec3(tangent[1], localbinormal[1], normal[1]),
						vec3(tangent[2], localbinormal[2], normal[2]));


    // Transform the light vector according to this matrix
    oEyeDir = normalize(TBN * eyeDir);
*/
}
