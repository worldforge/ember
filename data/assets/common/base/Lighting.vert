#version 330
//
//  Lighting vertex shader
//  Copyright (C) 2009  Alexey Torkhov
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// Based on OGRE samples
// Some ideas were borrowed from Arseny Kapoulkine's blog - http://zeuxcg.blogspot.com/2007/10/my-own-lighting-shader-with-blackjack.html

/*

Explanation of preprocessor defines used:

SKINNING:           Hardware linear skinning should be used. The value specifies how many bones to use.
                    The vertex position if calculated by the number of bones using the
                    worldMatrix3x4Array uniform. Linear interpolation is used. blendIndices and blendWeights are used to
                    lookup bones and their weight.
SKINNING_DQ:        Hardware double quaternion skinning should be used. The value specifies how many bones to use.
                    The worldDualQuaternion2x4Array uniform specifies the quaternions.

INSTANCED           Hardware instancing is used. Must be used with either INSTANCED_HW or INSTANCED_SHADER.
INSTANCED_HW        Position data is obtained from the uv* attributes. The "uv" coords being used depends on how many UV coords already are in use.
                    The number specified correlated to the number of existing UV coords. We currently only support "1" and "2", but more can easily be added.
INSTANCED_SHADER    Position data is obtained from the worldMatrix3x4Array uniform. Position in the array
                    is obtained from the blendIndices attribute.

NORMAL_TEXTURE      Normals are not defined per vertex but instead in a normal map, to be used in the fragment shader.
                    This is mainly used for the terrain shaders, since because of LOD we don't want to use the vertex normals.

NORMAL_MAP          The fragment shader uses a normal map, and thus needs to have some extra values calculated, such as eyeDir
                    in tangent space.

SHADOW              Shadow textures are used.

TODO:
*   When using skinning, the final normals and tangents aren't correctly propagated to eyeNormal and TBN. Should be fixed.


*/

#include "common/base/DualQuaternion_Common.glsl"


// Type of fog
#if !defined(FOG_NONE) && !defined(FOG_LINEAR) && !defined(FOG_EXP) && !defined(FOG_EXP2)
#define FOG_NONE		1
#endif
#ifndef FOG_NONE
#define FOG_NONE		0
#endif
#ifndef FOG_LINEAR
#define FOG_LINEAR		0
#endif
#ifndef FOG_EXP
#define FOG_EXP			0
#endif
#ifndef FOG_EXP2
#define FOG_EXP2		0
#endif

// Number of lights
// 3 is resonable number to support
#ifndef NUM_LIGHTS
#define NUM_LIGHTS		3
#endif

// Either have shadows or not
// Supports 3 lights
#ifndef SHADOW
#define SHADOW			0
#endif

// Either use normal mapping or not
#ifndef NORMAL_MAP
#define NORMAL_MAP		0
#endif

// Number of parallax steps - works when normal mapping is on as it needs height map
// 3 is ok for small heights (scale < 0,05)
// 5-7 for bigger ones
#ifndef PARALLAX_STEPS
#define PARALLAX_STEPS	0
#endif

uniform mat4 viewProjMatrix;

uniform vec4 fogColour;
uniform vec4 fogParams; // density, linear start, linear end, 1/(end-start)

uniform vec4 lightAttenuation[NUM_LIGHTS]; // range, constant, linear, quadric
uniform vec4 lightPosition[NUM_LIGHTS];

///The number of active lights. This can be less than NUM_LIGHTS depending on the scene.
uniform float numberOfActiveLights;

#if INSTANCED && INSTANCED_SHADER
uniform vec4 worldMatrix3x4Array[240]; //240 = 80*3
#elif SKINNING_DQ
mat2x4 blendTwoWeights(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);
mat2x4 blendTwoWeightsAntipod(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);
mat2x4 blendThreeWeightsAntipod(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);
mat2x4 blendFourWeightsAntipod(vec4 blendWgt, vec4 blendIdx, vec4 dualQuaternions[80]);

vec3 calculateBlendPosition(vec3 position, mat2x4 blendDQ);
vec3 calculateBlendNormal(vec3 normal, mat2x4 blendDQ);

uniform vec4 worldDualQuaternion2x4Array[80];

#elif SKINNING
uniform vec4 worldMatrix3x4Array[240]; // 240/3 = 80 bones

#endif //INSTANCED && INSTANCED_SHADER

//With instanced methods, the world matrix will be derived from either the uv* attributes, or worldMatrix3x4Array
#if !INSTANCED
uniform mat4 worldMatrix;
#endif //!INSTANCED

#if WAVE
uniform float time;
uniform float frequency;
uniform vec4 direction;
#endif //WAVE

#if SHADOW
#define LINEAR_RANGE 0

#if INSTANCED
// Shadow view projection matrix
uniform mat4 shadowViewProjectionMatrix0;
uniform mat4 shadowViewProjectionMatrix1;
uniform mat4 shadowViewProjectionMatrix2;
uniform mat4 shadowViewProjectionMatrix3;
uniform mat4 shadowViewProjectionMatrix4;
#else
// Shadow world view projection matrix
uniform mat4 shadowWorldViewProjectionMatrix0;
uniform mat4 shadowWorldViewProjectionMatrix1;
uniform mat4 shadowWorldViewProjectionMatrix2;
uniform mat4 shadowWorldViewProjectionMatrix3;
uniform mat4 shadowWorldViewProjectionMatrix4;
#endif //INSTANCED

#if LINEAR_RANGE
uniform vec4 shadowDepthRange0;
uniform vec4 shadowDepthRange1;
uniform vec4 shadowDepthRange2;
uniform vec4 shadowDepthRange3;
uniform vec4 shadowDepthRange4;
#endif //LINEAR_RANGE

#endif //SHADOW



in vec4 vertex;
in vec4 blendIndices;
in vec4 blendWeights;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;
in vec4 uv0;
in vec4 uv1;
in vec4 uv2;
in vec4 uv3;
#if INSTANCED_HW > 1
in vec4 uv4;
#endif



#if !NORMAL_MAP
#if !NORMAL_TEXTURE
// Fragment position in eye space
out vec3 eyePosition;

// Normal in eye space
out vec3 eyeNormal;
#endif  // !NORMAL_TEXTURE

// Light attenuation, packed to vector
out vec3 attenuation;
//#define ATTENUATION_I attenuation[i]

#else // !NORMAL_MAP
// Normal+height map texture
//uniform sampler2D normalHeightMap;

#if PARALLAX_STEPS > 0
// Scale and bias for parallax
// Having scale = 0,05 - 0,1 provides BIG visual depth
// Bias = - scale/2  shifts texture equally by depth
// Bias = - scale    is also good to have holes or scratches on flat surface
uniform vec2 scaleBias;
#endif

// Light vector in tangent space (not normalized to keep linearity)
// 4-th component contains attenuation value
out vec4 lightDir[NUM_LIGHTS];
//#define ATTENUATION_I lightDir[i].w

// Eye direction in tangent space
out vec3 eyeDir;

#endif // NORMAL_MAP


#if NORMAL_TEXTURE
// Fragment position in object space
out vec3 positionObjSpace;
#endif // NORMAL_TEXTURE

out vec4 oUv0;

#if SHADOW

// Shadow texture coordinates
out vec4 shadowTexCoord0;
out vec4 shadowTexCoord1;
out vec4 shadowTexCoord2;
out vec4 shadowTexCoord3;
out vec4 shadowTexCoord4;

#endif //SHADOW

// Fog factor
out float fog;


void main()
{
	vec4 position = vertex;

#if WAVE
	if (uv0.y == 0.0)
	{
		position += direction * sin(time + vertex.x * frequency);
	}
#endif  //WAVE

#if INSTANCED

	mat4 worldMatrix;

#if INSTANCED_SHADER

	int idx = int(blendIndices[0]) * 3;
	worldMatrix[0] = worldMatrix3x4Array[idx];
	worldMatrix[1] = worldMatrix3x4Array[idx + 1];
	worldMatrix[2] = worldMatrix3x4Array[idx + 2];
	worldMatrix[3] = vec4( 0, 0, 0, 1 );
#elif INSTANCED_HW
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
#endif //INSTANCED_SHADER

    //this worldMatrix is transposed compared to non-instanced worldMatrix
	vec4 positionEyeSpace = position * worldMatrix;
	mat3 normalMatrix = inverse(mat3(worldMatrix));

#else
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

	//No need to normalize, the magnitude of the normal is preserved because only rotation is performed
	vec3 blendNormal = calculateBlendNormal(normal, blendDQ);   // This isn't used further down as it should be. I.e. eyeNormal and the TBN matrix should be
                                                              	//calculated using the adjusted vertex positions.

    mat3 normalMatrix = inverse(transpose(mat3(worldMatrix))); //This does not take the skinned position into account. Should be fixed.
#else
#if SKINNING
    vec3 blendPos = vec3(0.0, 0.0, 0.0);
	vec3 blendNorm = vec3(0.0, 0.0, 0.0);

	for (int bone = 0; bone < SKINNING; ++bone)
	{
		float weight = blendWeights[bone];
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

		mat3 worldRotMatrix = mat3(boneWorldMatrix[0].xyz, boneWorldMatrix[1].xyz, boneWorldMatrix[2].xyz);
		blendNorm += (normal * worldRotMatrix) * weight;
	}

	blendNorm = normalize(blendNorm);   // This isn't used further down as it should be. I.e. eyeNormal and the TBN matrix should be
	                                    //calculated using the adjusted vertex positions.
	vec4 positionEyeSpace = vec4(blendPos, 1);
    mat3 normalMatrix = inverse(transpose(mat3(worldMatrix))); //This does not take the skinned position into account. Should be fixed.

#else

	vec4 positionEyeSpace = worldMatrix * position;
	mat3 normalMatrix = inverse(transpose(mat3(worldMatrix)));

#endif //SKINNING
#endif //SKINNING_DQ
#endif //INSTANCED

    gl_Position			= viewProjMatrix * positionEyeSpace;


	oUv0 = uv0;


#if !NORMAL_MAP
#if !NORMAL_TEXTURE
	// Pass on position in eye space
	eyePosition = positionEyeSpace.xyz;

	// Transform normal to eye space
	eyeNormal = normalMatrix * normal;

#endif // !NORMAL_TEXTURE
#else // !NORMAL_MAP
	vec3 N = normalize(normalMatrix * normal);
	vec3 T = normalize(normalMatrix * tangent);
	// Seems that texture space is left-handed
	vec3 B = cross(T, N);
	//vec3 B = normalize(normalMatrix * attrBitangent);

	vec3 eye = -positionEyeSpace.xyz;

	// Multiply eye direction by TBN matrix to transform it to tangent space
	// eyeDir = mat3(T, B, N) * eye;
	eyeDir = normalize(vec3(dot(eye, T), dot(eye, B), dot(eye, N)));
#endif // NORMAL_MAP

#if NORMAL_TEXTURE
	// pass on the position in object space
	positionObjSpace = position.xyz;
#endif // NORMAL_TEXTURE

#if SHADOW

#if INSTANCED
	// Set coordinates on shadow textures
	shadowTexCoord0 = shadowViewProjectionMatrix0 * positionEyeSpace;
	shadowTexCoord1 = shadowViewProjectionMatrix1 * positionEyeSpace;
	shadowTexCoord2 = shadowViewProjectionMatrix2 * positionEyeSpace;
	shadowTexCoord3 = shadowViewProjectionMatrix3 * positionEyeSpace;
	shadowTexCoord4 = shadowViewProjectionMatrix4 * positionEyeSpace;
#else
	// Set coordinates on shadow textures
	shadowTexCoord0 = shadowWorldViewProjectionMatrix0 * position;
	shadowTexCoord1 = shadowWorldViewProjectionMatrix1 * position;
	shadowTexCoord2 = shadowWorldViewProjectionMatrix2 * position;
	shadowTexCoord3 = shadowWorldViewProjectionMatrix3 * position;
	shadowTexCoord4 = shadowWorldViewProjectionMatrix4 * position;
#endif


#if LINEAR_RANGE
	shadowTexCoord0.z = (shadowTexCoord0.z - shadowDepthRange0.x) * shadowDepthRange0.w;
	shadowTexCoord1.z = (shadowTexCoord1.z - shadowDepthRange1.x) * shadowDepthRange1.w;
	shadowTexCoord2.z = (shadowTexCoord2.z - shadowDepthRange2.x) * shadowDepthRange2.w;
	shadowTexCoord3.z = (shadowTexCoord3.z - shadowDepthRange3.x) * shadowDepthRange3.w;
	shadowTexCoord4.z = (shadowTexCoord4.z - shadowDepthRange4.x) * shadowDepthRange4.w;
#endif

	// Pass depth for PSSM
	oUv0.p = gl_Position.z;
#endif

//We can't use a loop here, since that will break on ATI cards

// TODO SK: rewrite without using built-in constants, reduce redundancy
#if NUM_LIGHTS > 0
	if (numberOfActiveLights > 0.0) {
		// Compute vector from surface to light position
		vec3 L = lightPosition[0].xyz - positionEyeSpace.xyz * lightPosition[0].w;

		float curAttenuation = 1.0;
		// Attenuation only for non-directional lights
		if (lightPosition[0].w > 0.0)
		{
			// Compute distance between surface and light position
			float d = length(L);

			// Compute attenuation per vertex
			curAttenuation =  1.0 / (lightAttenuation[0].y +
									 lightAttenuation[0].z * d +
									 lightAttenuation[0].w * d * d);
		}
#if !NORMAL_MAP
		attenuation[0] = curAttenuation;
#else
		// Multiply light direction by TBN matrix to transform it to tangent space
		// lightDir[i].xyz = mat3(T, B, N) * L;
		lightDir[0] = vec4(dot(L, T), dot(L, B), dot(L, N), curAttenuation);
#endif

	}
#endif

#if NUM_LIGHTS > 1
	if (numberOfActiveLights > 1.0) {
		// Compute vector from surface to light position
		vec3 L = lightPosition[1].xyz - positionEyeSpace.xyz * lightPosition[1].w;

		float curAttenuation = 1.0;
		// Attenuation only for non-directional lights
		if (lightPosition[1].w > 0.0)
		{
			// Compute distance between surface and light position
			float d = length(L);

			// Compute attenuation per vertex
			curAttenuation =  1.0 / (lightAttenuation[1].y +
                                     lightAttenuation[1].z * d +
                                     lightAttenuation[1].w * d * d);
		}
#if !NORMAL_MAP
		attenuation[1] = curAttenuation;
#else
		// Multiply light direction by TBN matrix to transform it to tangent space
		// lightDir[i].xyz = mat3(T, B, N) * L;
		lightDir[1] = vec4(dot(L, T), dot(L, B), dot(L, N), curAttenuation);
#endif

	}
#endif

#if NUM_LIGHTS > 2
	if (numberOfActiveLights > 2.0) {
		// Compute vector from surface to light position
		vec3 L = lightPosition[2].xyz - positionEyeSpace.xyz * lightPosition[2].w;

		float curAttenuation = 1.0;
		// Attenuation only for non-directional lights
		if (lightPosition[2].w > 0.0)
		{
			// Compute distance between surface and light position
			float d = length(L);

			// Compute attenuation per vertex
			curAttenuation =  1.0 / (lightAttenuation[2].y +
                                     lightAttenuation[2].z * d +
                                     lightAttenuation[2].w * d * d);
		}
#if !NORMAL_MAP
		attenuation[2] = curAttenuation;
#else
		// Multiply light direction by TBN matrix to transform it to tangent space
		// lightDir[i].xyz = mat3(T, B, N) * L;
		lightDir[2] = vec4(dot(L, T), dot(L, B), dot(L, N), curAttenuation);
#endif

	}
#endif



#if FOG_NONE
	fog = 1.0;
#else
	// Calculating fog factor per vertex, no visual difference with per fragment
	//If the fog colour alpha is null, we consider the fog disabled
	if (fogColour.a != 0.0)
	{
#if FOG_LINEAR
		fog = clamp((fogParams.z - gl_Position.z) * fogParams.w, 0.0, 1.0);
#endif
#if FOG_EXP
		fog = clamp(exp(- fogParams.x * gl_Position.z), 0.0, 1.0);
#endif
#if FOG_EXP2
		fog = clamp(exp(- fogParams.x * fogParams.x * gl_Position.z * gl_Position.z), 0.0, 1.0);
#endif
	}
	else
	{
		fog = 1.0;
	}
#endif

}
