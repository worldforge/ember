#version 330

// Diffuse texture
uniform sampler2D diffuseMap;
uniform float alpha_value;
uniform vec4 colour;

in vec4 oUv0;

out vec4 fragColour;

void main()
{
    vec4 diffuseColour = texture(diffuseMap, oUv0.st);

    fragColour = colour;
    if (diffuseColour.w == 0) {
        fragColour.w = 0;
    }

}