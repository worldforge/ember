!!ARBfp1.0


OPTION ARB_precision_hint_fastest;
TEMP  blendcolor, currentcolor, coverage;

  # Fetch coverage.
  TEX coverage, fragment.texcoord[0], texture[0], 2D;

  TEX blendcolor, fragment.texcoord[1], texture[1], 2D;
  MUL currentcolor, coverage.wwww, blendcolor;

  TEX blendcolor, fragment.texcoord[1], texture[2], 2D;
  MAD currentcolor, coverage.zzzz, blendcolor, currentcolor;

  TEX blendcolor, fragment.texcoord[1], texture[3], 2D;
  MAD currentcolor, coverage.yyyy, blendcolor, currentcolor;

  TEX blendcolor, fragment.texcoord[1], texture[4], 2D;
  MAD currentcolor, coverage.xxxx, blendcolor, currentcolor;

  MUL result.color, currentcolor, fragment.color;

END
