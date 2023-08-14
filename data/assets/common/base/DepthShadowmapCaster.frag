#version 330

#if ALPHA
// Diffuse texture
uniform sampler2D diffuseMap;

in vec2 oUv0;
#endif

in vec2 depth;

out vec4 fragColour;

void main()
{
#if ALPHA
	if (texture(diffuseMap, oUv0).a < 0.5)
	{
		discard;
	}
#endif

#if LINEAR_RANGE
	float finalDepth = depth.x;
#else
	float finalDepth = depth.x / depth.y;
#endif
	// just smear across all components
	// therefore this one needs high individual channel precision
	fragColour = vec4(finalDepth, finalDepth, finalDepth, 1.0);
}

