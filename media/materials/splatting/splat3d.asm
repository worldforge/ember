!!ARBfp1.0

OPTION ARB_precision_hint_fastest;
TEMP  coordset, blendcolor, currentcolor, coverage;

	# Fetch coverage.
TEX coverage, fragment.texcoord[0], texture[0], 2D;

MOV coordset, fragment.texcoord[1];

	#grass
MOV coordset.z, 0;#uppper left
TEX blendcolor, coordset, texture[1], 3D;
MUL currentcolor, coverage.zzzz, blendcolor;

	#sand
MOV coordset.z, 0.33;
TEX blendcolor, coordset, texture[1], 3D;
MAD currentcolor, coverage.yyyy, blendcolor, currentcolor;


	#rock
MOV coordset.z, 0.66;
TEX blendcolor, coordset, texture[1], 3D;
MAD currentcolor, coverage.wwww, blendcolor, currentcolor;

	#snow
MOV coordset.z, 1;
TEX blendcolor, coordset, texture[1], 3D;
MAD currentcolor, coverage.xxxx, blendcolor, currentcolor;


MUL result.color, currentcolor, fragment.color;
#MOV result.color, blendcolor;

END
