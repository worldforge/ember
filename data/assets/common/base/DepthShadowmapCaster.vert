#version 330

#include "common/base/DualQuaternion_Common.glsl"

in vec4 vertex;
in vec4 blendIndices;
in vec4 blendWeights;

uniform vec4 texelOffsets;

#if LINEAR_RANGE
uniform vec4 depthRange;
#endif //LINEAR_RANGE
#if ALPHA
out vec2 oUv0;
//This attribute is automatically populated by Ogre, with the UV of the first texture
in vec4 uv0;
#endif //ALPHA

out vec2 depth;

#if INSTANCED

uniform mat4 viewProjMatrix;

#if INSTANCED_SHADER
uniform vec4 worldMatrix3x4Array[240]; //240 = 80*3
#endif //INSTANCED_SHADER

#if INSTANCED_HW
in vec4 uv1;
in vec4 uv2;
in vec4 uv3;
#endif //INSTANCED_HW
#if INSTANCED_HW > 1
in vec4 uv4;
#endif

#else //INSTANCED

#if SKINNING || SKINNING_DQ

uniform mat4 viewProjMatrix;


#if SKINNING_DQ
mat2x4 blendTwoWeights(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);
mat2x4 blendTwoWeightsAntipod(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);
mat2x4 blendThreeWeightsAntipod(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);
mat2x4 blendFourWeightsAntipod(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);

vec3 calculateBlendPosition(vec3 position, mat2x4 blendDQ);
vec3 calculateBlendNormal(vec3 normal, mat2x4 blendDQ);

uniform vec4 worldDualQuaternion2x4Array[80];
#elif SKINNING
uniform vec4 worldMatrix3x4Array[240]; // 240/3 = 80 bones
#endif //SKINNING_DQ

#else //SKINNING || SKINNING_DQ
uniform mat4 worldViewProj;

#endif //SKINNING || SKINNING_DQ
#endif //INSTANCED



void main()
{

#if INSTANCED
	mat4 worldMatrix;

#if INSTANCED_SHADER

	int idx = int(blendIndices[0]) * 3;
	worldMatrix[0] = worldMatrix3x4Array[idx];
	worldMatrix[1] = worldMatrix3x4Array[idx + 1];
	worldMatrix[2] = worldMatrix3x4Array[idx + 2];
	worldMatrix[3] = vec4( 0, 0, 0, 1 );
#endif

#if INSTANCED_HW
    #if INSTANCED_HW == 1
    worldMatrix[0] = uv1;
    worldMatrix[1] = uv2;
    worldMatrix[2] = uv3;
    #elif INSTANCED_HW == 2
    worldMatrix[0] = uv2;
    worldMatrix[1] = uv3;
    worldMatrix[2] = uv4;
    #endif
    worldMatrix[3] = vec4( 0.0, 0.0, 0.0, 1.0 );
#endif

    vec4 worldPos = vertex * worldMatrix;
    gl_Position    = viewProjMatrix * worldPos;

#else


#if SKINNING

#if SKINNING_DQ
    #if SKINNING_DQ == 1
	mat2x4 blendDQ = blendWeights.x*mat2x4(worldDualQuaternion2x4Array[int(blendIndices.x) * 2], worldDualQuaternion2x4Array[int(blendIndices.x) * 2 + 1]);
    #elif SKINNING_DQ == 2
	mat2x4 blendDQ = blendTwoWeightsAntipod(blendWeights, blendIndices, worldDualQuaternion2x4Array);
    #elif SKINNING_DQ == 3
	mat2x4 blendDQ = blendThreeWeightsAntipod(blendWeights, blendIndices, worldDualQuaternion2x4Array);
    #elif SKINNING_DQ == 4
	mat2x4 blendDQ = blendFourWeightsAntipod(blendWeights, blendIndices, worldDualQuaternion2x4Array);
    #endif //SKINNING_DQ == 1

	float len = length(blendDQ[0]);
	blendDQ /= len;

	vec4 positionEyeSpace = vec4(calculateBlendPosition(vertex.xyz, blendDQ), 1);

#else
    vec3 blendPos = vec3(0.0, 0.0, 0.0);

    for (int bone = 0; bone < SKINNING; ++bone)
	{
		float weight = blendWeights[bone];
		if (weight == 0.0) {
		    continue;
		}
		// perform matrix multiplication manually since no 3x4 matrices
        // ATI GLSL compiler can't handle indexing an array within an array so calculate the inner index first
		int idx = int(blendIndices[bone]) * 3;
        // ATI GLSL compiler can't handle unrolling the loop so do it manually
        // ATI GLSL has better performance when mat4 is used rather than using individual dot product
        // There is a bug in ATI mat4 constructor (Cat 7.2) when indexed uniform array elements are used as vec4 parameter so manually assign
		mat4 boneWorldMatrix;
		boneWorldMatrix[0] = worldMatrix3x4Array[idx];
		boneWorldMatrix[1] = worldMatrix3x4Array[idx + 1];
		boneWorldMatrix[2] = worldMatrix3x4Array[idx + 2];
		boneWorldMatrix[3] = vec4(0);
		// now weight this into final
		blendPos += (vertex * boneWorldMatrix).xyz * weight;
	}

	vec4 positionEyeSpace = vec4(blendPos, 1);
#endif

    gl_Position    = viewProjMatrix * positionEyeSpace;
#else

    // Pass-through default components
	gl_Position    = worldViewProj * vertex;

#endif

#endif




#if ALPHA
	oUv0 = uv0.st;
#endif

	// fix pixel / texel alignment
	gl_Position.xy += texelOffsets.zw * gl_Position.w;
	// linear depth storage
	// offset / scale range output
#if LINEAR_RANGE
	depth.x = (gl_Position.z - depthRange.x) * depthRange.w;
#else
	depth.x = gl_Position.z;
#endif
	depth.y = gl_Position.w;
}

