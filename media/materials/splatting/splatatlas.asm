!!ARBfp1.0

OPTION ARB_precision_hint_fastest;
TEMP  repeatcoord, coordset, blendcolor, currentcolor, coverage;

  # Fetch coverage.
  TEX coverage, fragment.texcoord[0], texture[0], 2D;

	#grass
  #MUL coordset, 0.25, fragment.texcoord[1];#uppper left
  #MUL repeatcoord, 0.05, coordset;
  #FRC repeatcoord, 20;

  MUL coordset, 1024, fragment.texcoord[1];#uppper left
  MUL coordset, coordset, 0.05;
  FRC coordset, coordset;
  MUL coordset, coordset, 0.0009765625;

  TEX blendcolor, repeatcoord, texture[2], 2D;
  #MUL currentcolor, coverage.zzzz, blendcolor;

MOV result.color, blendcolor;



	#sand
  #ADD coordset, coordset, {0.0, 0.25, 0.0, 0.0};# goes down
  #MUL repeatcoord, 1, coordset;
  #TEX blendcolor, repeatcoord, texture[1], 2D;
  #MAD currentcolor, coverage.yyyy, blendcolor, currentcolor;


	#snow
  #ADD coordset, coordset, {0.0, 0.25, 0.0, 0.0};# goes down
  #MUL repeatcoord, 0.05, coordset;
  #TEX blendcolor, repeatcoord, texture[1], 2D;
  #MAD currentcolor, coverage.xxxx, blendcolor, currentcolor;

	#rock
  #ADD coordset, coordset, {0.0, 0.25, 0.0, 0.0};# goes down
  #MUL repeatcoord, 0.05, coordset;
  #TEX blendcolor, repeatcoord, texture[1], 2D;
  #MAD currentcolor, coverage.wwww, blendcolor, currentcolor;


  #MUL result.color, currentcolor, fragment.color;

END
