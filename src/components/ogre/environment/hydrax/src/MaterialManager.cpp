/*
--------------------------------------------------------------------------------
This source file is part of Hydrax.
Visit ---

Copyright (C) 2008 Xavier Verguín González <xavierverguin@hotmail.com>
                                           <xavyiy@gmail.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
--------------------------------------------------------------------------------
*/

#include "MaterialManager.h"

#include "Hydrax.h"

#define _def_Water_Material_Name  "_Hydrax_Water_Material"
#define _def_Water_Shader_VP_Name "_Hydrax_Water_VP"
#define _def_Water_Shader_FP_Name "_Hydrax_Water_FP"

#define _def_Depth_Material_Name  "_Hydrax_Depth_Material"
#define _def_Depth_Shader_VP_Name "_Hydrax_Depth_VP"
#define _def_Depth_Shader_FP_Name "_Hydrax_Depth_FP"

#define _def_Underwater_Material_Name  "_Hydrax_Underwater_Material"
#define _def_Underwater_Shader_VP_Name "_Hydrax_Underwater_Shader_VP"
#define _def_Underwater_Shader_FP_Name "_Hydrax_Underwater_Shader_FP"

#define _def_Underwater_Compositor_Material_Name  "_Hydrax_Underwater_Compositor_Material"
#define _def_Underwater_Compositor_Shader_VP_Name "_Hydrax_Underwater_Compositor_Shader_VP"
#define _def_Underwater_Compositor_Shader_FP_Name "_Hydrax_Underwater_Compositor_Shader_FP"

#define _def_Underwater_Compositor_Name "_Hydrax_Underwater_Compositor_Name"

#define _def_Simple_Red_Material_Name "_Hydrax_Simple_Red_Material"
#define _def_Simple_Black_Material_Name "_Hydrax_Simple_Black_Material"

namespace Hydrax
{
	MaterialManager::MaterialManager(Hydrax *h)
		: mComponents(HYDRAX_COMPONENTS_NONE)
		, mHydrax(h)
		, mCreated(false)
	{
		for (int k = 0; k < 6; k++)
		{
			mMaterials[k].setNull();
		}

		for (int k = 0; k < 1; k++)
		{
			mCompositorsEnable[k] = false;
			mCompositorsNeedToBeReloaded[k] = false;
		}

		mUnderwaterCompositorListener.mMaterialManager = this;
	}

	MaterialManager::~MaterialManager()
	{
		removeMaterials();
	}

	void MaterialManager::removeMaterials()
	{
		if (Ogre::MaterialManager::getSingleton().resourceExists(_def_Water_Material_Name))
		{
			Ogre::MaterialManager::getSingleton().remove(_def_Water_Material_Name);

			Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Water_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Water_Shader_FP_Name);
			Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Water_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Water_Shader_FP_Name);
		}

		if (Ogre::MaterialManager::getSingleton().resourceExists(_def_Depth_Material_Name))
		{
			Ogre::MaterialManager::getSingleton().remove(_def_Depth_Material_Name);

			Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Depth_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Depth_Shader_FP_Name);
			Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Depth_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Depth_Shader_FP_Name);
		}

		if (Ogre::MaterialManager::getSingleton().resourceExists(_def_Underwater_Material_Name))
		{
			Ogre::MaterialManager::getSingleton().remove(_def_Underwater_Material_Name);

			Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Underwater_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Underwater_Shader_FP_Name);
			Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Underwater_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Underwater_Shader_FP_Name);
		}

		if (Ogre::MaterialManager::getSingleton().resourceExists(_def_Simple_Red_Material_Name))
		{
			Ogre::MaterialManager::getSingleton().remove(_def_Simple_Red_Material_Name);
		}

		if (Ogre::MaterialManager::getSingleton().resourceExists(_def_Simple_Black_Material_Name))
		{
			Ogre::MaterialManager::getSingleton().remove(_def_Simple_Black_Material_Name);
		}

		removeCompositor();

		mCreated = false;
	}

	void MaterialManager::removeCompositor()
	{
		if (Ogre::MaterialManager::getSingleton().resourceExists(_def_Underwater_Compositor_Material_Name))
		{
			setCompositorEnable(COMP_UNDERWATER, false);
			Ogre::CompositorManager::getSingleton().remove(_def_Underwater_Compositor_Name);

			Ogre::MaterialManager::getSingleton().remove(_def_Underwater_Compositor_Material_Name);

			Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Underwater_Compositor_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().unload(_def_Underwater_Compositor_Shader_FP_Name);
			Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Underwater_Compositor_Shader_VP_Name);
		    Ogre::HighLevelGpuProgramManager::getSingleton().remove(_def_Underwater_Compositor_Shader_FP_Name);
		}
	}

	bool MaterialManager::createMaterials(const HydraxComponent &Components, const Options &Options)
	{
		removeMaterials();

		HydraxLOG("Creating water material...");
		if (!_createWaterMaterial(Components, Options))
		{
			return false;
		}
		mHydrax->getDecalsManager()->registerAll();
		HydraxLOG("Water material created.");

		if (_isComponent(Components, HYDRAX_COMPONENT_DEPTH))
		{
			HydraxLOG("Creating depth material...");
			if(!_createDepthMaterial(Components, Options))
			{
				return false;
			}
			HydraxLOG("Depth material created.");
		}

		if (_isComponent(Components, HYDRAX_COMPONENT_UNDERWATER))
		{
			HydraxLOG("Creating underwater material...");
			if(!_createUnderwaterMaterial(Components, Options))
			{
				return false;
			}
			if(!_createUnderwaterCompositor(Components, Options))
			{
				return false;
			}
			if(!_createSimpleColorMaterial(Ogre::ColourValue::Red, MAT_SIMPLE_RED, _def_Simple_Red_Material_Name, false))
			{
				return false;
			}
			HydraxLOG("Underwater material created.");
		}

		mComponents = Components;
		mOptions = Options;
		mCreated = true;

		std::vector<Ogre::Technique*>::iterator TechIt;

		for(TechIt = mDepthTechniques.begin(); TechIt != mDepthTechniques.end(); TechIt++)
		{
			if (!(*TechIt))
			{
				mDepthTechniques.erase(TechIt);
				// TechIt-- ?
				continue;
			}

			addDepthTechnique((*TechIt), false);
		}

		return true;
	}

	bool MaterialManager::fillGpuProgramsToPass(Ogre::Pass* Pass,
							                    const Ogre::String GpuProgramNames[2],
						                        const ShaderMode& SM,
				     	                        const Ogre::String EntryPoints[2],
			     		                        const Ogre::String Data[2])
	{
		GpuProgram GpuPrograms[2] = {GPUP_VERTEX, GPUP_FRAGMENT};

		for (int k = 0; k < 2; k++)
		{
			 if (!createGpuProgram(GpuProgramNames[k], SM, GpuPrograms[k], EntryPoints[k], Data[k]))
			 {
				 return false;
			 }
		}

		Pass->setVertexProgram(GpuProgramNames[0]);
		Pass->setFragmentProgram(GpuProgramNames[1]);

		return true;
	}

	bool MaterialManager::createGpuProgram(const Ogre::String &Name,
			                               const ShaderMode& SM,
							               const GpuProgram& GPUP,
							               const Ogre::String& EntryPoint,
							               const Ogre::String& Data)
	{
		if (Ogre::HighLevelGpuProgramManager::getSingleton().resourceExists(Name))
		{
			HydraxLOG("Error in bool MaterialManager::createGpuProgram(): "+ Name + " exists.");
			return false;
		}

		Ogre::String ShaderModesStr[3] = {"hlsl", "cg", "glsl"};
		Ogre::String Profiles[2];

		switch (SM)
		{
		    case SM_HLSL:
			{
				Profiles[0] = "target";

				if (GPUP == GPUP_VERTEX)
				{
				    Profiles[1] = "vs_1_1";
				}
				else
				{
					Profiles[1] = "ps_2_0";
				}
			}
			break;

			case SM_CG:
			{
				Profiles[0] = "profiles";

				if (GPUP == GPUP_VERTEX)
				{
				    Profiles[1] = "vs_1_1 arbvp1";
				}
				else
				{
					Profiles[1] = "ps_2_0 arbfp1 fp20";
				}
			}
			break;

			case SM_GLSL:
			{}
			break;
		}

		Ogre::GpuProgramType GpuPType;

		if (GPUP == GPUP_VERTEX)
		{
			GpuPType = Ogre::GPT_VERTEX_PROGRAM;
		}
		else
		{
			GpuPType = Ogre::GPT_FRAGMENT_PROGRAM;
		}

		Ogre::HighLevelGpuProgramPtr HLGpuProgram =
			Ogre::HighLevelGpuProgramManager::getSingleton().
			      createProgram(Name,
			                    HYDRAX_RESOURCE_GROUP,
					  		    ShaderModesStr[static_cast<int>(SM)],
			                    GpuPType);

	    HLGpuProgram->setSource(Data);
        HLGpuProgram->setParameter("entry_point", EntryPoint);
        HLGpuProgram->setParameter(Profiles[0], Profiles[1]);
		HLGpuProgram->load();

		return true;
	}

	bool MaterialManager::_createWaterMaterial(const HydraxComponent &Components, const Options &Options)
	{
		const bool cDepth    = _isComponent(Components, HYDRAX_COMPONENT_DEPTH   );
		const bool cSmooth   = _isComponent(Components, HYDRAX_COMPONENT_SMOOTH  );
		const bool cSun      = _isComponent(Components, HYDRAX_COMPONENT_SUN     );
		const bool cFoam     = _isComponent(Components, HYDRAX_COMPONENT_FOAM    );
		const bool cCaustics = _isComponent(Components, HYDRAX_COMPONENT_CAUSTICS);

		Ogre::String VertexProgramData, FragmentProgramData;

		// Vertex program

		switch (Options.NM)
		{
		    case NM_TEXTURE:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						VertexProgramData +=
						Ogre::String(
						"void main_vp(\n") +
						       // IN
                          	   "float4 iPosition         : POSITION,\n" +
                           	   "float2 iUv               : TEXCOORD0,\n" +
                           	   // OUT
                           	   "out float4 oPosition      : POSITION,\n" +
							   "out float4 oPosition_     : TEXCOORD0,\n" +
                               "out float2 oUvNoise       : TEXCOORD1,\n" +
                               "out float4 oUvProjection  : TEXCOORD2,\n";
						       if (cFoam)
							   {
							       VertexProgramData += "out float4 oWorldPosition : TEXCOORD3,\n uniform float4x4         uWorld,\n";
							   }
					    VertexProgramData +=
				        Ogre::String(
                               // UNIFORM
                               "uniform float4x4         uWorldViewProj)\n") +
               	        "{\n" +
                  	        "oPosition_  = iPosition;\n";
							if (cFoam)
							{
							    VertexProgramData += "oWorldPosition = mul(uWorld, iPosition);\n";
							}
						VertexProgramData +=
				        Ogre::String(
	              	        "oPosition = mul(uWorldViewProj, iPosition);\n") +
	               	        // Projective texture coordinates, adjust for mapping
	                	    "float4x4 scalemat = float4x4(0.5,   0,   0, 0.5,"+
	                                              	     "0,-0.5,   0, 0.5,"+
	                							  	     "0,   0, 0.5, 0.5,"+
	                							  	     "0,   0,   0,   1);\n" +
	               	        "oUvProjection = mul(scalemat, oPosition);\n" +
	               	        "oUvNoise = iUv;\n" +
               	         "}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
		    }
		    break;

			case NM_VERTEX:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						VertexProgramData +=
						Ogre::String(
						"void main_vp(\n") +
						       // IN
                          	   "float4 iPosition         : POSITION,\n" +
							   "float3 iNormal           : NORMAL,\n"+
                           	   // OUT
                           	   "out float4 oPosition     : POSITION,\n" +
                               "out float4 oPosition_    : TEXCOORD0,\n" +
                               "out float4 oUvProjection : TEXCOORD1,\n" +
							   "out float3 oNormal       : TEXCOORD2,\n";
						       if (cFoam)
							   {
							       VertexProgramData += "out float4 oWorldPosition : TEXCOORD3,\n uniform float4x4         uWorld,\n";
							   }
					    VertexProgramData +=
				        Ogre::String(
                               // UNIFORM
                               "uniform float4x4         uWorldViewProj)\n") +
               	        "{\n" +
                  	        "oPosition_  = iPosition;\n";
							if (cFoam)
							{
							    VertexProgramData += "oWorldPosition = mul(uWorld, iPosition);\n";
							}
						VertexProgramData +=
				        Ogre::String(
	              	        "oPosition = mul(uWorldViewProj, iPosition);\n") +
	               	        // Projective texture coordinates, adjust for mapping
	                	    "float4x4 scalemat = float4x4(0.5,   0,   0, 0.5,"+
	                                              	     "0,-0.5,   0, 0.5,"+
	                							  	     "0,   0, 0.5, 0.5,"+
	                							  	     "0,   0,   0,   1);\n" +
	               	        "oUvProjection = mul(scalemat, oPosition);\n" +
							"oNormal = normalize(iNormal);\n"+
               	         "}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
		    }
		    break;

			case NM_RTT:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						VertexProgramData +=
						Ogre::String(
						"void main_vp(\n") +
						       // IN
                          	   "float4 iPosition         : POSITION,\n" +
                           	   // OUT
                           	   "out float4 oPosition     : POSITION,\n" +
                               "out float4 oPosition_    : TEXCOORD0,\n" +
                               "out float4 oUvProjection : TEXCOORD1,\n";
						       if (cFoam)
							   {
							       VertexProgramData += "out float4 oWorldPosition : TEXCOORD2,\n uniform float4x4         uWorld,\n";
							   }
					    VertexProgramData +=
				        Ogre::String(
                               // UNIFORM
                               "uniform float4x4         uWorldViewProj)\n") +
               	        "{\n" +
                  	        "oPosition_  = iPosition;\n";
							if (cFoam)
							{
							    VertexProgramData += "oWorldPosition = mul(uWorld, iPosition);\n";
							}
						VertexProgramData +=
				        Ogre::String(
	              	        "oPosition = mul(uWorldViewProj, iPosition);\n") +
	               	        // Projective texture coordinates, adjust for mapping
	                	    "float4x4 scalemat = float4x4(0.5,   0,   0, 0.5,"+
	                                              	     "0,-0.5,   0, 0.5,"+
	                							  	     "0,   0, 0.5, 0.5,"+
	                							  	     "0,   0,   0,   1);\n" +
	               	        "oUvProjection = mul(scalemat, oPosition);\n" +
               	         "}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
			}
		    break;
		}

		// Fragment program

		switch (Options.NM)
		{
		    case NM_TEXTURE: case NM_VERTEX: case NM_RTT:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						FragmentProgramData +=
							Ogre::String("float3 expand(float3 v)\n") +
						    "{\n" +
	                            "return (v - 0.5) * 2;\n" +
							"}\n\n" +

							"void main_fp(" +
							    // IN
                                "float4 iPosition      : TEXCOORD0,\n";
						int TEXCOORDNUM = 1;
						if (Options.NM == NM_TEXTURE)
						{
							FragmentProgramData +=
								"float2 iUvNoise      : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
							TEXCOORDNUM++;

						}
						FragmentProgramData +=
                                "float4 iUvProjection : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
						TEXCOORDNUM++;
						if (Options.NM == NM_VERTEX)
						{
							FragmentProgramData +=
							    "float4 iNormal       : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
							TEXCOORDNUM++;
						}
						if (cFoam)
						{
							FragmentProgramData +=
							    "float4 iWorldPosition  : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
						}
						FragmentProgramData +=
							Ogre::String(
	                            // OUT
	                            "out float4 oColor    : COLOR,\n") +
	                            // UNIFORM
	                            "uniform float3       uEyePosition,\n" +
	                            "uniform float        uFullReflectionDistance,\n" +
	                            "uniform float        uGlobalTransparency,\n" +
	                            "uniform float        uNormalDistortion,\n";

						if (cDepth)
						{
							FragmentProgramData +=
								"uniform float3       uWaterColor,\n";
						}
						if (cSmooth)
						{
							FragmentProgramData +=
								"uniform float        uSmoothPower,\n";
						}
						if (cSun)
						{
							FragmentProgramData += Ogre::String(
							    "uniform float3       uSunPosition,\n") +
	                            "uniform float        uSunStrength,\n" +
	                            "uniform float        uSunArea,\n" +
	                            "uniform float3       uSunColor,\n";
						}
						if (cFoam)
						{
							FragmentProgramData += Ogre::String(
							    "uniform float        uFoamRange,\n") +
							    "uniform float        uFoamMaxDistance,\n" +
	                            "uniform float        uFoamScale,\n" +
	                            "uniform float        uFoamStart,\n" +
	                            "uniform float        uFoamTransparency,\n";
						}
						if (cCaustics)
						{
							FragmentProgramData +=
								"uniform float        uCausticsPower,\n";
						}

						int TexNum = 0;

						if (Options.NM == NM_TEXTURE || Options.NM == NM_RTT)
						{
						    FragmentProgramData +=
							   "uniform sampler2D    uNormalMap       : register(s" + Ogre::StringConverter::toString(TexNum) + "),\n";
							TexNum++;
						}

						FragmentProgramData +=
							Ogre::String(
						       "uniform sampler2D    uReflectionMap   : register(s" + Ogre::StringConverter::toString(TexNum) + "),\n") +
	                           "uniform sampler2D    uRefractionMap   : register(s" + Ogre::StringConverter::toString(TexNum+1) + "),\n";

						TexNum += 2;

						if (cDepth)
						{
							FragmentProgramData +=
								"uniform sampler2D    uDepthMap        : register(s" + Ogre::StringConverter::toString(TexNum) + "),\n";
							TexNum++;
						}

						FragmentProgramData +=
								//"uniform sampler1D    uFresnelMap      : register(s" + Ogre::StringConverter::toString(TexNum) + ")";
								"uniform sampler2D    uFresnelMap      : register(s" + Ogre::StringConverter::toString(TexNum) + ")";
						TexNum++;

						if (cFoam)
						{
							FragmentProgramData += Ogre::String(
							 ",\nuniform sampler2D    uFoamMap         : register(s" + Ogre::StringConverter::toString(TexNum) + ")\n");
						}

						FragmentProgramData +=
							Ogre::String(                                            ")\n") +
							"{\n"     +
							    "float2 ProjectionCoord = iUvProjection.xy / iUvProjection.w;\n" +
                                "float3 camToSurface = iPosition.xyz - uEyePosition;\n" +
                                "float additionalReflection=camToSurface.x*camToSurface.x+camToSurface.z*camToSurface.z;\n";

						if (cFoam)
						{
							// Calculate the foam visibility as a function fo distance specified by user
							FragmentProgramData +=
								"float foamVisibility=1.0f-saturate(additionalReflection/uFoamMaxDistance);\n";
						}

						FragmentProgramData +=
							Ogre::String(
							    "additionalReflection/=uFullReflectionDistance;\n") +
								"camToSurface=normalize(-camToSurface);\n";
						if (Options.NM == NM_TEXTURE)
						{
							FragmentProgramData += Ogre::String(
								"float3 pixelNormal = tex2D(uNormalMap,iUvNoise);\n") +
								// Inverte y with z, because at creation our local normal to the plane was z
								"pixelNormal.yz=pixelNormal.zy;\n" +
								// Remap from [0,1] to [-1,1]
								"pixelNormal.xyz=expand(pixelNormal.xyz);\n";
						}
						else if (Options.NM == NM_VERTEX)
						{
							FragmentProgramData +=
								"float3 pixelNormal = iNormal;\n";
						}
						else // NM_RTT
						{
							FragmentProgramData +=
								"float3 pixelNormal = 2.0*tex2D(uNormalMap, ProjectionCoord.xy) - 1.0;\n";
						}
						FragmentProgramData +=
								"float2 pixelNormalModified = uNormalDistortion*pixelNormal.zx;\n";
						if (Options.NM == NM_TEXTURE || Options.NM == NM_RTT)
						{
							FragmentProgramData +=
								"float dotProduct=dot(camToSurface,pixelNormal);\n";
						}
						else
						{
							FragmentProgramData +=
								"float dotProduct=dot(-camToSurface,pixelNormal);\n";
						}
						FragmentProgramData +=
							Ogre::String(
								"dotProduct=saturate(dotProduct);\n") +
								//"float fresnel = tex1D(uFresnelMap,dotProduct);\n" +
								"float fresnel = tex2D(uFresnelMap,float2(dotProduct,dotProduct));\n" +
								// Add additional reflection and saturate
								"fresnel+=additionalReflection;\n" +
								"fresnel=saturate(fresnel);\n" +
								// Decrease the transparency and saturate
								"fresnel-=uGlobalTransparency;\n" +
                                "fresnel=saturate(fresnel);\n" +
								// Get the reflection/refraction pixels. Make sure to disturb the texcoords by pixelnormal
								"float4 reflection=tex2D(uReflectionMap,ProjectionCoord.xy+pixelNormalModified);\n" +
								"float4 refraction=tex2D(uRefractionMap,ProjectionCoord.xy-pixelNormalModified);\n";

						if (cDepth)
						{
							if (cCaustics)
						    {
								FragmentProgramData += Ogre::String(
								"float2 depth = tex2D(uDepthMap,ProjectionCoord.xy-pixelNormalModified).rg;\n") +
								"refraction *= 1+depth.y*uCausticsPower;\n" +
								"refraction = lerp(float4(uWaterColor,1),refraction,depth.x);\n";
						    }
							else
							{
								FragmentProgramData += Ogre::String(
								"float depth = tex2D(uDepthMap,ProjectionCoord.xy-pixelNormalModified).r;\n") +
								"refraction = lerp(float4(uWaterColor,1),refraction,depth);\n";
							}
						}

						FragmentProgramData +=
								"oColor = lerp(refraction,reflection,fresnel);\n";

						if (cSun)
						{
							FragmentProgramData += Ogre::String(
							    "float3 relfectedVector = normalize(reflect(-camToSurface,pixelNormal.xyz));\n") +
								"float3 surfaceToSun=normalize(uSunPosition-iPosition.xyz);\n" +
								"float3 sunlight = uSunStrength*pow(saturate(dot(relfectedVector,surfaceToSun)),uSunArea)*uSunColor;\n" +
								"oColor.xyz+=sunlight;\n";
						}

						if (cFoam)
						{
							FragmentProgramData += Ogre::String(
							    "float hmap = iPosition.y/uFoamRange*foamVisibility;\n") +
								"float2 foamTex=iWorldPosition.xz*uFoamScale+pixelNormalModified;\n" +
								"float foam=tex2D(uFoamMap,foamTex).r;\n" +
								"float foamTransparency=saturate(hmap-uFoamStart)*uFoamTransparency;\n" +
								"oColor=lerp(oColor,float4(1,1,1,1),foamTransparency*foam);\n";
						}

						if (cSmooth)
						{
							FragmentProgramData +=
								"oColor.xyz = lerp(tex2D(uRefractionMap,ProjectionCoord.xy).xyz,oColor.xyz,saturate((1-tex2D(uDepthMap,ProjectionCoord.xy).r)*uSmoothPower));\n";
						}

						FragmentProgramData +=
							"}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
		    }
		    break;
		}

		// Build our material
		Ogre::MaterialPtr &WaterMaterial = getMaterial(MAT_WATER);
		WaterMaterial = Ogre::MaterialManager::getSingleton().
			create(_def_Water_Material_Name,
			       HYDRAX_RESOURCE_GROUP);

		Ogre::Pass *WM_Technique0_Pass0 = WaterMaterial->getTechnique(0)->getPass(0);

		WM_Technique0_Pass0->setCullingMode(Ogre::CULL_NONE);
		WM_Technique0_Pass0->setDepthWriteEnabled(true);

		Ogre::String GpuProgramsData[2] = {VertexProgramData, FragmentProgramData};
		Ogre::String GpuProgramNames[2] = {_def_Water_Shader_VP_Name, _def_Water_Shader_FP_Name};
		Ogre::String EntryPoints[2]     = {"main_vp", "main_fp"};

		fillGpuProgramsToPass(WM_Technique0_Pass0, GpuProgramNames, Options.SM, EntryPoints, GpuProgramsData);

		Ogre::GpuProgramParametersSharedPtr VP_Parameters = WM_Technique0_Pass0->getVertexProgramParameters();
		Ogre::GpuProgramParametersSharedPtr FP_Parameters = WM_Technique0_Pass0->getFragmentProgramParameters();

		VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		if (cFoam)
		{
		    VP_Parameters->setNamedAutoConstant("uWorld",     Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
		}
		FP_Parameters->setNamedAutoConstant("uEyePosition",   Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);

		FP_Parameters->setNamedConstant("uFullReflectionDistance", mHydrax->getFullReflectionDistance());
		FP_Parameters->setNamedConstant("uGlobalTransparency",     mHydrax->getGlobalTransparency());
		FP_Parameters->setNamedConstant("uNormalDistortion",       mHydrax->getNormalDistortion());

		if (cDepth)
		{
			FP_Parameters->setNamedConstant("uWaterColor", mHydrax->getWaterColor());
		}
		if (cSmooth)
		{
			FP_Parameters->setNamedConstant("uSmoothPower", mHydrax->getSmoothPower());
		}
		if (cSun)
		{
			FP_Parameters->setNamedConstant("uSunPosition", mHydrax->getMesh()->getObjectSpacePosition(mHydrax->getSunPosition()));
			FP_Parameters->setNamedConstant("uSunStrength", mHydrax->getSunStrength());
			FP_Parameters->setNamedConstant("uSunArea",     mHydrax->getSunArea());
			FP_Parameters->setNamedConstant("uSunColor",    mHydrax->getSunColor());
		}
		if (cFoam)
		{
			FP_Parameters->setNamedConstant("uFoamRange",        mHydrax->getMesh()->getOptions().MeshStrength);
			FP_Parameters->setNamedConstant("uFoamMaxDistance",  mHydrax->getFoamMaxDistance());
			FP_Parameters->setNamedConstant("uFoamScale",        mHydrax->getFoamScale());
			FP_Parameters->setNamedConstant("uFoamStart",        mHydrax->getFoamStart());
			FP_Parameters->setNamedConstant("uFoamTransparency", mHydrax->getFoamTransparency());
		}
		if (cCaustics)
		{
			FP_Parameters->setNamedConstant("uCausticsPower",    mHydrax->getCausticsPower());
		}

		if (Options.NM == NM_TEXTURE || Options.NM == NM_RTT)
		{
		    WM_Technique0_Pass0->createTextureUnitState("HydraxNormalMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}

		WM_Technique0_Pass0->createTextureUnitState("HydraxReflectionMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		WM_Technique0_Pass0->createTextureUnitState("HydraxRefractionMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

		if (cDepth)
		{
			WM_Technique0_Pass0->createTextureUnitState("HydraxDepthMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		}

		WM_Technique0_Pass0->createTextureUnitState("Fresnel.bmp")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

		if (cFoam)
		{
			WM_Technique0_Pass0->createTextureUnitState("Foam.png")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}

		WaterMaterial->setReceiveShadows(false);
		WaterMaterial->load();

		return true;
	}

	bool MaterialManager::_createDepthMaterial(const HydraxComponent &Components, const Options &Options)
	{
		const bool cCaustics   = _isComponent(Components, HYDRAX_COMPONENT_CAUSTICS);

		Ogre::String VertexProgramData, FragmentProgramData;

		// Vertex program

		switch (Options.SM)
		{
		    case SM_HLSL: case SM_CG:
			{
				// No caustics
				if (!cCaustics)
				{
				    VertexProgramData +=
					    Ogre::String(
					    "void main_vp(\n") +
					        // IN
					        "float4 iPosition         : POSITION,\n" +
						    // OUT
						    "out float4 oPosition     : POSITION,\n" +
						    "out float  oPosition_    : TEXCOORD0,\n" +
						    // UNIFORM
						    "uniform float            uPlaneYPos,\n" +
                            "uniform float4x4         uWorld,\n" +
                            "uniform float4x4         uWorldViewProj)\n" +
					    "{\n" +
					       "oPosition = mul(uWorldViewProj, iPosition);\n" +
						   "oPosition_ = mul(uWorld, iPosition).y;\n" +
						   "oPosition_-=uPlaneYPos;\n" +
						"}\n";
				}
				else // Caustics
				{
					VertexProgramData +=
					    Ogre::String(
					    "void main_vp(\n") +
					        // IN
					        "float4 iPosition         : POSITION,\n" +
						    // OUT
						    "out float4 oPosition     : POSITION,\n" +
						    "out float  oPosition_    : TEXCOORD0,\n" +
							"out float2 oUvWorld      : TEXCOORD1,\n" +
						    // UNIFORM
						    "uniform float            uPlaneYPos,\n" +
                            "uniform float4x4         uWorld,\n" +
                            "uniform float4x4         uWorldViewProj)\n" +
					    "{\n" +
					       "oPosition = mul(uWorldViewProj, iPosition);\n" +
						   "float3 wPos = mul(uWorld, iPosition);\n" +
						   "oPosition_ = wPos.y;\n" +
						   "oPosition_-=uPlaneYPos;\n" +
						   "oUvWorld = wPos.xz;\n" +
						"}\n";
				}
			}
			break;

			case SM_GLSL:
			{}
			break;
		}

		// Fragment program

		switch (Options.SM)
		{
		    case SM_HLSL: case SM_CG:
			{
				// No caustics
				if (!cCaustics)
				{
					FragmentProgramData +=
						Ogre::String(
						"void main_fp(\n") +
						    // IN
						    "float  iPosition     : TEXCOORD0,\n" +
							// OUT
							"out float4 oColor    : COLOR,\n" +
							// UNIFORM
							"uniform float        uDepthLimit)\n" +
						"{\n" +
						    "float pixelYDepth = (iPosition*uDepthLimit+1);\n" +
							"pixelYDepth = saturate(pixelYDepth);\n" +
							"oColor = float4(pixelYDepth,0,0,0);\n" +
						"}\n";
				}
				else // Caustics
				{
					FragmentProgramData +=
						Ogre::String(
						"void main_fp(\n") +
						    // IN
						    "float  iPosition     : TEXCOORD0,\n" +
                            "float2 iUvWorld      : TEXCOORD1,\n" +
							// OUT
							"out float4 oColor    : COLOR,\n" +
							// UNIFORM
							"uniform float        uDepthLimit,\n" +
							"uniform float        uCausticsScale,\n" +
	                        "uniform float        uCausticsEnd,\n" +
							"uniform sampler2D    uCaustics : register(s0))\n" +
						"{\n" +
						    "float pixelYDepth = (iPosition*uDepthLimit+1);\n" +
							"pixelYDepth = saturate(pixelYDepth);\n" +
							"oColor = float4(pixelYDepth,0,0,0);\n" +
							"oColor.g = saturate(uCausticsEnd-pixelYDepth)*tex2D(uCaustics, iUvWorld/uCausticsScale).r;\n" +
						"}\n";
				}
			}
			break;

			case SM_GLSL:
			{}
			break;
		}

		// Build our material
		Ogre::MaterialPtr &DepthMaterial = getMaterial(MAT_DEPTH);
		DepthMaterial = Ogre::MaterialManager::getSingleton().
			create(_def_Depth_Material_Name,
			       HYDRAX_RESOURCE_GROUP);

		DepthMaterial->getTechnique(0)->setSchemeName("HydraxDepth");

		Ogre::Pass *DM_Technique0_Pass0 = DepthMaterial->getTechnique(0)->getPass(0);

		Ogre::String GpuProgramsData[2] = {VertexProgramData, FragmentProgramData};
		Ogre::String GpuProgramNames[2] = {_def_Depth_Shader_VP_Name, _def_Depth_Shader_FP_Name};
		Ogre::String EntryPoints[2]     = {"main_vp", "main_fp"};

		fillGpuProgramsToPass(DM_Technique0_Pass0, GpuProgramNames, Options.SM, EntryPoints, GpuProgramsData);

		Ogre::GpuProgramParametersSharedPtr VP_Parameters = DM_Technique0_Pass0->getVertexProgramParameters();
		Ogre::GpuProgramParametersSharedPtr FP_Parameters = DM_Technique0_Pass0->getFragmentProgramParameters();

		VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		VP_Parameters->setNamedAutoConstant("uWorld", Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
		VP_Parameters->setNamedConstant("uPlaneYPos", mHydrax->getPosition().y);

		FP_Parameters->setNamedConstant("uDepthLimit", 1/mHydrax->getDepthLimit());

		if (cCaustics)
		{
			FP_Parameters->setNamedConstant("uCausticsScale", mHydrax->getCausticsScale());
			FP_Parameters->setNamedConstant("uCausticsEnd",   mHydrax->getCausticsEnd());

			Ogre::TextureUnitState *TUS_Caustics = DM_Technique0_Pass0->createTextureUnitState("Caustics.bmp");
			TUS_Caustics->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
			TUS_Caustics->setAnimatedTextureName("Caustics.bmp", 32, 1.5);
		}

		DepthMaterial->setReceiveShadows(false);
		DepthMaterial->load();

		return true;
	}

	bool MaterialManager::_createUnderwaterMaterial(const HydraxComponent &Components, const Options &Options)
	{
		const bool cDepth    = _isComponent(Components, HYDRAX_COMPONENT_DEPTH   );
		const bool cSmooth   = _isComponent(Components, HYDRAX_COMPONENT_SMOOTH  );
		const bool cSun      = _isComponent(Components, HYDRAX_COMPONENT_SUN     );
		const bool cFoam     = _isComponent(Components, HYDRAX_COMPONENT_FOAM    );
		const bool cCaustics = _isComponent(Components, HYDRAX_COMPONENT_CAUSTICS);
		const bool cUReflections = _isComponent(Components, HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS);

		Ogre::String VertexProgramData, FragmentProgramData;

		// Vertex program

		switch (Options.NM)
		{
		    case NM_TEXTURE:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						VertexProgramData +=
						Ogre::String(
						"void main_vp(\n") +
						       // IN
                          	   "float4 iPosition         : POSITION,\n" +
                           	   "float2 iUv               : TEXCOORD0,\n" +
                           	   // OUT
                           	   "out float4 oPosition      : POSITION,\n" +
							   "out float4 oPosition_     : TEXCOORD0,\n" +
                               "out float2 oUvNoise       : TEXCOORD1,\n" +
                               "out float4 oUvProjection  : TEXCOORD2,\n";
						       if (cFoam)
							   {
							       VertexProgramData += "out float4 oWorldPosition : TEXCOORD3,\n uniform float4x4         uWorld,\n";
							   }
					    VertexProgramData +=
				        Ogre::String(
                               // UNIFORM
                               "uniform float4x4         uWorldViewProj)\n") +
               	        "{\n" +
                  	        "oPosition_  = iPosition;\n";
							if (cFoam)
							{
							    VertexProgramData += "oWorldPosition = mul(uWorld, iPosition);\n";
							}
						VertexProgramData +=
				        Ogre::String(
	              	        "oPosition = mul(uWorldViewProj, iPosition);\n") +
	               	        // Projective texture coordinates, adjust for mapping
	                	    "float4x4 scalemat = float4x4(0.5,   0,   0, 0.5,"+
	                                              	     "0,-0.5,   0, 0.5,"+
	                							  	     "0,   0, 0.5, 0.5,"+
	                							  	     "0,   0,   0,   1);\n" +
	               	        "oUvProjection = mul(scalemat, oPosition);\n" +
	               	        "oUvNoise = iUv;\n" +
               	         "}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
		    }
		    break;

			case NM_VERTEX:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						VertexProgramData +=
						Ogre::String(
						"void main_vp(\n") +
						       // IN
                          	   "float4 iPosition         : POSITION,\n" +
							   "float3 iNormal           : NORMAL,\n"+
                           	   // OUT
                           	   "out float4 oPosition     : POSITION,\n" +
                               "out float4 oPosition_    : TEXCOORD0,\n" +
                               "out float4 oUvProjection : TEXCOORD1,\n" +
							   "out float3 oNormal       : TEXCOORD2,\n";
						       if (cFoam)
							   {
							       VertexProgramData += "out float4 oWorldPosition : TEXCOORD3,\n uniform float4x4         uWorld,\n";
							   }
					    VertexProgramData +=
				        Ogre::String(
                               // UNIFORM
                               "uniform float4x4         uWorldViewProj)\n") +
               	        "{\n" +
                  	        "oPosition_  = iPosition;\n";
							if (cFoam)
							{
							    VertexProgramData += "oWorldPosition = mul(uWorld, iPosition);\n";
							}
						VertexProgramData +=
				        Ogre::String(
	              	        "oPosition = mul(uWorldViewProj, iPosition);\n") +
	               	        // Projective texture coordinates, adjust for mapping
	                	    "float4x4 scalemat = float4x4(0.5,   0,   0, 0.5,"+
	                                              	     "0,-0.5,   0, 0.5,"+
	                							  	     "0,   0, 0.5, 0.5,"+
	                							  	     "0,   0,   0,   1);\n" +
	               	        "oUvProjection = mul(scalemat, oPosition);\n" +
							"oNormal = normalize(iNormal);\n"+
               	         "}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
		    }
		    break;

			case NM_RTT:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						VertexProgramData +=
						Ogre::String(
						"void main_vp(\n") +
						       // IN
                          	   "float4 iPosition         : POSITION,\n" +
                           	   // OUT
                           	   "out float4 oPosition     : POSITION,\n" +
                               "out float4 oPosition_    : TEXCOORD0,\n" +
                               "out float4 oUvProjection : TEXCOORD1,\n";
						       if (cFoam)
							   {
							       VertexProgramData += "out float4 oWorldPosition : TEXCOORD2,\n uniform float4x4         uWorld,\n";
							   }
					    VertexProgramData +=
				        Ogre::String(
                               // UNIFORM
                               "uniform float4x4         uWorldViewProj)\n") +
               	        "{\n" +
                  	        "oPosition_  = iPosition;\n";
							if (cFoam)
							{
							    VertexProgramData += "oWorldPosition = mul(uWorld, iPosition);\n";
							}
						VertexProgramData +=
				        Ogre::String(
	              	        "oPosition = mul(uWorldViewProj, iPosition);\n") +
	               	        // Projective texture coordinates, adjust for mapping
	                	    "float4x4 scalemat = float4x4(0.5,   0,   0, 0.5,"+
	                                              	     "0,-0.5,   0, 0.5,"+
	                							  	     "0,   0, 0.5, 0.5,"+
	                							  	     "0,   0,   0,   1);\n" +
	               	        "oUvProjection = mul(scalemat, oPosition);\n" +
               	         "}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
			}
		    break;
		}

		// Fragment program

		switch (Options.NM)
		{
		    case NM_TEXTURE: case NM_VERTEX: case NM_RTT:
		    {
				switch (Options.SM)
				{
				    case SM_HLSL: case SM_CG:
					{
						FragmentProgramData +=
							Ogre::String("float3 expand(float3 v)\n") +
						    "{\n" +
	                            "return (v - 0.5) * 2;\n" +
							"}\n\n" +

							"void main_fp(" +
							    // IN
                                "float4 iPosition     : TEXCOORD0,\n";
						int TEXCOORDNUM = 1;
						if (Options.NM == NM_TEXTURE)
						{
							FragmentProgramData +=
								"float2 iUvNoise      : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
							TEXCOORDNUM++;

						}
						FragmentProgramData +=
                                "float4 iUvProjection : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
						TEXCOORDNUM++;
						if (Options.NM == NM_VERTEX)
						{
							FragmentProgramData +=
							    "float4 iNormal       : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
							TEXCOORDNUM++;
						}
						if (cFoam)
						{
							FragmentProgramData +=
							    "float4 iWorldPosition  : TEXCOORD" + Ogre::StringConverter::toString(TEXCOORDNUM) + ",\n";
						}
						FragmentProgramData +=
							Ogre::String(
	                            // OUT
	                            "out float4 oColor    : COLOR,\n") +
	                            // UNIFORM
	                            "uniform float3       uEyePosition,\n" +
	                            "uniform float        uFullReflectionDistance,\n" +
	                            "uniform float        uGlobalTransparency,\n" +
	                            "uniform float        uNormalDistortion,\n";

						if ((cDepth && cUReflections) || (!cUReflections))
						{
							FragmentProgramData +=
								"uniform float3       uWaterColor,\n";
						}
						if (cSun)
						{
							FragmentProgramData += Ogre::String(
							    "uniform float3       uSunPosition,\n") +
	                            "uniform float        uSunStrength,\n" +
	                            "uniform float        uSunArea,\n" +
	                            "uniform float3       uSunColor,\n";
						}
						if (cFoam)
						{
							FragmentProgramData += Ogre::String(
							    "uniform float        uFoamRange,\n") +
							    "uniform float        uFoamMaxDistance,\n" +
	                            "uniform float        uFoamScale,\n" +
	                            "uniform float        uFoamStart,\n" +
	                            "uniform float        uFoamTransparency,\n";
						}
						if (cCaustics && cUReflections)
						{
							FragmentProgramData +=
							   "uniform float        uCausticsPower,\n";
						}

						int TexNum = 0;

						if (Options.NM == NM_TEXTURE || Options.NM == NM_RTT)
						{
						    FragmentProgramData +=
							   "uniform sampler2D    uNormalMap       : register(s" + Ogre::StringConverter::toString(TexNum) + "),\n";
							TexNum++;
						}

						if (cUReflections)
						{
							FragmentProgramData +=
								"uniform sampler2D    uReflectionMap   : register(s" + Ogre::StringConverter::toString(TexNum) + "),\n";
							TexNum++;
						}

						FragmentProgramData +=
	                        "uniform sampler2D    uRefractionMap   : register(s" + Ogre::StringConverter::toString(TexNum) + "),\n";
						TexNum++;

						if (cDepth && cUReflections)
						{
							FragmentProgramData +=
								"uniform sampler2D    uDepthReflectionMap : register(s" + Ogre::StringConverter::toString(TexNum) + "),\n";
							TexNum++;
						}

						FragmentProgramData +=
								//--"uniform sampler1D    uFresnelMap      : register(s" + Ogre::StringConverter::toString(TexNum) + ")";
								"uniform sampler2D    uFresnelMap      : register(s" + Ogre::StringConverter::toString(TexNum) + ")";

						TexNum++;

						if (cFoam)
						{
							FragmentProgramData += Ogre::String(
							 ",\nuniform sampler2D    uFoamMap         : register(s" + Ogre::StringConverter::toString(TexNum) + ")\n");
						}

						FragmentProgramData +=
							Ogre::String(                                            ")\n") +
							"{\n"     +
							    "float2 ProjectionCoord = iUvProjection.xy / iUvProjection.w;\n" +
                                "float3 camToSurface = iPosition.xyz - uEyePosition;\n" +
                                "float additionalReflection=camToSurface.x*camToSurface.x+camToSurface.z*camToSurface.z;\n";

						if (cFoam)
						{
							// Calculate the foam visibility as a function fo distance specified by user
							FragmentProgramData +=
								"float foamVisibility=1.0f-saturate(additionalReflection/uFoamMaxDistance);\n";
						}

						FragmentProgramData +=
							Ogre::String(
							    "additionalReflection/=uFullReflectionDistance;\n") +
								"camToSurface=normalize(-camToSurface);\n";
						if (Options.NM == NM_TEXTURE)
						{
							FragmentProgramData += Ogre::String(
								"float3 pixelNormal = tex2D(uNormalMap,iUvNoise);\n") +
								// Inverte y with z, because at creation our local normal to the plane was z
								"pixelNormal.yz=pixelNormal.zy;\n" +
								// Remap from [0,1] to [-1,1]
								"pixelNormal.xyz=-expand(pixelNormal.xyz);\n";
						}
						else if (Options.NM == NM_VERTEX)
						{
							FragmentProgramData +=
								"float3 pixelNormal = -iNormal;\n";
						}
						else // NM_RTT
						{
							FragmentProgramData +=
								"float3 pixelNormal = -(2.0*tex2D(uNormalMap, ProjectionCoord.xy) - 1.0);\n";
						}
						FragmentProgramData +=
								"float2 pixelNormalModified = uNormalDistortion*pixelNormal.zx;\n";
						if (Options.NM == NM_TEXTURE || Options.NM == NM_RTT)
						{
							FragmentProgramData +=
								"float dotProduct=dot(camToSurface,pixelNormal);\n";
						}
						else
						{
							FragmentProgramData +=
								"float dotProduct=dot(-camToSurface,pixelNormal);\n";
						}
						FragmentProgramData +=
							Ogre::String(
								"dotProduct=saturate(dotProduct);\n") +
								//"float fresnel = tex1D(uFresnelMap,dotProduct);\n" +
								"float fresnel = tex2D(uFresnelMap,float2(dotProduct,dotProduct));\n" +
								// Add additional reflection and saturate
								"fresnel+=additionalReflection;\n" +
								"fresnel=saturate(fresnel);\n" +
								// Decrease the transparency and saturate
								"fresnel-=uGlobalTransparency;\n" +
                                "fresnel=saturate(fresnel);\n" +
								// Get the reflection/refraction pixels. Make sure to disturb the texcoords by pixelnormal
								"float4 refraction=tex2D(uRefractionMap,ProjectionCoord.xy-pixelNormalModified);\n";
						if (cUReflections)
						{
							FragmentProgramData +=
								"float4 reflection=tex2D(uReflectionMap,ProjectionCoord.xy+pixelNormalModified);\n";
						}
						else
						{
							FragmentProgramData +=
								"float4 reflection=float4(uWaterColor,1);\n";
						}

						if (cDepth && cUReflections)
						{
							if (cCaustics)
						    {
								FragmentProgramData += Ogre::String(
								"float2 depth = tex2D(uDepthReflectionMap,ProjectionCoord.xy+pixelNormalModified).rg;\n") +
								"reflection *= 1+depth.y*uCausticsPower;\n" +
								"reflection = lerp(float4(uWaterColor,1),reflection,depth.x);\n";
						    }
							else
							{
								FragmentProgramData += Ogre::String(
								"float depth = tex2D(uDepthReflectionMap,ProjectionCoord.xy-pixelNormalModified).r;\n") +
								"reflection = lerp(float4(uWaterColor,1),reflection,depth);\n";
							}
						}

						FragmentProgramData +=
								"oColor = lerp(refraction,reflection,fresnel);\n";

						if (cSun)
						{
							FragmentProgramData += Ogre::String(
							    "float3 refractedVector = normalize(reflect(camToSurface, pixelNormal.xyz));\n") +
								"float3 surfaceToSun=normalize(uSunPosition-iPosition.xyz);\n" +
								// Temporally solution, fix this
								"surfaceToSun.xz = -surfaceToSun.xz;" +
								"float3 sunlight = uSunStrength*pow(saturate(dot(refractedVector,surfaceToSun)),uSunArea)*uSunColor;\n" +
								"oColor.xyz+=sunlight*saturate(1-additionalReflection);\n";
						}

						if (cFoam)
						{
							FragmentProgramData += Ogre::String(
							    "float hmap = iPosition.y/uFoamRange*foamVisibility;\n") +
								"float2 foamTex=iWorldPosition.xz*uFoamScale+pixelNormalModified;\n" +
								"float foam=tex2D(uFoamMap,foamTex).r;\n" +
								"float foamTransparency=saturate(hmap-uFoamStart)*uFoamTransparency;\n" +
								"oColor=lerp(oColor,float4(1,1,1,1),foamTransparency*foam);\n";
						}

						FragmentProgramData +=
							"}\n";
					}
					break;

					case SM_GLSL:
					{}
					break;
				}
		    }
		    break;
		}

		// Second: build our material
		Ogre::MaterialPtr &UnderwaterMaterial = getMaterial(MAT_UNDERWATER);
		UnderwaterMaterial = Ogre::MaterialManager::getSingleton().
			create(_def_Underwater_Material_Name,
			       HYDRAX_RESOURCE_GROUP);

		Ogre::Pass *UM_Technique0_Pass0 = UnderwaterMaterial->getTechnique(0)->getPass(0);

		UM_Technique0_Pass0->setDepthWriteEnabled(true);
        UM_Technique0_Pass0->setCullingMode(Ogre::CULL_NONE);

		Ogre::String GpuProgramsData[2] = {VertexProgramData, FragmentProgramData};
		Ogre::String GpuProgramNames[2] = {_def_Underwater_Shader_VP_Name, _def_Underwater_Shader_FP_Name};
		Ogre::String EntryPoints[2]     = {"main_vp", "main_fp"};

		fillGpuProgramsToPass(UM_Technique0_Pass0, GpuProgramNames, Options.SM, EntryPoints, GpuProgramsData);

		Ogre::GpuProgramParametersSharedPtr VP_Parameters = UM_Technique0_Pass0->getVertexProgramParameters();
		Ogre::GpuProgramParametersSharedPtr FP_Parameters = UM_Technique0_Pass0->getFragmentProgramParameters();

		VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		if (cFoam)
		{
		    VP_Parameters->setNamedAutoConstant("uWorld",     Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
		}
		FP_Parameters->setNamedAutoConstant("uEyePosition",   Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);

		FP_Parameters->setNamedConstant("uFullReflectionDistance", mHydrax->getFullReflectionDistance());
		FP_Parameters->setNamedConstant("uGlobalTransparency",     mHydrax->getGlobalTransparency());
		FP_Parameters->setNamedConstant("uNormalDistortion",       mHydrax->getNormalDistortion());

		if ((cDepth && cUReflections) || (!cUReflections))
		{
		    FP_Parameters->setNamedConstant("uWaterColor", mHydrax->getWaterColor());
		}

		if (cSun)
		{
			FP_Parameters->setNamedConstant("uSunPosition", mHydrax->getMesh()->getObjectSpacePosition(mHydrax->getSunPosition()));
			FP_Parameters->setNamedConstant("uSunStrength", mHydrax->getSunStrength());
			FP_Parameters->setNamedConstant("uSunArea",     mHydrax->getSunArea());
			FP_Parameters->setNamedConstant("uSunColor",    mHydrax->getSunColor());
		}
		if (cFoam)
		{
			FP_Parameters->setNamedConstant("uFoamRange",        mHydrax->getMesh()->getOptions().MeshStrength);
			FP_Parameters->setNamedConstant("uFoamMaxDistance",  mHydrax->getFoamMaxDistance());
			FP_Parameters->setNamedConstant("uFoamScale",        mHydrax->getFoamScale());
			FP_Parameters->setNamedConstant("uFoamStart",        mHydrax->getFoamStart());
			FP_Parameters->setNamedConstant("uFoamTransparency", mHydrax->getFoamTransparency());
		}
		if (cCaustics && cDepth && cUReflections)
		{
			FP_Parameters->setNamedConstant("uCausticsPower", mHydrax->getCausticsPower());
		}

		if (Options.NM == NM_TEXTURE || Options.NM == NM_RTT)
		{
		    UM_Technique0_Pass0->createTextureUnitState("HydraxNormalMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}

		if (cUReflections)
		{
		    UM_Technique0_Pass0->createTextureUnitState("HydraxReflectionMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		}
		UM_Technique0_Pass0->createTextureUnitState("HydraxRefractionMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

		if (cDepth && cUReflections)
		{
			UM_Technique0_Pass0->createTextureUnitState()->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		}

		UM_Technique0_Pass0->createTextureUnitState("Fresnel.bmp")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

		if (cFoam)
		{
			UM_Technique0_Pass0->createTextureUnitState("Foam.png")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}

		UnderwaterMaterial->setReceiveShadows(false);
		UnderwaterMaterial->load();

		return true;
	}

	bool MaterialManager::_createUnderwaterCompositor(const HydraxComponent &Components, const Options &Options)
	{
		const bool cCaustics   = _isComponent(Components, HYDRAX_COMPONENT_CAUSTICS);
		const bool cDepth   = _isComponent(Components, HYDRAX_COMPONENT_DEPTH);
		const bool cGodRays = _isComponent(Components, HYDRAX_COMPONENT_UNDERWATER_GODRAYS);

		Ogre::String VertexProgramData, FragmentProgramData;

		// Vertex program

		switch (Options.SM)
		{
		    case SM_HLSL: case SM_CG:
			{
				VertexProgramData +=
					Ogre::String(
					"void main_vp(\n") +
					    // IN
						"float4 iPosition      : POSITION,\n" +
						// OUT
						"out float4 oPosition  : POSITION,\n" +
						"out float3 oPosition_ : TEXCOORD0,\n" +
						"out float2 oUV        : TEXCOORD1,\n";
						// UNIFORM
				        if (cGodRays)
						{
							VertexProgramData +=
								Ogre::String(
				        "uniform float3   uCorner0,\n") +
		                "uniform float3   uCorner01,\n" +
		                "uniform float3   uCorner02,\n";
						}
						VertexProgramData +=
							Ogre::String(
						"uniform float4x4 uWorldViewProj)\n") +
					"{\n" +
					    "oPosition = mul(uWorldViewProj, iPosition);\n"+
						"iPosition.xy = sign(iPosition.xy);\n"+
						"oUV = (float2(iPosition.x, -iPosition.y) + 1.0f) * 0.5f;";
						if (cGodRays)
						{
							VertexProgramData +=
								Ogre::String(
						"uCorner01 *= oUV.x;\n")+
	                    "uCorner02 *= oUV.y;\n"+
	                    "oPosition_ = uCorner0+uCorner01+uCorner02;";
						}
						VertexProgramData +=
					"}\n";
			}
			break;

			case SM_GLSL:
			{}
			break;
		}

		// Fragment program

		switch (Options.SM)
		{
		    case SM_HLSL: case SM_CG:
			{
				FragmentProgramData +=
					Ogre::String(
				    "void main_fp(\n") +
						// IN
						"float3  iPosition    : TEXCOORD0,\n" +
						"float2  iUV          : TEXCOORD1,\n" +
					    // OUT
						"out float4 oColor    : COLOR,\n";
						// UNIFORM
					if (cDepth)
					{
					FragmentProgramData +=
						"uniform float3       uWaterColor,\n";
					}
					if (cCaustics)
					{
					FragmentProgramData +=
						"uniform float        uCausticsPower,\n";
					}
					if (cGodRays)
					{
					FragmentProgramData +=
						Ogre::String(
						"uniform float3  uSunColor,\n") +
						"uniform float3  uLightDirection,\n"+
                        "uniform float   uIntensity,\n"+
	                    "uniform float3  uHGg,\n"+
	                    "uniform float3  uCameraPos,\n";
					}
					FragmentProgramData +=
						Ogre::String(
						"uniform float        uTime,\n") +
						"uniform sampler2D    uOriginalMap   : register(s0),\n"  +
						"uniform sampler2D    uDistortMap    : register(s1)\n";
					if (cDepth)
					{
					FragmentProgramData +=
					    ",\nuniform sampler2D    uDepthMap      : register(s2)";
					}
					FragmentProgramData +=
						Ogre::String
					(")\n{\n") +
					    "float2 distortUV = ((tex2D(uDistortMap, float2(iUV.x + uTime, iUV.y + uTime)).xy - 0.5))/50;\n";
					if (cCaustics) // Depth, caustics
					{
					FragmentProgramData +=
						Ogre::String(
						"float2 depth = tex2D(uDepthMap, iUV+distortUV).xy;\n") +
						"oColor = lerp(float4(uWaterColor,1),tex2D(uOriginalMap, iUV+distortUV)*(1+depth.y*uCausticsPower), depth.x);\n";
						if (cGodRays)
						{
						FragmentProgramData +=
							Ogre::String(
					        "float3 view_vector = normalize(iPosition-uCameraPos);") +
							"float dot_product = dot(view_vector, -uLightDirection);"+
                        	"float num = uHGg.x;"+
                        	"float den = (uHGg.y - uHGg.z*dot_product); "+
                       	    "den = rsqrt(den); "+
                       	    "float phase = num * (den*den*den);"+
							"oColor.xyz += (0.15 + uIntensity*tex2D(uDepthMap, iUV).z)*phase*uSunColor;";
						}
					}
					else if (cDepth) // Depth, no caustics
					{
				    FragmentProgramData +=
						"oColor = lerp(float4(uWaterColor,1),tex2D(uOriginalMap, iUV+distortUV),tex2D(uDepthMap, iUV+distortUV).r);\n";
					    if (cGodRays)
						{
						FragmentProgramData +=
							Ogre::String(
							"float3 view_vector = normalize(iPosition-uCameraPos);") +
							"float dot_product = dot(view_vector, -uLightDirection);"+
                        	"float num = uHGg.x;"+
                        	"float den = (uHGg.y - uHGg.z*dot_product); "+
                       	    "den = rsqrt(den); "+
                       	    "float phase = num * (den*den*den);"+
							"oColor.xyz += (0.15 + uIntensity*tex2D(uDepthMap, iUV).y)*phase*uSunColor;";
						}
					}
					else // No depth, no caustics
					{
					FragmentProgramData +=
						"oColor = tex2D(uOriginalMap, iUV+distortUV);";
					}
					FragmentProgramData +=
					"}\n";
			}
			break;

			case SM_GLSL:
			{}
			break;
		}

		// Build our material
		Ogre::MaterialPtr &UnderwaterCompositorMaterial = getMaterial(MAT_UNDERWATER_COMPOSITOR);
		UnderwaterCompositorMaterial = Ogre::MaterialManager::getSingleton().
			create(_def_Underwater_Compositor_Material_Name,
			       HYDRAX_RESOURCE_GROUP);

		Ogre::Pass *DM_Technique0_Pass0 = UnderwaterCompositorMaterial->getTechnique(0)->getPass(0);

		DM_Technique0_Pass0->setCullingMode(Ogre::CULL_NONE);
		DM_Technique0_Pass0->setDepthFunction(Ogre::CMPF_ALWAYS_PASS);

		Ogre::String GpuProgramsData[2] = {VertexProgramData, FragmentProgramData};
		Ogre::String GpuProgramNames[2] = {_def_Underwater_Compositor_Shader_VP_Name, _def_Underwater_Compositor_Shader_FP_Name};
		Ogre::String EntryPoints[2]     = {"main_vp", "main_fp"};

		fillGpuProgramsToPass(DM_Technique0_Pass0, GpuProgramNames, Options.SM, EntryPoints, GpuProgramsData);

		Ogre::GpuProgramParametersSharedPtr VP_Parameters = DM_Technique0_Pass0->getVertexProgramParameters();
		Ogre::GpuProgramParametersSharedPtr FP_Parameters = DM_Technique0_Pass0->getFragmentProgramParameters();

		VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);

		if (cDepth)
		{
		    FP_Parameters->setNamedConstant("uWaterColor", mHydrax->getWaterColor());
		}
		FP_Parameters->setNamedConstantFromTime("uTime", 0.1f);

		if (cCaustics)
		{
			FP_Parameters->setNamedConstant("uCausticsPower", mHydrax->getCausticsPower());
		}

		if (cGodRays)
		{
			FP_Parameters->setNamedConstant("uSunColor", mHydrax->getSunColor());
			FP_Parameters->setNamedConstant("uLightDirection",
				(mHydrax->getMesh()->getObjectSpacePosition(mHydrax->getCamera()->getPosition()) -
				 mHydrax->getMesh()->getObjectSpacePosition(mHydrax->getSunPosition()))
			     .normalisedCopy());
		    FP_Parameters->setNamedConstant("uIntensity", mHydrax->getGodRaysIntensity());
		    FP_Parameters->setNamedConstant("uHGg", mHydrax->getGodRaysExposure());
		    FP_Parameters->setNamedAutoConstant("uCameraPos", Ogre::GpuProgramParameters::ACT_CAMERA_POSITION);
		}

		// From compositor, original scene
		DM_Technique0_Pass0->createTextureUnitState()->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		DM_Technique0_Pass0->createTextureUnitState("UnderwaterDistortion.jpg")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		if (cDepth)
		{
		    DM_Technique0_Pass0->createTextureUnitState("HydraxDepthMap")->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		}

		UnderwaterCompositorMaterial->setReceiveShadows(false);
		UnderwaterCompositorMaterial->load();

		Ogre::CompositorPtr &UnderwaterCompositor = getCompositor(COMP_UNDERWATER);
		UnderwaterCompositor = Ogre::CompositorManager::getSingleton().
			create(_def_Underwater_Compositor_Name, HYDRAX_RESOURCE_GROUP);

		Ogre::CompositionTechnique* UnderWaterComp_Technique = UnderwaterCompositor->createTechnique();

		// Create the texture definition to render the original scene
		Ogre::CompositionTechnique::TextureDefinition* TDef = UnderWaterComp_Technique->createTextureDefinition("OriginalScene");
        TDef->width = 0;
        TDef->height = 0;
		Ogre::PixelFormatList l;
		l.push_back(Ogre::PF_A8R8G8B8);
		TDef->formatList = l;

		// Render the original scene
        Ogre::CompositionTargetPass* CTPass = UnderWaterComp_Technique->createTargetPass();
		CTPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
        CTPass->setOutputName("OriginalScene");
		Ogre::CompositionPass* CPassClear = CTPass->createPass();
		CPassClear->setType(Ogre::CompositionPass::PT_CLEAR);

		const Ogre::Vector3& WC = mHydrax->getWaterColor();
		CPassClear->setClearColour(Ogre::ColourValue(WC.x, WC.y, WC.z));

		Ogre::CompositionPass* CPass = CTPass->createPass();
		CPass->setType(Ogre::CompositionPass::PT_RENDERSCENE);
		CPass->setFirstRenderQueue(Ogre::RENDER_QUEUE_SKIES_EARLY+1);

		// Build the output target pass
        Ogre::CompositionTargetPass* CTOutputPass = UnderWaterComp_Technique->getOutputTargetPass();
        CTOutputPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);

		// Final composition pass
	    Ogre::CompositionPass* COutputPass = CTOutputPass->createPass();
	    COutputPass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
		COutputPass->setMaterial(getMaterial(MAT_UNDERWATER_COMPOSITOR));
        COutputPass->setInput(0, "OriginalScene");
		COutputPass->setLastRenderQueue(0);

        Ogre::CompositorManager::getSingleton().
			addCompositor(mHydrax->getViewport(),_def_Underwater_Compositor_Name)->
			    addListener(&mUnderwaterCompositorListener);

		return true;
	}

	bool MaterialManager::_createSimpleColorMaterial(const Ogre::ColourValue& Color, const MaterialType& MT, const Ogre::String& Name, const bool& DepthCheck, const bool& DepthWrite)
	{
		Ogre::MaterialPtr &SimpleColorMaterial = getMaterial(MT);
		SimpleColorMaterial = Ogre::MaterialManager::getSingleton().
			create(Name,
			       HYDRAX_RESOURCE_GROUP);

		Ogre::Pass *SCM_T0_Pass0 = SimpleColorMaterial->getTechnique(0)->getPass(0);
		SCM_T0_Pass0->setLightingEnabled(false);
		SCM_T0_Pass0->setDepthCheckEnabled(DepthCheck);
		SCM_T0_Pass0->setDepthWriteEnabled(DepthWrite);
		SCM_T0_Pass0->setCullingMode(Ogre::CULL_NONE);
		SCM_T0_Pass0->createTextureUnitState()->setColourOperationEx(Ogre::LBX_MODULATE,Ogre::LBS_MANUAL,Ogre::LBS_CURRENT, Color);

		SimpleColorMaterial->setReceiveShadows(false);
		SimpleColorMaterial->load();

		return true;
	}

	void MaterialManager::reload(const MaterialType &Material)
	{
		Ogre::MaterialPtr &Mat = getMaterial(Material);

		if (Mat.isNull())
		{
			return;
		}

		Mat->reload();

		const bool cDepth    = _isComponent(mComponents, HYDRAX_COMPONENT_DEPTH   );
		const bool cSmooth   = _isComponent(mComponents, HYDRAX_COMPONENT_SMOOTH  );
		const bool cSun      = _isComponent(mComponents, HYDRAX_COMPONENT_SUN     );
		const bool cFoam     = _isComponent(mComponents, HYDRAX_COMPONENT_FOAM    );
		const bool cCaustics = _isComponent(mComponents, HYDRAX_COMPONENT_CAUSTICS);
		const bool cUReflections = _isComponent(mComponents, HYDRAX_COMPONENT_UNDERWATER_REFLECTIONS);

		switch (Material)
		{
		    case MAT_WATER:
			{
				Ogre::Pass *M_Technique0_Pass0 = Mat->getTechnique(0)->getPass(0);

				switch (mOptions.NM)
				{
				    case NM_TEXTURE: case NM_RTT:
					{
						M_Technique0_Pass0->getTextureUnitState(0)->setTextureName("HydraxNormalMap");
						M_Technique0_Pass0->getTextureUnitState(1)->setTextureName("HydraxReflectionMap");
						M_Technique0_Pass0->getTextureUnitState(2)->setTextureName("HydraxRefractionMap");
						if (cDepth)
						{
						    M_Technique0_Pass0->getTextureUnitState(3)->setTextureName("HydraxDepthMap");
						}
					}
					break;

					case NM_VERTEX:
					{
						M_Technique0_Pass0->getTextureUnitState(0)->setTextureName("HydraxReflectionMap");
						M_Technique0_Pass0->getTextureUnitState(1)->setTextureName("HydraxRefractionMap");
						if (cDepth)
						{
						    M_Technique0_Pass0->getTextureUnitState(2)->setTextureName("HydraxDepthMap");
						}
					}
					break;
				}

				Ogre::GpuProgramParametersSharedPtr VP_Parameters = M_Technique0_Pass0->getVertexProgramParameters();
				Ogre::GpuProgramParametersSharedPtr FP_Parameters = M_Technique0_Pass0->getFragmentProgramParameters();

				VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
				if (cFoam)
				{
					VP_Parameters->setNamedAutoConstant("uWorld", Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
				}
				FP_Parameters->setNamedAutoConstant("uEyePosition",   Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);

				FP_Parameters->setNamedConstant("uFullReflectionDistance", mHydrax->getFullReflectionDistance());
				FP_Parameters->setNamedConstant("uGlobalTransparency",     mHydrax->getGlobalTransparency());
				FP_Parameters->setNamedConstant("uNormalDistortion",       mHydrax->getNormalDistortion());

				if (cDepth)
				{
					FP_Parameters->setNamedConstant("uWaterColor", mHydrax->getWaterColor());
				}
				if (cSmooth)
				{
					FP_Parameters->setNamedConstant("uSmoothPower", mHydrax->getSmoothPower());
				}
				if (cSun)
				{
					FP_Parameters->setNamedConstant("uSunPosition", mHydrax->getMesh()->getObjectSpacePosition(mHydrax->getSunPosition()));
					FP_Parameters->setNamedConstant("uSunStrength", mHydrax->getSunStrength());
					FP_Parameters->setNamedConstant("uSunArea",     mHydrax->getSunArea());
					FP_Parameters->setNamedConstant("uSunColor",    mHydrax->getSunColor());
				}
				if (cFoam)
				{
			        FP_Parameters->setNamedConstant("uFoamRange",        mHydrax->getMesh()->getOptions().MeshStrength);
					FP_Parameters->setNamedConstant("uFoamMaxDistance",  mHydrax->getFoamMaxDistance());
					FP_Parameters->setNamedConstant("uFoamScale",        mHydrax->getFoamScale());
					FP_Parameters->setNamedConstant("uFoamStart",        mHydrax->getFoamStart());
					FP_Parameters->setNamedConstant("uFoamTransparency", mHydrax->getFoamTransparency());
				}
				if (cCaustics)
				{
					FP_Parameters->setNamedConstant("uCausticsPower",    mHydrax->getCausticsPower());
				}
			}
			break;

			case MAT_DEPTH:
			{
				Ogre::Pass *M_Technique0_Pass0 = Mat->getTechnique(0)->getPass(0);

				Ogre::GpuProgramParametersSharedPtr VP_Parameters = M_Technique0_Pass0->getVertexProgramParameters();
				Ogre::GpuProgramParametersSharedPtr FP_Parameters = M_Technique0_Pass0->getFragmentProgramParameters();

				VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
				VP_Parameters->setNamedAutoConstant("uWorld", Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
				VP_Parameters->setNamedConstant("uPlaneYPos", mHydrax->getPosition().y);

				FP_Parameters->setNamedConstant("uDepthLimit", 1/mHydrax->getDepthLimit());

				if (cCaustics)
				{
					FP_Parameters->setNamedConstant("uCausticsScale", mHydrax->getCausticsScale());
					FP_Parameters->setNamedConstant("uCausticsEnd",   mHydrax->getCausticsEnd());
				}
			}
			break;

			case MAT_UNDERWATER:
			{
				Ogre::Pass *M_Technique0_Pass0 = Mat->getTechnique(0)->getPass(0);

				switch (mOptions.NM)
				{
				    case NM_TEXTURE: case NM_RTT:
					{
						M_Technique0_Pass0->getTextureUnitState(0)->setTextureName("HydraxNormalMap");
						int Index = 1;
						if (cUReflections)
						{
						    M_Technique0_Pass0->getTextureUnitState(Index)->setTextureName("HydraxReflectionMap");
							Index++;
						}
						M_Technique0_Pass0->getTextureUnitState(Index)->setTextureName("HydraxRefractionMap");
						Index++;
						if (cDepth && cUReflections)
						{
						    M_Technique0_Pass0->getTextureUnitState(Index)->setTextureName("HydraxDepthReflectionMap");
						}
					}
					break;

					case NM_VERTEX:
					{
						int Index = 0;
						if (cUReflections)
						{
						    M_Technique0_Pass0->getTextureUnitState(Index)->setTextureName("HydraxReflectionMap");
							Index++;
						}
						M_Technique0_Pass0->getTextureUnitState(Index)->setTextureName("HydraxRefractionMap");
						Index++;
						if (cDepth && cUReflections)
						{
						    M_Technique0_Pass0->getTextureUnitState(Index)->setTextureName("HydraxDepthReflectionMap");
						}
					}
					break;
				}

				Ogre::GpuProgramParametersSharedPtr VP_Parameters = M_Technique0_Pass0->getVertexProgramParameters();
				Ogre::GpuProgramParametersSharedPtr FP_Parameters = M_Technique0_Pass0->getFragmentProgramParameters();

				VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
				if (cFoam)
				{
		            VP_Parameters->setNamedAutoConstant("uWorld",     Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
		        }
				FP_Parameters->setNamedAutoConstant("uEyePosition",   Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);

				FP_Parameters->setNamedConstant("uFullReflectionDistance", mHydrax->getFullReflectionDistance());
				FP_Parameters->setNamedConstant("uGlobalTransparency",     mHydrax->getGlobalTransparency());
				FP_Parameters->setNamedConstant("uNormalDistortion",       mHydrax->getNormalDistortion());

				if ((cDepth && cUReflections) || (!cUReflections))
		        {
		            FP_Parameters->setNamedConstant("uWaterColor", mHydrax->getWaterColor());
		        }

				if (cSun)
				{
					FP_Parameters->setNamedConstant("uSunPosition", mHydrax->getMesh()->getObjectSpacePosition(mHydrax->getSunPosition()));
					FP_Parameters->setNamedConstant("uSunStrength", mHydrax->getSunStrength());
					FP_Parameters->setNamedConstant("uSunArea",     mHydrax->getSunArea());
					FP_Parameters->setNamedConstant("uSunColor",    mHydrax->getSunColor());
				}
				if (cFoam)
				{
			        FP_Parameters->setNamedConstant("uFoamRange",        mHydrax->getMesh()->getOptions().MeshStrength);
					FP_Parameters->setNamedConstant("uFoamMaxDistance",  mHydrax->getFoamMaxDistance());
					FP_Parameters->setNamedConstant("uFoamScale",        mHydrax->getFoamScale());
					FP_Parameters->setNamedConstant("uFoamStart",        mHydrax->getFoamStart());
					FP_Parameters->setNamedConstant("uFoamTransparency", mHydrax->getFoamTransparency());
				}
				if (cCaustics && cDepth && cUReflections)
				{
					FP_Parameters->setNamedConstant("uCausticsPower",    mHydrax->getCausticsPower());
				}
			}
			break;

			case MAT_UNDERWATER_COMPOSITOR:
			{
				mCompositorsNeedToBeReloaded[COMP_UNDERWATER] = true;
			}
			break;
		}
	}

	void MaterialManager::addDepthTechnique(Ogre::Technique *Technique, const bool& AutoUpdate)
	{
		if (!Ogre::MaterialManager::getSingleton().resourceExists(_def_Depth_Material_Name))
		{
			_createDepthMaterial(mComponents, mOptions);
		}

		Technique->removeAllPasses();
		Technique->createPass();
		Technique->setSchemeName("HydraxDepth");

		Ogre::Pass *DM_Technique_Pass0 = Technique->getPass(0);

		DM_Technique_Pass0->setVertexProgram(_def_Depth_Shader_VP_Name);
		DM_Technique_Pass0->setFragmentProgram(_def_Depth_Shader_FP_Name);

		Ogre::GpuProgramParametersSharedPtr VP_Parameters = DM_Technique_Pass0->getVertexProgramParameters();
		Ogre::GpuProgramParametersSharedPtr FP_Parameters = DM_Technique_Pass0->getFragmentProgramParameters();

		VP_Parameters->setNamedAutoConstant("uWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		VP_Parameters->setNamedAutoConstant("uWorld", Ogre::GpuProgramParameters::ACT_WORLD_MATRIX);
		VP_Parameters->setNamedConstant("uPlaneYPos", mHydrax->getPosition().y);

		FP_Parameters->setNamedConstant("uDepthLimit", 1/mHydrax->getDepthLimit());

		if (_isComponent(mComponents, HYDRAX_COMPONENT_CAUSTICS))
		{
			FP_Parameters->setNamedConstant("uCausticsScale", mHydrax->getCausticsScale());
			FP_Parameters->setNamedConstant("uCausticsEnd",   mHydrax->getCausticsEnd());

			Ogre::TextureUnitState *TUS_Caustics = DM_Technique_Pass0->createTextureUnitState("Caustics.bmp");
			TUS_Caustics->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
			TUS_Caustics->setAnimatedTextureName("Caustics.bmp", 32, 1.5);
		}

		if (AutoUpdate)
		{
		    mDepthTechniques.push_back(Technique);
		}
	}

	void MaterialManager::setCompositorEnable(const CompositorType &Compositor, const bool &Enable)
	{
		Ogre::CompositorPtr &Comp = mCompositors[static_cast<int>(Compositor)];

		if (Comp.isNull())
		{
			return;
		}

		Ogre::CompositorManager::getSingleton().
					setCompositorEnabled(mHydrax->getViewport(), Comp->getName(), Enable);

		mCompositorsEnable[static_cast<int>(Compositor)] = Enable;
	}

	bool MaterialManager::_isComponent(const HydraxComponent &List, const HydraxComponent &ToCheck) const
	{
		if (List & ToCheck)
        {
            return true;
        }

        if (ToCheck == HYDRAX_COMPONENTS_NONE && List == HYDRAX_COMPONENTS_NONE)
        {
            return true;
        }

        if (ToCheck == HYDRAX_COMPONENTS_ALL && List == HYDRAX_COMPONENTS_ALL)
        {
            return true;
        }

        return false;
	}

	void MaterialManager::setGpuProgramParameter(const GpuProgram &GpuP, const MaterialType &MType, const Ogre::String &Name, const Ogre::Real &Value)
	{
		if (!mCreated)
		{
			return;
		}

		Ogre::GpuProgramParametersSharedPtr Parameters;

		switch (GpuP)
		{
		    case GPUP_VERTEX:
			{
				Parameters = getMaterial(MType)->getTechnique(0)->getPass(0)->getVertexProgramParameters();
			}
			break;

			case GPUP_FRAGMENT:
			{
				Parameters = getMaterial(MType)->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			}
			break;
		}

		Parameters->setNamedConstant(Name, Value);

		if (MType == MAT_DEPTH)
		{
			std::vector<Ogre::Technique*>::iterator TechIt;

			for(TechIt = mDepthTechniques.begin(); TechIt != mDepthTechniques.end(); TechIt++)
			{
				if (!(*TechIt))
				{
					mDepthTechniques.erase(TechIt);
					continue;
				}

				switch (GpuP)
				{
		  		    case GPUP_VERTEX:
				    {
					    Parameters = (*TechIt)->getPass(0)->getVertexProgramParameters();
				    }
				    break;

				    case GPUP_FRAGMENT:
				    {
					    Parameters = (*TechIt)->getPass(0)->getFragmentProgramParameters();
				    }
				    break;
			    }

				Parameters->setNamedConstant(Name, Value);
			}
		}
	}

	void MaterialManager::setGpuProgramParameter(const GpuProgram &GpuP, const MaterialType &MType, const Ogre::String &Name, const Ogre::Vector2 &Value)
	{
		if (!mCreated)
		{
			return;
		}

		Ogre::GpuProgramParametersSharedPtr Parameters;

		switch (GpuP)
		{
		    case GPUP_VERTEX:
			{
				Parameters = getMaterial(MType)->getTechnique(0)->getPass(0)->getVertexProgramParameters();
			}
			break;

			case GPUP_FRAGMENT:
			{
				Parameters = getMaterial(MType)->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			}
			break;
		}

		float Value_[2] = {Value.x, Value.y};

		if (MType == MAT_DEPTH)
		{
			std::vector<Ogre::Technique*>::iterator TechIt;

			for(TechIt = mDepthTechniques.begin(); TechIt != mDepthTechniques.end(); TechIt++)
			{
				if (!(*TechIt))
				{
					mDepthTechniques.erase(TechIt);
					continue;
				}

				switch (GpuP)
				{
		  		    case GPUP_VERTEX:
				    {
					    Parameters = (*TechIt)->getPass(0)->getVertexProgramParameters();
				    }
				    break;

				    case GPUP_FRAGMENT:
				    {
					    Parameters = (*TechIt)->getPass(0)->getFragmentProgramParameters();
				    }
				    break;
			    }

				Parameters->setNamedConstant(Name, Value_, 1, 2);
			}
		}
	}

	void MaterialManager::setGpuProgramParameter(const GpuProgram &GpuP, const MaterialType &MType, const Ogre::String &Name, const Ogre::Vector3 &Value)
	{
		if (!mCreated)
		{
			return;
		}

		Ogre::GpuProgramParametersSharedPtr Parameters;

		switch (GpuP)
		{
		    case GPUP_VERTEX:
			{
				Parameters = getMaterial(MType)->getTechnique(0)->getPass(0)->getVertexProgramParameters();
			}
			break;

			case GPUP_FRAGMENT:
			{
				Parameters = getMaterial(MType)->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			}
			break;
		}

		Parameters->setNamedConstant(Name, Value);

		if (MType == MAT_DEPTH)
		{
			std::vector<Ogre::Technique*>::iterator TechIt;

			for(TechIt = mDepthTechniques.begin(); TechIt != mDepthTechniques.end(); TechIt++)
			{
				if (!(*TechIt))
				{
					mDepthTechniques.erase(TechIt);
					continue;
				}

				switch (GpuP)
				{
		  		    case GPUP_VERTEX:
				    {
					    Parameters = (*TechIt)->getPass(0)->getVertexProgramParameters();
				    }
				    break;

				    case GPUP_FRAGMENT:
				    {
					    Parameters = (*TechIt)->getPass(0)->getFragmentProgramParameters();
				    }
				    break;
			    }

				Parameters->setNamedConstant(Name, Value);
			}
		}
	}

	void MaterialManager::UnderwaterCompositorListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
	{
	}

	void MaterialManager::UnderwaterCompositorListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
	{
		const Ogre::Vector3& WC = mMaterialManager->mHydrax->getWaterColor();
		Ogre::CompositorPtr &UnderwaterCompositor = mMaterialManager->getCompositor(COMP_UNDERWATER);
		UnderwaterCompositor->getTechnique(0)->getTargetPass(0)->getPass(0)->setClearColour(Ogre::ColourValue(WC.x, WC.y, WC.z));

		Ogre::GpuProgramParametersSharedPtr FP_Parameters = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

		if (mMaterialManager->mHydrax->isComponent(HYDRAX_COMPONENT_DEPTH))
		{
			FP_Parameters->
				setNamedConstant("uWaterColor", mMaterialManager->mHydrax->getWaterColor());
		}

		if (mMaterialManager->mHydrax->isComponent(HYDRAX_COMPONENT_CAUSTICS))
		{
			FP_Parameters->
				setNamedConstant("uCausticsPower", mMaterialManager->mHydrax->getCausticsPower());
		}

		if (mMaterialManager->mHydrax->isComponent(HYDRAX_COMPONENT_UNDERWATER_GODRAYS))
		{
			FP_Parameters->
				setNamedConstant("uSunColor", mMaterialManager->mHydrax->getSunColor());

			FP_Parameters->setNamedConstant("uLightDirection",
				   (mMaterialManager->mHydrax->getMesh()->getObjectSpacePosition(mMaterialManager->mHydrax->getCamera()->getPosition()) -
				    mMaterialManager->mHydrax->getMesh()->getObjectSpacePosition(mMaterialManager->mHydrax->getSunPosition()))
					.normalisedCopy());

			FP_Parameters->setNamedConstant("uIntensity", mMaterialManager->mHydrax->getGodRaysIntensity());
		    FP_Parameters->setNamedConstant("uHGg", mMaterialManager->mHydrax->getGodRaysExposure());

			Ogre::GpuProgramParametersSharedPtr VP_Parameters = mat->getTechnique(0)->getPass(0)->getVertexProgramParameters();

		    // FAR_LEFT_TOP
		    VP_Parameters->
			    setNamedConstant( "uCorner0", mMaterialManager->mHydrax->getCamera()->getWorldSpaceCorners()[5] );
		    // FAR_RIGHT_TOP - FAR_LEFT_TOP
		    VP_Parameters->
		    	setNamedConstant( "uCorner01",  mMaterialManager->mHydrax->getCamera()->getWorldSpaceCorners()[4] -  mMaterialManager->mHydrax->getCamera()->getWorldSpaceCorners()[5]);
		    // FAR_LEFT_BOTTOM - FAR_LEFT_TOP
		    VP_Parameters->
			    setNamedConstant( "uCorner02",  mMaterialManager->mHydrax->getCamera()->getWorldSpaceCorners()[6] - mMaterialManager->mHydrax->getCamera()->getWorldSpaceCorners()[5]);
		}

		if (mMaterialManager->mCompositorsNeedToBeReloaded[COMP_UNDERWATER])
		{
			Ogre::Pass* DM_Technique0_Pass0 = mat->getTechnique(0)->getPass(0);

			if (mMaterialManager->_isComponent(mMaterialManager->mComponents, HYDRAX_COMPONENT_DEPTH))
		    {
				DM_Technique0_Pass0->getTextureUnitState(2)->setTextureName("HydraxDepthMap");
		    }

			mMaterialManager->mCompositorsNeedToBeReloaded[COMP_UNDERWATER] = false;
		}
	}
}
