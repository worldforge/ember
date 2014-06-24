#ifndef _SBSTATE_H
#define _SBSTATE_H

#include "SBTypes.h"
#ifdef EMBER_SB_SURFACES
	#include "components/sb/sr/sr_sn_colorsurf.h"
#endif
#include "PABlend.h"
#include "components/sb/sr/sr_lines.h"
#include "components/sb/sr/sr_sn_shape.h"


#define VFLOW_LINE_WIDTH 2.0f

typedef std::map<std::string, std::vector<double> > KeyTag;
typedef std::map<int,KeyTag> KeyTagMap;
class MotionAnalysis;

namespace SmartBody {

class SBAnimationBlend : public PABlend
{
	public:
		SBAPI SBAnimationBlend();
		SBAPI SBAnimationBlend(const std::string& name);
		SBAPI ~SBAnimationBlend();

		SBAPI int getParameterDimension();
		SBAPI bool getWeightsFromParameters(SrVec& para, std::vector<float>& weights);
		SBAPI SrBox getParameterBoundBox();

		SBAPI void setBlendSkeleton(std::string skelName);
		SBAPI std::string getBlendSkeleton();

		SBAPI void setIncrementWorldOffsetY(bool flag);
		SBAPI void addCorrespondencePoints(const std::vector<std::string>& motions, const std::vector<double>& points);			
		SBAPI void removeCorrespondencePoints(int index);
		SBAPI void addKeyTagValue(const std::string& motionName, int iType, const std::string& tagName, double value);	
		SBAPI KeyTagMap* getKeyTagMap(const std::string& motionName);
		SBAPI void buildMotionAnalysis(const std::string& skeletonName, const std::string& baseName, const std::vector<std::string>& motions, std::string motionPrefix);
		SBAPI MotionAnalysis* getMotionAnalysis();

		SBAPI virtual int getNumMotions();
		SBAPI virtual std::string getMotion(int num);
		SBAPI SkMotion* getSkMotion(const std::string& motionName);
		SBAPI virtual int getNumCorrespondencePoints();
		SBAPI virtual std::vector<double> getCorrespondencePoints(int num);
		SBAPI virtual void setCorrespondencePoints(int motionIndex, int pointIndex, double value);

		SBAPI virtual void removeMotion(const std::string& motionName);
		SBAPI virtual void addEvent(const std::string& motion, double time, const std::string& type, const std::string& parameters, bool onceOnly);
		SBAPI virtual void removeEvent(int index);
		SBAPI virtual SBMotionEvent* getEvent(int index);
		SBAPI virtual void removeAllEvents();
		SBAPI virtual int getNumEvents();

		SBAPI virtual std::string getDimension();
		SBAPI bool validateState();

		SBAPI virtual std::string saveToString();

		// Functions to build visualization surface
		#ifdef EMBER_SB_SURFACES
		SBAPI void buildVisSurfaces(const std::string& errorType, const std::string& surfaceType, int segments, int dimensions);
		SBAPI SrSnColorSurf* createCurveSurface(float radius, unsigned int dimension, SrVec center, SrVec2 phi, SrVec2 theta);		
		SBAPI SrSnColorSurf* createFlatSurface(float depth, unsigned int dimension, SrVec2 topLeft, SrVec2 lowerRight);
		SBAPI void createErrorSurfaces(const std::string& type, SrVec center, int segments, int dimensions, std::vector<SrSnColorSurf*>& surfList);
		SBAPI void updateErrorSurace(SrSnColorSurf* surf, SrVec center);
		SBAPI void updateSmoothSurface(SrSnColorSurf* surf);
		SBAPI std::vector<SrSnColorSurf*>& getErrorSurfaces() { return errorSurfaces; }
		SBAPI std::vector<SrSnColorSurf*>& getSmoothSurfaces() { return smoothSurfaces; }
		#endif


		/* motion vector flow for motion smoothness visualize: each vector is the abs. movement of a particular joint
		// as it traverse 3d space between two consecutive motion frames. Colors are assigned to the vectors representing
		// sudden change in vector length compared against local average of the length computed with a sliding window,
		// highlighting the abnormal speed-ups (warm color) and slowdowns (cool color) caused by jitters and such.
		// vecs within [1-plotTh, 1+plotTh] of local avg are plotted in light gray.
		// sliding window size = slidWinHalfSize x 2 + 1.
		// added by David Huang, June 2012 */
		SBAPI void createMotionVectorFlow(const std::string& motionName, const std::string& chrName, float plotThreshold=0.45f,
									unsigned int slidWinHalfSize=7, bool clearAll=false);
		SBAPI std::vector<SrSnLines*>& getVectorFlowSrSnLines() { return vecflowLinesArray; }
		SBAPI void clearMotionVectorFlow(void);

		/* plot motion frames (stick figures) and joint trajectory
		// added by David Huang, June 2012 */
		SBAPI void plotMotion(const std::string& motionName, const std::string& chrName, unsigned int interval=10,
						bool clearAll=false, bool useRandomColor=true);
		SBAPI void plotMotionFrameTime(const std::string& motionName, const std::string& chrName, float time, bool useRandomColor);
		SBAPI void plotMotionJointTrajectory(const std::string& motionName, const std::string& chrName, const std::string& jointName,
										float start_t=0.0f, float end_t=0.0f, bool useRandomColor=false);
		SBAPI std::vector<SrSnLines*>& getPlotMotionSrSnLines() { return plotMotionLinesArray; }
		SBAPI void clearPlotMotion(void);

		SBAPI void setChrPlotMotionTransform(const std::string& chrName);
		SBAPI void setPlotMotionTransform(SrVec offset=SrVec::null, float yRot=0.0f);
		SBAPI const SrMat& getPlotMotionTransform() { return plotMotionTransform; }
		SBAPI void clearPlotMotionTransform() { plotMotionTransform.identity(); }
		SBAPI void setChrPlotVectorFlowTransform(const std::string& chrName);
		SBAPI void setPlotVectorFlowTransform(SrVec offset=SrVec::null, float yRot=0.0f);
		SBAPI const SrMat& getPlotVectorFlowTransform() { return plotVectorFlowTransform; }
		SBAPI void clearPlotVectorFlowTransform() { plotVectorFlowTransform.identity(); }

		bool addMotionRef( SBMotion* sbmotion );
	protected:
		bool addSkMotion(const std::string& motionName);		
		bool removeSkMotion(const std::string& motionName);		
		/*	This function make sure that all the correspondence points are in ascendant order		*/
		void validateCorrespondencePoints();		

	protected:
		MotionAnalysis* motionAnalysis;
		std::string _dimension;
		std::string blendSkelName;
		bool _isFinalized;
		int parameterDim;

		#ifdef EMBER_SB_SURFACES
		std::vector<SrSnColorSurf*> errorSurfaces;
		std::vector<SrSnColorSurf*> smoothSurfaces;
		#endif
		std::vector<KeyTagMap> keyTagList;
		SrMat plotMotionTransform;
		SrMat plotVectorFlowTransform;
		std::vector<SrSnShape<SrLines>*> vecflowLinesArray;
		// put a list of joint global positions into array
		void getJointsGPosFromSkel(SkSkeleton* sk, SrArray<SrVec>& pnts_array, const std::vector<SkJoint*>& jnt_list);
		// find maximum vector norm (vector are connected between jnt global positions in consecutive frame pairs)
		float getVectorMaxNorm(SrArray<SrArray<SrVec>*>& pnts_arr);
		std::vector<SrSnLines*> plotMotionLinesArray; // plotMotion()
		std::vector<SkJoint*> plot_excld_list;
		// create a list for joint exclusion when plotting motion and vector flow (hard coded to exclude fingers, eye, etc)
		void createJointExclusionArray(const std::vector<SkJoint*>& orig_list);
		bool isExcluded(SkJoint* j); // return true if joint is excluded

};


class SBAnimationBlend0D : public SBAnimationBlend
{
	public:
		SBAPI SBAnimationBlend0D();
		SBAPI SBAnimationBlend0D(const std::string& name);
		SBAPI ~SBAnimationBlend0D();		
		SBAPI virtual void addMotion(const std::string& motion);
		SBAPI virtual void removeMotion(const std::string& motionName);
		
};

class SBAnimationBlend1D : public SBAnimationBlend
{
	public:
		SBAPI SBAnimationBlend1D();
		SBAPI SBAnimationBlend1D(const std::string& name);
		SBAPI ~SBAnimationBlend1D();
		
		virtual void addMotionFromRef(SmartBody::SBMotion* motion, float parameter);
		SBAPI virtual void addMotion(const std::string& motion, float parameter);
		SBAPI virtual void removeMotion(const std::string& motionName);
		SBAPI void setParameter(const std::string& motion, float parameter);
};

class SBAnimationBlend2D : public SBAnimationBlend
{
	public:
		SBAPI SBAnimationBlend2D();
		SBAPI SBAnimationBlend2D(const std::string& name);
		SBAPI ~SBAnimationBlend2D();

		SBAPI virtual void addMotion(const std::string& motion, float parameter1, float paramter2);
		SBAPI virtual void removeMotion(const std::string& motionName);
		SBAPI void setParameter(const std::string& motion, float parameter1, float parameter2);
		SBAPI void addTriangle(const std::string& motion1, const std::string& motion2, const std::string&motion3);
};

class SBAnimationBlend3D : public SBAnimationBlend
{
	public:
		SBAPI SBAnimationBlend3D();
		SBAPI SBAnimationBlend3D(const std::string& name);
		SBAPI ~SBAnimationBlend3D();

		SBAPI virtual void addMotion(const std::string& motion, float parameter1, float paramter2, float paramter3);
		SBAPI virtual void removeMotion(const std::string& motionName);
		SBAPI void setParameter(const std::string& motion, float parameter1, float parameter2, float parameter3);
		SBAPI void addTetrahedron(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4);

};
}
#endif