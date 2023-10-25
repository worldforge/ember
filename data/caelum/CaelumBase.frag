#version 330

in vec4 oUv0;
in vec4 oColour;

uniform sampler2D diffuseMap;
uniform vec4 emissiveColour;

out vec4 fragColour;

void main()
{
	vec4 diffuseColour = texture(diffuseMap, oUv0.st);
	fragColour = diffuseColour * vec4(oColour.rgb, 1);
}
