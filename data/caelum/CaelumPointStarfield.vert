#version 330

/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2008 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

// These params are in clipspace; not pixels
uniform mat4 worldViewProj;
uniform float mag_scale;
uniform float mag0_size;
uniform float min_size;
uniform float max_size;
uniform float render_target_flipping;

// width/height
uniform float aspect_ratio;

//This attribute is automatically populated by Ogre, with the UV of the first texture
in vec4 uv0;
in vec4 vertex;

out vec4 oUv0;
out vec4 colour;

void main()
{
    gl_Position = worldViewProj * vertex;

    oUv0 = uv0;

    float magnitude = uv0.z;
    float size = exp(mag_scale * magnitude) * mag0_size;

    // Fade below minSize.
    float fade = clamp(size / min_size, 0.0, 1.0);
    colour = vec4(1.0, 1.0, 1.0, fade * fade);

    // Clamp size to range.
    size = clamp(size, min_size, max_size);

    // Splat the billboard on the screen.
    gl_Position.xy +=
            gl_Position.w *
            uv0.xy *
            vec2(size, size * aspect_ratio * render_target_flipping);

}
