#version 330

uniform float offset;
uniform sampler2D gradientsMap;
uniform sampler2D atmRelativeDepth;

in float incidenceAngleCos;
in float y;
in vec4 oUv0;
out vec4 fragColour;


float bias (float b, float x)
{
	return pow (x, log (b) / log (0.5));
}


vec4 sunlightInscatter
(
    vec4 sunColour,
    float absorption,
    float incidenceAngleCos,
    float sunlightScatteringFactor
)
{
	float scatteredSunlight = bias (sunlightScatteringFactor * 0.5, incidenceAngleCos);

	sunColour = sunColour * (1.0 - absorption) * vec4 (0.9, 0.5, 0.09, 1);

    return sunColour * scatteredSunlight;
}

 // main procedure, the original name was SkyDomeFP
void main()
{

    vec2 uv = oUv0.st;
    vec4 sunColour = vec4(1.0, 1.0, 1.0, 1.0);

    fragColour = texture(gradientsMap, uv + vec2(offset, 0.0));

    if (incidenceAngleCos > 0.0)
        {
    		float sunlightScatteringFactor = 0.05;
    		float sunlightScatteringLossFactor = 0.3;
    		float atmLightAbsorptionFactor = 0.1;

            float atmDepth = texture(atmRelativeDepth, vec2(y, 0)).r;

    		fragColour.rgb += sunlightInscatter (
                    sunColour,
                    clamp (atmLightAbsorptionFactor * (1.0 - atmDepth), 0.0, 1.0),
                    clamp (incidenceAngleCos, 0.0, 1.0),
                    sunlightScatteringFactor).rgb * (1.0 - sunlightScatteringLossFactor);

    	}

}
