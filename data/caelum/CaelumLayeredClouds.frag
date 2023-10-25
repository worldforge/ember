#version 330
/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/



in vec3 relPosition;
in float sunGlow;
in vec4 worldPosition;
in vec4 oUv0;

uniform float   cloudMassInvScale;
uniform float   cloudDetailInvScale;
uniform vec2  cloudMassOffset;
uniform vec2  cloudDetailOffset;
uniform float   cloudMassBlend;
uniform float   cloudDetailBlend;

uniform float   cloudCoverageThreshold;

uniform vec4  sunLightColour;
uniform vec4  sunSphereColour;
uniform vec4  fogColour;
uniform vec3	sunDirection;
uniform float   cloudSharpness;
uniform float   cloudThickness;
uniform vec3	camera_position;

uniform vec3	fadeDistMeasurementVector;
uniform float	layerHeight;
uniform float	cloudUVFactor;
uniform float	heightRedFactor;

uniform float   nearFadeDist;
uniform float   farFadeDist;


// Global cloud textures
uniform sampler2D cloud_shape1;
uniform sampler2D cloud_shape2;
uniform sampler2D cloud_detail;

out vec4 fragColour;

// Get cloud layer intensity at a certain point.
float LayeredClouds_intensity
(
        in vec2   pos,
        float       cloudMassInvScale,
        float       cloudDetailInvScale,
        vec2      cloudMassOffset,
        vec2      cloudDetailOffset,
        float       cloudMassBlend,
        float       cloudDetailBlend,
        float       cloudCoverageThreshold
)
{
	// Calculate the base alpha
	vec2 finalMassOffset = cloudMassOffset + pos;
	float aCloud = mix(texture(cloud_shape1, finalMassOffset * cloudMassInvScale).r,
						texture(cloud_shape2, finalMassOffset * cloudMassInvScale).r,
						cloudMassBlend);
	float aDetail = texture(cloud_detail, (cloudDetailOffset + pos) * cloudDetailInvScale).r;
	aCloud = (aCloud + aDetail * cloudDetailBlend) / (1.0 + cloudDetailBlend);
	return max(0.0, aCloud - cloudCoverageThreshold);
}

vec4 OldCloudColor
(
		vec2       uv,
		vec3       relPosition,
		float        sunGlow,

        float   cloudMassInvScale,
        float   cloudDetailInvScale,
        vec2  cloudMassOffset,
        vec2  cloudDetailOffset,
        float   cloudMassBlend,
        float   cloudDetailBlend,

        float   cloudCoverageThreshold,

        vec4  sunColour,
        vec4  fogColour,
        float   cloudSharpness,
        float   cloudThickness

) {
    // Initialize output.
	vec4 oCol = vec4(1.0, 1.0, 1.0, 0.0);

	// Get cloud intensity.
	float intensity = LayeredClouds_intensity
    (
            uv,
            cloudMassInvScale,
            cloudDetailInvScale,
            cloudMassOffset,
            cloudDetailOffset,
            cloudMassBlend,
            cloudDetailBlend,
            cloudCoverageThreshold
    );

	// Opacity is exponential.
	float aCloud = clamp(exp(cloudSharpness * intensity) - 1.0, 0.0, 1.0);

	float shine = pow(clamp(sunGlow, 0.0, 1.0), 8.0) / 4.0;
	sunColour.rgb *= 1.5;
	vec3 cloudColour = fogColour.rgb * (1.0 - intensity / 3.0);
	float thickness = clamp(0.8 - exp(-cloudThickness * (intensity + 0.2 - shine)), 0.0, 1.0);

	oCol.rgb = mix(sunColour.rgb, cloudColour.rgb, thickness);
	oCol.a = aCloud;

	return oCol;
}

//Converts a color from RGB to YUV color space
//the rgb color is in [0,1] [0,1] [0,1] range
//the yuv color is in [0,1] [-0.436,0.436] [-0.615,0.615] range
vec3 YUVfromRGB(vec3 col)
{
    return vec3(dot(col, vec3(0.299,0.587,0.114)),
				  dot(col, vec3(-0.14713,-0.28886,0.436)),
				  dot(col, vec3(0.615,-0.51499,-0.10001)));
}

vec3 RGBfromYUV(vec3 col)
{
    return vec3(dot(col,vec3(1,0,1.13983)),
				  dot(col,vec3(1,-0.39465,-0.58060)),
				  dot(col,vec3(1,2.03211,0)));
}

// Creates a color that has the intensity of col1.0 and the chrominance of col2
vec3 MagicColorMix(vec3 col1, vec3 col2)
{
    return clamp(RGBfromYUV(vec3(YUVfromRGB(col1).x, YUVfromRGB(col2).yz)), 0.0, 1.0);
}

// Entry point for Cloud fragment program.
void main() {
    vec2 uv = oUv0.st;
	uv *= cloudUVFactor;

    fragColour = OldCloudColor(
            uv, relPosition, sunGlow,
		    cloudMassInvScale, cloudDetailInvScale,
            cloudMassOffset, cloudDetailOffset,
            cloudMassBlend, cloudDetailBlend,
            cloudCoverageThreshold,
		    sunLightColour,
            fogColour,
            cloudSharpness,
            cloudThickness);
	fragColour.r += layerHeight / heightRedFactor;

//	//float dist = distance(worldPosition.xyz, camera_position.xyz);
	float dist = length((worldPosition.xyz - camera_position) * fadeDistMeasurementVector);
	float aMod = 1.0;
	if (dist > nearFadeDist) {
        aMod = clamp(mix(0.0, 1.0, (farFadeDist - dist) / (farFadeDist - nearFadeDist)), 0.0, 1.0);
    }
    float alfa = fragColour.a * aMod;

	vec3 cloudDir = normalize(
	         vec3(worldPosition.x, layerHeight, worldPosition.y) - camera_position);
	float angleDiff = clamp(dot(cloudDir, normalize(sunDirection.xyz)), 0.0, 1.0);

	vec3 lCol = mix(fragColour.rgb, MagicColorMix(fragColour.rgb, sunSphereColour.rgb), angleDiff);
	fragColour.rgb = mix(lCol, fragColour.rgb, alfa);
	fragColour.a = alfa;

}
