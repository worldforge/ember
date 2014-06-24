#include "SBMotionBlendBase.h"
#include "SBMotion.h"
#include "SBCharacter.h"
#include "SBSkeleton.h"
#include "SBScene.h"

#include <boost/algorithm/string.hpp>
#include "components/sb/controllers/MeCtBlendEngine.h" // ?

namespace SmartBody {

SBMotionBlendBase::SBMotionBlendBase()
{
	_isFinalized = false;
	parameterDim = 3;	
	_dimension = "3D";
	blendEngine = NULL;		
	interpType = "KNN";
	blendType = "";
}

SBMotionBlendBase::SBMotionBlendBase(const std::string& name, const std::string& skelName, int dimension) : SBAnimationBlend(name)
{
	_isFinalized = false;
	parameterDim = 3;	
	_dimension = "3D";
	interpType = "KNN";

	SBSkeleton* sbSkel = SBScene::getScene()->getSkeleton(skelName);
	if (sbSkel)
	{
		skeletonName = skelName;
		blendEngine = new MeCtBlendEngine(sbSkel, "base");
	}
	else // error
	{
		blendEngine = NULL;		
	}
}

SBMotionBlendBase::~SBMotionBlendBase()
{
}

void SBMotionBlendBase::addMotion( const std::string& motion, std::vector<double>& parameter )
{
	addSkMotion(motion);
	setMotionParameter(motion, parameter);
}

void SBMotionBlendBase::removeMotion( const std::string& motionName )
{
	SBAnimationBlend::removeMotion(motionName);

	// remove correspondence points
	removeSkMotion(motionName);
}

bool SBMotionBlendBase::getWeightsFromParameters( double x, std::vector<double>& weights )
{	
	if (interpType == "Barycentric")
	{
		return PABlend::getWeightsFromParameters(x,weights);
	}
	dVector param;
	param.resize(1);
	param(0) = x;
	blendEngine->setBlendParameter(param, weights);	
	return true;
}

bool SBMotionBlendBase::getWeightsFromParameters( double x, double y, std::vector<double>& weights )
{
	if (interpType == "Barycentric")
	{
		return PABlend::getWeightsFromParameters(x,y,weights);
	}
	dVector param;
	param.resize(2);
	param(0) = x;
	param(1) = y;
	blendEngine->setBlendParameter(param, weights);
	return true;
}

bool SBMotionBlendBase::getWeightsFromParameters( double x, double y, double z, std::vector<double>& weights )
{
	if (interpType == "Barycentric")
	{
		return PABlend::getWeightsFromParameters(x,y,z,weights);
	}
	dVector param;
	param.resize(3);
	param(0) = x;
	param(1) = y;
	param(2) = z;
	blendEngine->setBlendParameter(param, weights);
	return true;
}

void SBMotionBlendBase::getParametersFromWeights( float& x, std::vector<double>& weights )
{
	dVector param;
	blendEngine->getBlendParameterFromWeights(param, weights);
	x = (float)param(0);
}

void SBMotionBlendBase::getParametersFromWeights( float& x, float& y, std::vector<double>& weights )
{
	dVector param;
	blendEngine->getBlendParameterFromWeights(param, weights);
	x = (float)param(0);
	y = (float)param(1);
}

void SBMotionBlendBase::getParametersFromWeights( float& x, float& y, float& z, std::vector<double>& weights )
{
	dVector param;
	blendEngine->getBlendParameterFromWeights(param, weights);
	x = (float)param(0);
	y = (float)param(1);
	z = (float)param(2);	
}

void SBMotionBlendBase::setMotionParameter( const std::string& motion, std::vector<double>& parameter )
{
	if (parameter.size() == 1)
		setParameter(motion,parameter[0]);
	else if (parameter.size() == 2)
		setParameter(motion,parameter[0],parameter[1]);
	else if (parameter.size() == 3)
		setParameter(motion,parameter[0],parameter[1],parameter[2]);
}

void SBMotionBlendBase::buildBlendBase( const std::string& motionParameter, const std::string& interpolatorType, bool copySimplex)
{
	blendType = motionParameter;
	LOG("build blend base");
	blendEngine->init(motionParameter);	
	LOG("num motions = %d, motion parameter = %s, interpolator type = %s",motions.size(), motionParameter.c_str(), interpolatorType.c_str());
	interpType = interpolatorType;	
	blendEngine->updateMotionExamples(motions, interpolatorType);	
	// update all motion parameters
	for (unsigned int i=0;i<motions.size();i++)
	{
		dVector motionParam;
		SkMotion* motion = motions[i];		
		std::vector<double> param = getMotionParameter(motion->getName());		
		setMotionParameter(motion->getName(), param);
	}
	// automatically generate tetrahedron
	if (copySimplex && blendEngine->simplexList)
	{
		VecOfSimplex& simplexList = *blendEngine->simplexList;
		for (unsigned int i=0;i<simplexList.size();i++)
		{
			std::vector<std::string> motionNameList;
			Simplex& simp = simplexList[i];
			if (simp.numDim != 3) continue; // only handle tetrahedrons

			int nvert = simp.vertexIndices.size();
			motionNameList.resize(nvert);
			for (int k=0;k<nvert;k++)
				motionNameList[k] = motions[simp.vertexIndices[k]]->getName();

			addTetrahedron(motionNameList[0],motionNameList[1],motionNameList[2],motionNameList[3]);
		}		
	}
	SBSkeleton* sbSkel = SBScene::getScene()->getSkeleton(skeletonName);
	SrVec center = SrVec(0,0,0);
	if (sbSkel)
	{
		SBJoint* baseJoint =  sbSkel->getJointByName("base");
		if (baseJoint)
			center = baseJoint->gmat().get_translation();

	//// 	createErrorSurfaces("curve", center, 0, 50, errorSurfaces);
	//// 	createErrorSurfaces("curve", center, 0, 80, smoothSurfaces);
	//// 	for (unsigned int i=0;i<errorSurfaces.size();i++)
	//// 	{
	//// 		SrSnColorSurf* surf = errorSurfaces[i];
	//// 		updateErrorSurace(surf, center);
	//// 	}
	//// 	for (unsigned int i=0;i<smoothSurfaces.size();i++)
	//// 	{
	//// 		SrSnColorSurf* surf = smoothSurfaces[i];
	//// 		updateSmoothSurface(surf);
	//// 	}
	}

	pseudoParameters = blendEngine->resamplePts;
}



std::vector<double> SBMotionBlendBase::getMotionParameter( const std::string& motion )
{
	dVector motionParam;
	blendEngine->getMotionParameter(motion,motionParam);
	std::vector<double> param;
	for (unsigned int i=0;i<motionParam.size();i++)
		param.push_back(motionParam(i));
	return param;
}

void SBMotionBlendBase::addTetrahedron( const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4 )
{
	PABlend::addTetrahedron(motion1, motion2, motion3, motion4);
}

std::string SBMotionBlendBase::saveToString()
{
	SmartBody::SBMotionBlendBase* state = this;

	std::string stateNameVariable = state->stateName + "State";
	boost::replace_all(stateNameVariable,"@","");
	boost::replace_all(stateNameVariable,".","dot");
	std::stringstream strstr;
	strstr << "# blend " << stateName << "\n";
	strstr << "# autogenerated by SmartBody\n";
	strstr << "\n";
	strstr << "blendManager = scene.getBlendManager()\n";
	strstr << "\n";
	strstr << "# align motions first if needed\n";
#if 1
	for (int i = 0; i < state->getNumMotions(); i++)
	{
		SBMotion* motion = dynamic_cast<SBMotion*>(state->motions[i]);
		if (!motion)
			continue;
		int alignIndex = motion->getAlignIndex();
		if (alignIndex > 0)
		{
			strstr << "alignmotion = scene.getMotion(\"" << state->getMotionName(i) << "\")\n";
			strstr << "alignmotion.alignToBegin(" << alignIndex << ")\n";
		}
		else if (alignIndex < 0)
		{
			strstr << "alignmotion = scene.getMotion(\"" << state->getMotionName(i) << "\")\n";
			strstr << "alignmotion.alignToEnd(" << -alignIndex << ")\n";
		}
	}
#endif

	strstr << "\n";	
	// add the motions
	strstr << stateNameVariable << " = blendManager.createMotionBlendBase(\"" << stateName << "\", \"" << skeletonName << "\", " << getParameterDimension() << ")\n";			
	strstr << stateNameVariable << ".setBlendSkeleton(\"" << state->getBlendSkeleton() << "\")\n";
	strstr << "\n";	
	strstr << "motions = StringVec()\n";
	for (int x = 0; x < state->getNumMotions(); x++)
	{
		strstr << "motions.append(\"" << state->getMotion(x) << "\")\n";
	}

	strstr << "para = DoubleVec()\n";
	strstr << "for i in range(0,3):\n";
	strstr << "\tpara.append(0)\n";
	strstr << "for i in range(0, len(motions)):\n";
	strstr << "\t" << stateNameVariable << ".addMotion(motions[i], para)\n";

	// add the correspondence points
	strstr << "\n";
	for (int c = 0; c < state->getNumKeys(); c++)
	{
		strstr << "points" << c << " = DoubleVec()\n";
		for (int m = 0; m < state->getNumMotions(); m++)
		{
			strstr << "points" << c << ".append(" << state->keys[m][c] << ") # " << state->getMotion(m) << " " << c << "\n";
		}
		strstr << stateNameVariable << ".addCorrespondencePoints(motions, points" << c << ")\n";
	}

	strstr << stateNameVariable << ".buildBlendBase(\"" << getBlendType() << "\", \"" << getInterpType() << "\", False)\n";
	
	return strstr.str();
}



const std::string& SBMotionBlendBase::getSkeleton()
{
	return skeletonName;
}

const std::string& SBMotionBlendBase::getInterpType()
{
	return interpType;
}

const std::string& SBMotionBlendBase::getBlendType()
{
	return blendType;
}




} // namespace