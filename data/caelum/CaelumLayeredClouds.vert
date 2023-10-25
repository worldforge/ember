#version 330
/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

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


uniform mat4 projMatrix;
uniform mat4 worldViewMatrix;

uniform vec3 sunDirection;

out vec3 relPosition;
out float sunGlow;
out vec4 worldPosition;
out vec4 oUv0;

//This attribute is automatically populated by Ogre, with the UV of the first texture
in vec4 uv0;
in vec4 vertex;


// Entry point for Cloud vertex program.
void main() {



    gl_Position = projMatrix * worldViewMatrix * vertex;
    worldPosition = worldViewMatrix * vertex;
    oUv0 = uv0;

    // This is the relative position, or view direction.
	relPosition = normalize (vertex.xyz);


    // Calculate the angle between the direction of the sun and the current
    // view direction. This we call "glow" and ranges from 1 next to the sun
    // to -1 in the opposite direction.
	sunGlow = dot (relPosition, normalize (-sunDirection));
}


