#version 330

in vec4 oUv0;
in vec4 oColour;
in vec3 oNormal;

// Diffuse texture
uniform sampler2D diffuseMap;
uniform vec4 fogColour;
uniform vec4 fogParams; // density, linear start, linear end, 1/(end-start)
uniform vec4 ambientColour;
uniform vec4 diffuseLight;
uniform float time;

out vec4 fragColour;


void main()
{

	vec4 diffuseColour = texture(diffuseMap, oUv0.st + vec2(time * 0.005));

	float z = gl_FragCoord.z / gl_FragCoord.w;

    float fogEnd = fogParams.z;
    float fogDensity = fogParams.x;

#if FOG_LINEAR
	float fog = clamp((fogEnd - z) * fogParams.w, 0.0, 1.0);
#endif
#if FOG_EXP
	float fog = clamp(exp(- fogDensity * z), 0.0, 1.0);
#endif
#if FOG_EXP2
	float fog = clamp(exp(- fogDensity * fogDensity * z * z), 0.0, 1.0);
#endif

	vec3 colour = vec3((ambientColour * diffuseColour) + (diffuseColour * diffuseLight));

	fragColour.rgb = mix(fogColour.rgb, colour, fog);
	fragColour.a = max(diffuseColour.a, 1.0 - fog);
}
