#version 330

uniform sampler2D diffuseMap;
uniform sampler2D alphaMap;

in vec4 oUv0;
in vec4 oUv0Transformed;

out vec4 fragColour;

void main()
{
	vec4 diffuseColour = texture(diffuseMap, oUv0Transformed.st);
	fragColour = diffuseColour;
	fragColour.a = texture(alphaMap, oUv0.st).x;

}
