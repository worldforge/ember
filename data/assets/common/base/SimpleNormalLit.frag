#version 330

in vec4 oUv0;
in vec4 oColour;
in vec3 oNormal;

out vec4 fragColour;

void main()
{
	fragColour = vec4((oNormal + vec3(1.0, 1.0, 1.0)) / 2, 1);
}
