#version 330

// Diffuse texture

uniform vec4 emissiveColour;

out vec4 fragColour;

void main()
{
	fragColour = emissiveColour;
	fragColour.a = 1;
}
