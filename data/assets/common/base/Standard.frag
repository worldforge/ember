#version 330

// Number of lights
// 3 is resonable number to support
#define NUM_LIGHTS	3


uniform sampler2D   diffuseMap;
uniform sampler2D   specularMap;
uniform sampler2D   normalHeightMap;
uniform sampler2D   shadowMap0;
uniform sampler2D   shadowMap1;
uniform sampler2D   shadowMap2;
uniform sampler2D   shadowMap3;
uniform sampler2D   shadowMap4;
uniform float       fixedDepthBias;


uniform vec3	    cameraPosition;
uniform vec3 	    lightAmbient;

uniform vec4        scaleBias;

uniform vec4	    lightPosition;
uniform vec3	    lightDiffuse;
uniform vec3	    lightSpecular;

uniform vec4        lightPositions[NUM_LIGHTS];
uniform vec4        lightDiffuseColours[NUM_LIGHTS];
uniform vec4        lightSpecularColours[NUM_LIGHTS];
uniform vec4        lightAttenuations[NUM_LIGHTS];
uniform float       numberOfActiveLights;

uniform float       shininess;

uniform vec4        invShadowMapSize0;
uniform vec4        invShadowMapSize1;
uniform vec4        invShadowMapSize2;
uniform vec4        invShadowMapSize3;
uniform vec4        invShadowMapSize4;
//uniform vec4        pssmSplitPoints;
const   vec4        pssmSplitPoints = vec4(1.0, 15.0, 50.0, 200.0);


in      vec4        oUv0;
in      vec3        oVNormal;
in      vec4        oVPos;
in      vec3        oEyeDir;
in      mat4        oWorldMatrix;
in      vec3        oVTangent;
in      vec3        oNormal;
in      vec3        oTangent;

in      vec4        oShadowTexCoord0;
in      vec4        oShadowTexCoord1;
in      vec4        oShadowTexCoord2;
in      vec4        oShadowTexCoord3;
in      vec4        oShadowTexCoord4;


out     vec4        fragColour;


float shadowPCF(sampler2D shadowMap, vec4 shadowMapPos, vec2 offset)
{
	shadowMapPos = shadowMapPos / shadowMapPos.w;
	vec2 uv = shadowMapPos.xy;
	vec3 o = vec3(offset, -offset.x) * 0.3;

	float c =	(shadowMapPos.z <= texture(shadowMap, uv.xy - o.xy).r * (1.0 - fixedDepthBias))  ? 1.0 : 0.0;   // top left
	c +=		(shadowMapPos.z <= texture(shadowMap, uv.xy + o.xy).r * (1.0 - fixedDepthBias)) ? 1.0 : 0.0;    // bottom right
	c +=		(shadowMapPos.z <= texture(shadowMap, uv.xy + o.zy).r * (1.0 - fixedDepthBias)) ? 1.0 : 0.0;    // bottom left
	c +=		(shadowMapPos.z <= texture(shadowMap, uv.xy - o.zy).r * (1.0 - fixedDepthBias)) ? 1.0 : 0.0;    // top right

	return c / 4.0;
}

vec4 expand(vec4 x)
{
	return 2.0 * x - 1.0;
}


void main(void)
{

/*
    // Get the height using the tex coords
    float height = texture2D(normalHeightMap, oUv0.xy).a;

    // Calculate displacement
    float displacement = (height * scaleBias.x) + scaleBias.y;

    vec3 uv2 = vec3(oUv0.xy, 1.0);

    // calculate the new tex coord to use for normal and diffuse
    vec2 uv = ((oEyeDir * displacement) + uv2).xy;

*/
    vec2 uv = oUv0.xy;

	vec4 baseColour = texture(diffuseMap, uv);

	if (baseColour.a < 0.5) {
	    discard;
	}

    vec4 specularColour = texture(specularMap, uv);


	vec3 ambient    = lightAmbient * baseColour.xyz;
	vec3 normal     = normalize(oVNormal);
	vec3 eyeDir	    = normalize(cameraPosition - oVPos.xyz );

//  vec4 bumpVec = expand(texture(normalHeightMap, uv));
//	normal = normalize(bumpVec.xyz * oWorldMatrix).xyz;
//	normal = normalize(bumpVec.xyz);
//	normal = normalize((oWorldMatrix * bumpVec).xyz);




    // derive the tangent space basis
	vec3 tangent = oVTangent.xyz;

	normal = normalize(oNormal);
	tangent = normalize(oTangent);
	// we do this in the pixel shader because we don't have per-vertex normals
	// because of the LOD, we use a normal map
	vec3 binormal = normalize(cross(tangent, normal));
	// note, now we need to re-cross to derive tangent again because it wasn't orthonormal
	tangent = normalize(cross(normal, binormal));
	// derive final matrix
	mat3 TBN = mat3(tangent, binormal, normal);

	vec3 cameraDirectionTangentSpace = normalize(transpose(TBN) * (eyeDir));

    float displacement = texture(normalHeightMap, uv).a * scaleBias.x + scaleBias.y;
	uv += cameraDirectionTangentSpace.xy * displacement;
	normal = texture(normalHeightMap, uv).rgb;
	baseColour = texture(diffuseMap, uv);
	
	normal = normalize(TBN * normal);



    vec3 directLighting = vec3(0);
	for (int i = 0; i < NUM_LIGHTS && i < int(numberOfActiveLights); i++) {
    	float shadowing = 1.0;

        //First light uses PSSM
        if (i == 0) {
            if (oUv0.z <= pssmSplitPoints.y) {
                // Calculate the position of vertex in light space
                shadowing = shadowPCF(shadowMap0, oShadowTexCoord0, invShadowMapSize0.xy);
            } else if (oUv0.z <= pssmSplitPoints.z) {
                // Calculate the position of vertex in light space
                shadowing = shadowPCF(shadowMap1, oShadowTexCoord1, invShadowMapSize1.xy);
            } else {
                // Calculate the position of vertex in light space
                shadowing = shadowPCF(shadowMap2, oShadowTexCoord2, invShadowMapSize2.xy);
            }
        } else if (i == 1) {
            shadowing = shadowPCF(shadowMap3, oShadowTexCoord3, invShadowMapSize3.xy);
        } else {
            shadowing = shadowPCF(shadowMap4, oShadowTexCoord4, invShadowMapSize4.xy);
        }


        vec3 lightDir   = lightPositions[i].xyz - oVPos.xyz * lightPositions[i].w;
        float fLength	= length(lightDir);
        lightDir		= normalize(lightDir);

        float NdotL	    = max(0, dot(normal, lightDir));

        vec3 diffuse    = lightDiffuseColours[i].rgb * NdotL * baseColour.xyz;

        float attenuation = 1.0 / (lightAttenuations[i].y +
                            	    lightAttenuations[i].z * fLength +
                            		lightAttenuations[i].w * fLength * fLength);

        vec3 specular   = vec3(0);
        if (NdotL > 0.0) {
            vec3 halfVector	= normalize(lightDir + eyeDir);
            float NdotH     = max(0.0, dot(normal, halfVector));
            specular        = lightSpecularColours[i].rgb * vec3(pow(NdotH, shininess));
        }

        directLighting += (diffuse + (specular * specularColour.xyz)) * shadowing * attenuation;

	}
    fragColour = vec4(directLighting + ambient, baseColour.a);

}
