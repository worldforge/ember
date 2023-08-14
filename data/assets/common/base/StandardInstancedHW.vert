#version 330

in      vec4    vertex;
in      vec3    normal;
in      vec4    uv0;
in      vec4    uv1;
in      vec4    uv2;
in      vec4    uv3;
#if NUM_UVS > 1
//layout(location = 12) in      vec4    uv4;
layout(location = 12) in      vec4    uv4;
#endif
in      vec3    tangent;

uniform mat4    viewProjMatrix;

uniform vec4    eyePosition;// world space

// Shadow view projection matrix
uniform mat4    shadowViewProjectionMatrix0;
uniform mat4    shadowViewProjectionMatrix1;
uniform mat4    shadowViewProjectionMatrix2;
uniform mat4    shadowViewProjectionMatrix3;
uniform mat4    shadowViewProjectionMatrix4;

out     vec4    oUv0;
#if NUM_UVS > 1
out     vec4    oUv1;
#endif
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
    mat4 worldMatrix;

    #if NUM_UVS == 1
    worldMatrix[0] = uv1;
    worldMatrix[1] = uv2;
    worldMatrix[2] = uv3;
    #elif NUM_UVS == 2
    worldMatrix[0] = uv2;
    worldMatrix[1] = uv3;
    worldMatrix[2] = uv4;
    #endif
    worldMatrix[3] = vec4(0, 0, 0, 1);

    vec4 worldPos = vertex * worldMatrix;
    vec3 worldNorm = normal * mat3(worldMatrix);

    gl_Position    = viewProjMatrix * worldPos;

    oUv0        = uv0;
    #if NUM_UVS > 1
    oUv1        = uv1;
    #endif
    oVNormal    = worldNorm;
    oVPos        = worldPos;
    oWorldMatrix= worldMatrix;
    oVTangent   = mat3(worldMatrix) * tangent;
    oNormal     = normal;
    oTangent    = tangent;

    // Set coordinates on shadow textures
    oShadowTexCoord0 = shadowViewProjectionMatrix0 * worldPos;
    oShadowTexCoord1 = shadowViewProjectionMatrix1 * worldPos;
    oShadowTexCoord2 = shadowViewProjectionMatrix2 * worldPos;
    oShadowTexCoord3 = shadowViewProjectionMatrix3 * worldPos;
    oShadowTexCoord4 = shadowViewProjectionMatrix4 * worldPos;


    vec3 eyePositionObjectSpace = (eyePosition * inverse(worldMatrix)).xyz;

    vec3 oEyeDir = eyePositionObjectSpace - vertex.xyz;

    /*
        vec3 localbinormal = cross(tangent, normal);

        mat3 TBN = mat3(vec3(tangent[0], localbinormal[0], normal[0]),
                            vec3(tangent[1], localbinormal[1], normal[1]),
                            vec3(tangent[2], localbinormal[2], normal[2]));


        // Transform the light vector according to this matrix
        oEyeDir = normalize(TBN * eyeDir);
    */
}
