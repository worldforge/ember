#version 330
/*
	Lighting fragment shader
	Copyright (C) 2009  Alexey Torkhov

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Based on OGRE samples
// Some ideas were borrowed from Arseny Kapoulkine's blog - http://zeuxcg.blogspot.com/2007/10/my-own-lighting-shader-with-blackjack.html

// Conditional defines are disabled because of bug in OGRE 1.6.0 GLSL preprocessor - http://www.ogre3d.org/forums/viewtopic.php?f=4&t=46837

// Number of lights
// 3 is resonable number to support
//#ifndef NUM_LIGHTS
#define NUM_LIGHTS	3
//#endif

// Either have shadows or not
// Supports 3 lights
//#ifndef SHADOW
// #define SHADOW			1
//#endif

#define PCF				1

// Either use normal mapping or not
//#ifndef NORMAL_MAP
//#define NORMAL_MAP		0
//#endif

// Number of parallax steps - works when normal mapping is on as it needs height map
// 3 is ok for small heights (scale < 0,05)
// 5-7 for bigger ones
//#ifndef PARALLAX_STEPS
//#define PARALLAX_STEPS	0
//#endif

// Either show specular highlights or not
// Could be disabled for better speed
//#ifndef SPECULAR
//#define SPECULAR		0
//#endif

// Other formula for calculating specular, could save few instructions
//#ifndef SPECULAR_PHONG
#define SPECULAR_PHONG	0
//#endif

// Eye is at (0, 0, 0) or at (0, 0, infinity) in eye space, could save few instructions when not in normal mapping mode
//#ifndef EYE_AT_ORIGIN
#define EYE_AT_ORIGIN	1
//#endif


// Fog factor
in float fog;

///The number of active lights. This can be less than NUM_LIGHTS depending on the scene.
uniform float numberOfActiveLights;

// Diffuse texture
uniform sampler2D diffuseMap;

#if SPECULAR || SPECULAR_PHONG
// Specular texture
uniform sampler2D specularMap;
#endif

#if !NORMAL_MAP
// Fragment position in eye space
in vec3 eyePosition;

// Normal in eye space
in vec3 eyeNormal;

// Light attenuation, packed to vector
in vec3 attenuation;
//#define ATTENUATION_I attenuation[i]

#else // NORMAL_MAP

// Normal+height map texture
uniform sampler2D normalHeightMap;

#if PARALLAX_STEPS > 0
// Scale and bias for parallax
// Having scale = 0,05 - 0,1 provides big visual depth
// Bias = - scale/2  shifts texture equally by depth
// Bias = - scale    is also good to have holes or scratches on flat surface
uniform vec2 scaleBias;
#endif

// Light vector in tangent space (not normalized to keep linearity)
// 4-th component contains attenuation value
in vec4 lightDir[NUM_LIGHTS];
//#define ATTENUATION_I lightDir[i].w

// Eye direction in tangent space
in vec3 eyeDir;
#endif // NORMAL_MAP

#if SHADOW
#define LINEAR_RANGE 0

// Shadow maps
uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D shadowMap3;
uniform sampler2D shadowMap4;

uniform vec4 inverseShadowMapSize0;
uniform vec4 inverseShadowMapSize1;
uniform vec4 inverseShadowMapSize2;
uniform vec4 inverseShadowMapSize3;
uniform vec4 inverseShadowMapSize4;

uniform float fixedDepthBias;
uniform float gradientClamp;
uniform float gradientScaleBias;

//uniform vec4 pssmSplitPoints;
const vec4 pssmSplitPoints = vec4(1.0, 15.0, 50.0, 200.0);

// Shadow texture coordinates
in vec4 shadowTexCoord0;
in vec4 shadowTexCoord1;
in vec4 shadowTexCoord2;
in vec4 shadowTexCoord3;
in vec4 shadowTexCoord4;
#endif

in vec4 oUv0;
uniform vec4 lightPosition[NUM_LIGHTS];
uniform vec4 lightDiffuse[NUM_LIGHTS];
uniform vec4 lightSpecular[NUM_LIGHTS];
uniform float shininess;
uniform vec4 ambientColour;
uniform vec4 fogColour;

out vec4 fragColour;

vec4 expand(vec4 x)
{
	return 2.0 * x - 1.0;
}

void lighting  (in vec4 lightPosition,
                in vec4 lightDiffuse,
                in vec4 lightSpecular,
#if NORMAL_MAP
				in vec3 L,
#endif
				in vec3 N,
				in float attenuation,
				in float shadowing,
				inout vec4 diffuse
#if SPECULAR
				, in vec3 V
				, inout vec4 specular
#endif
#if SPECULAR_PHONG
				, in vec3 R
				, inout vec4 specular
#endif
				)
{
	// Compute vector from surface to light position
#if !NORMAL_MAP
	vec3 L = normalize(lightPosition.xyz - eyePosition * lightPosition.w);
#endif

	float NdotL = max(0.0, dot(N, L));
	diffuse += lightDiffuse * NdotL * attenuation * shadowing;

#if SPECULAR
	vec3 H = normalize(L + V);

	float NdotH = max(0.0, dot(N, H));
	if (NdotL > 0.0) {
		specular += lightSpecular * pow(NdotH, shininess) * attenuation * shadowing;
	} else {
		specular += 0.0;
	}
#endif
#if SPECULAR_PHONG
	// Phong specular power is dot(reflect(L, N), V), replacing this with equal equation dot(reflect(V, N), L)
	float RdotL = max(0.0, dot(R, L));
	if (NdotL > 0.0) {
		// To accomodate substitution of half vector with reflect vector, dividing shininess by 4
		specular += lightSpecular * pow(RdotL, shininess / 4.0) * attenuation * shadowing;
	}
#endif
}

#if SHADOW

float shadowPCF(in sampler2D shadowMap, in vec4 shadowTexCoord, vec2 inverseShadowMapSize)
{
	// point on shadowmap
#if LINEAR_RANGE
	shadowTexCoord.xy = shadowTexCoord.xy / shadowTexCoord.w;
#else
	shadowTexCoord = shadowTexCoord / shadowTexCoord.w;
#endif

#if !PCF
	float depth = texture(shadowMap, shadowTexCoord.xy).x * (1.0 - fixedDepthBias);
	return (depth > shadowTexCoord.z) ? 1.0 : 0.0;
#else

	// Do not shade too far away objects
	if (abs(shadowTexCoord.z) > 1.0)
	{
		return 1.0;
	}

	float centerdepth = texture(shadowMap, shadowTexCoord.xy).x;

	// gradient calculation
  	float pixeloffset = inverseShadowMapSize.x;
	vec4 depths = vec4(
		texture(shadowMap, shadowTexCoord.xy + vec2(-pixeloffset, 0)).x,
		texture(shadowMap, shadowTexCoord.xy + vec2(+pixeloffset, 0)).x,
		texture(shadowMap, shadowTexCoord.xy + vec2(0, -pixeloffset)).x,
		texture(shadowMap, shadowTexCoord.xy + vec2(0, +pixeloffset)).x);

	vec2 differences = abs( depths.yw - depths.xz );
	float gradient = min(gradientClamp, max(differences.x, differences.y));
	float gradientFactor = gradient * gradientScaleBias;

	// visibility function
	float depthAdjust = gradientFactor - (fixedDepthBias * centerdepth);
	//depthAdjust =  -fixedDepthBias * centerdepth;
	float finalCenterDepth = centerdepth + depthAdjust;

	// use depths from prev, calculate diff
	depths += depthAdjust;
	float final = (finalCenterDepth > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.x > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.y > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.z > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.w > shadowTexCoord.z) ? 1.0 : 0.0;
	final *= 0.2;

	return final;
#endif
}

float shadowPSSM(float depth)
{
	float shadowing = 0.0;

	if (depth <= pssmSplitPoints.y)
	{
		shadowing = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
	}
	else if (depth <= pssmSplitPoints.z)
	{
		shadowing = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
	}
	else
	{
		shadowing = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);
	}

	return shadowing;
}

vec3 shadowPSSMDebug(float depth)
{
	float shadowing = 0.0;
	vec3 splitColour;
	float showLayer = 0.0;

	if (depth <= pssmSplitPoints.y && showLayer == 0.0 || showLayer == 1.0)
	{
		splitColour = vec3(1.0, 0.0, 0.0);
		shadowing = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
	}
	else if (depth <= pssmSplitPoints.z && showLayer == 0.0 || showLayer == 2.0)
	{
		splitColour = vec3(0.0, 1.0, 0.0);
		shadowing = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
	}
	else
	{
		splitColour = vec3(0.0, 0.0, 1.0);
		shadowing = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);
	}

	return vec3(1.0) - (vec3(1.0) - splitColour) * (1.0 - shadowing);
}

vec3 shadow3Debug()
{
	float shadowing1, shadowing2, shadowing3;

	shadowing1 = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
	shadowing2 = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
	shadowing3 = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);

	return vec3(1.0) - ((1.0 - shadowing1) * (1.0 - vec3(1.0, 0.0, 0.0)) +
						(1.0 - shadowing2) * (1.0 - vec3(0.0, 1.0, 0.0)) +
						(1.0 - shadowing3) * (1.0 - vec3(0.0, 0.0, 1.0)));
}
#endif

void main()
{
	// Exit if fully in fog
//	if (fog == 0.0)
//	{
//		gl_FragColor = gl_Fog.color;
//		return;
//	}

	vec2 uv = oUv0.st;

#if NORMAL_MAP && PARALLAX_STEPS > 0
//Displacement disabled for now.
/*
	// Divide by number of steps to have consistent scale between techniques with different number of steps
	vec2 scaleBias_temp = scaleBias / float(PARALLAX_STEPS);

	// Scale and bias factors
	float scale = scaleBias_temp.x;
	float bias = scaleBias_temp.y;



	for (int i = 0; i < PARALLAX_STEPS; i++)
	{
		// Get the height using the original tex coords
		float height = texture(normalHeightMap, uv).a;

		// Calculate displacement
		float displacement = height * scale + bias;

		// Calculate the new tex coord to use for normal and diffuse
		uv = uv + eyeDir.xy * displacement;
	}
*/
#endif

	vec4 diffuseColour = texture(diffuseMap, uv);

	if (diffuseColour.a < 0.5) {
	    discard;
	}

#if SPECULAR || SPECULAR_PHONG
	vec4 specularColour = texture(specularMap, uv);
#endif

#if !NORMAL_MAP
	// Renormalize interpolated normal, required for good quality specular highlight
	vec3 N = normalize(eyeNormal);
#else
	vec3 N = expand(texture(normalHeightMap, uv)).xyz;
	// Need to normalize
	N = normalize(N);
#endif

	// Accumulates diffuse light colour
	vec4 diffuse = vec4(0.0);

#if !NORMAL_MAP
#if EYE_AT_ORIGIN
	vec3 V = -normalize(eyePosition);
#else
	vec3 V = vec3(0.0, 0.0, 1.0);
#endif
#else
	vec3 V = normalize(eyeDir);
#endif

#if SPECULAR
	// Accumulates specular light colour
	vec4 specular = vec4(0.0);
#endif
#if SPECULAR_PHONG
	vec3 R = reflect(-V, N);
	vec4 specular = vec4(0.0);
#endif


//We can't use a loop here, since that will break on ATI cards

#if NORMAL_MAP
	vec3 L;
#endif
	float shadowing;
	bool isUsingPSSM = lightPosition[0].w == 0.0;

#if NUM_LIGHTS > 0
	if (numberOfActiveLights > 0.0) {
#if NORMAL_MAP
		L = normalize(lightDir[0].xyz);
#endif
		shadowing = 1.0;
#if SHADOW
		// Use PSSM only for first directional light
		if (isUsingPSSM)
		{
			shadowing = shadowPSSM(oUv0.p);
		} else {
			shadowing = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
		}
#endif
#if !NORMAL_MAP
		float curAttenuation = attenuation[0];
#else
		float curAttenuation = lightDir[0].w;
#endif

		lighting(lightPosition[0]
		    , lightDiffuse[0]
		    , lightSpecular[0]
#if NORMAL_MAP
			, L
#endif
			, N, curAttenuation /*ATTENUATION_I*/
			, shadowing
			, diffuse
#if SPECULAR
			, V, specular
#endif
#if SPECULAR_PHONG
			, R, specular
#endif
		);
	}
#endif
#if NUM_LIGHTS > 1
	if (numberOfActiveLights > 1.0) {
#if NORMAL_MAP
		L = normalize(lightDir[1].xyz);
#endif
		shadowing = 1.0;
#if SHADOW
		if (isUsingPSSM)
		{
			shadowing = shadowPCF(shadowMap3, shadowTexCoord3, inverseShadowMapSize3.xy);
		} else {
			shadowing = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
		}
#endif
#if !NORMAL_MAP
		float curAttenuation = attenuation[1];
#else
		float curAttenuation = lightDir[1].w;
#endif

		lighting(lightPosition[1]
            , lightDiffuse[1]
            , lightSpecular[1]
#if NORMAL_MAP
			, L
#endif
			, N, curAttenuation /*ATTENUATION_I*/
			, shadowing
			, diffuse
#if SPECULAR
			, V, specular
#endif
#if SPECULAR_PHONG
			, R, specular
#endif
		);	
	}
#endif
#if NUM_LIGHTS > 2
	if (numberOfActiveLights > 2.0) {
#if NORMAL_MAP
		L = normalize(lightDir[2].xyz);
#endif
		shadowing = 1.0;
#if SHADOW
		if (isUsingPSSM)
		{
			shadowing = shadowPCF(shadowMap4, shadowTexCoord4, inverseShadowMapSize4.xy);
		} else {
			shadowing = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);
		}
#endif
#if !NORMAL_MAP
		float curAttenuation = attenuation[2];
#else
		float curAttenuation = lightDir[2].w;
#endif
		lighting(lightPosition[2]
            , lightDiffuse[2]
            , lightSpecular[2]
#if NORMAL_MAP
			, L
#endif
			, N, curAttenuation /*ATTENUATION_I*/
			, shadowing
			, diffuse
#if SPECULAR
			, V, specular
#endif
#if SPECULAR_PHONG
			, R, specular
#endif
		);
	}
#endif

	vec3 colour = vec3(ambientColour * diffuseColour +
#if SPECULAR || SPECULAR_PHONG
		specularColour * specular +
#endif
		diffuseColour * diffuse)
		// /1000.0 + shadowPSSMDebug(oUv0.p)
		// /1000.0 + shadow3Debug()
		;

	fragColour.rgb = mix(fogColour.rgb, colour, fog);
	fragColour.a = diffuseColour.a;
}
