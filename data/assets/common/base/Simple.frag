#version 330

in vec4 oUv0;
in vec4 oColour;

// Diffuse texture
uniform sampler2D diffuseMap;

out vec4 fragColour;

void main()
{

	vec4 diffuseColour = texture(diffuseMap, oUv0.st);
	fragColour = diffuseColour;

}
