!!ARBfp1.0

OPTION ARB_precision_hint_fastest;

TEMP  blendcolor, currentcolor, coverage;

  # Fetch coverage.
  TEX coverage, fragment.texcoord[0], texture[0], 2D;

  TEX currentcolor, fragment.texcoord[1], texture[1], 2D;

  TEX blendcolor, fragment.texcoord[1], texture[2], 2D;
  LRP currentcolor, coverage.zzzz, blendcolor, currentcolor;

  TEX blendcolor, fragment.texcoord[1], texture[3], 2D;
  LRP currentcolor, coverage.yyyy, blendcolor, currentcolor;

  TEX blendcolor, fragment.texcoord[1], texture[4], 2D;
  LRP result.color, coverage.xxxx, blendcolor, currentcolor;

END
