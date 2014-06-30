#ifndef _SBMOTIONGRAPH_H
#define _SBMOTIONGRAPH_H

#include <sb/SBTypes.h>
#include <sb/SBAnimationState.h>
#include <sb/SBRetarget.h>
/*#include <sbm/SteerPath.h> */
#include "controllers/me_ct_ublas.hpp"

class MotionTimeWarpFunc;

namespace SmartBody {

	class SBMotionTransitionEdge;
	class SBMotionGraph;

	class SBMotionFrameBuffer
	{
	protected:
		std::map<std::pair<std::string,SkChannel::Type>, int> channelIdxMap; // map channel name to buffer index
		SrBuffer<float> motionFrameBuffer;
		std::vector<std::string> quatChannelNames, posChannelNames;
	public:
		SBMotionFrameBuffer();
		~SBMotionFrameBuffer();

		void applyRetarget(SBRetarget* retarget);
		void copyFrameBuffer(SBMotionFrameBuffer& inBuffer);
		void initFrameBuffer(SkChannelArray& channelArray, std::vector<std::string>& affectedChannelList);
		void setQuat(const std::string& chanName, const SrQuat& q);
		void setPos(const std::string& chanName, const SrVec& p);
		void setMat(const std::string& chanName, const SrMat& mat);
		void setAllQuats(const std::vector<std::string>& quatNames, const std::vector<SrQuat>& quatList);
		void setAllPos(const std::vector<std::string>& posNames, const std::vector<SrVec>& posList);


		SrQuat getQuat(const std::string& chanName);
		SrVec  getPos(const std::string& chanName);
		SrMat  getMat(const std::string& chanName);

		int findChannelIndex(const std::string& chanName, SkChannel::Type chanType);
		SrBuffer<float>& getBuffer();
		const std::map<std::pair<std::string,SkChannel::Type>, int>& getChannelIdxMap();
		const std::vector<std::string>& getQuatChannelNames();
		const std::vector<std::string>& getPosChannelNames();
		int getNumQuatChannels();
		int getNumPosChannels();

		// note : frameA, frameB needs to be the same size & channel layouts
		static void interpMotionFrameBuffer(SBMotionFrameBuffer& frameA, SBMotionFrameBuffer& frameB, float weight, SBMotionFrameBuffer& outFrame);
	};

	class SBMotionNode
	{
	protected:
		int index;		
		SBAnimationBlend* animBlend;
		std::vector<MotionTimeWarpFunc*> timeWarpFuncs;
		std::string baseJointName;
		SrMat basePreRot;
		std::vector<SBMotionTransitionEdge*> outEdges;
	public:
		SBMotionNode();
		SBMotionNode(SBAnimationBlend* blend, int idx);
		~SBMotionNode();
		float getRefDuration();
        float getActualDuration(const std::vector<float>& weights);	
		float getActualTime(float u, const std::vector<float>& weights);
		float getRefDeltaTime(float u, float dt, const std::vector<float>& weights);
		SBAnimationBlend* getAnimBlend();
		int getIndex();
		std::string getName();
		void addOutEdge(SBMotionTransitionEdge* edge);
		const std::vector<SBMotionTransitionEdge*>& getOutEdges();
		void getBlendedMotionFrame(float u, const std::vector<float>& weights, SBMotionFrameBuffer& outFrame);
		// get the delta transform with y-rotation and xz-translation between time S(u) and S(u) + dt, here u is canonical time
		// also get the base transform with xz-rotation and y-translation
		void getDeltaAlignTransform(float u, float du, const std::vector<float>& weights, SrMat& outAlignDiff, SrMat& outBaseXZFrame);	
		void getRandomBlendWeights(std::vector<float>& outWeights);
		static void decomposeBaseMat(SrMat& inMat, SrMat& inPreRot, SrMat& outYWoMat, SrMat& outXZBaseMat, SBRetarget* retarget = NULL);
	protected:
		void initTimeWarp();
		static void interpQuatList(float w, std::vector<SrQuat>& quatListA, std::vector<SrQuat>& quatListB, std::vector<SrQuat>& outQuatList);
		static void interpPosList(float w, std::vector<SrVec>& posListA, std::vector<SrVec>& posListB, std::vector<SrVec>& outPosList);
	};

	class SBMotionTransitionEdge
	{
	public:
		bool forwardEdge;
	protected:
		int index;
		SBMotionNode *srcNode, *tgtNode;
		float srcRefTime, tgtRefTime;			
	public:
		SBMotionTransitionEdge();
		SBMotionTransitionEdge(int idx);
		~SBMotionTransitionEdge();
		int getIndex();
		std::string getName();
		SBMotionNode* getSrcNode();
		SBMotionNode* getTgtNode();
		
		void initTransitionEdge(SBMotionNode* nodeFrom, SBMotionNode* nodeTo, float srcTime, float dstTime);
		void getSimpleTransition(float desiredSourceRefTime, float& uSrc, float& uTgt);
		void getParametricTransition(float desiredSourceRefTime, const std::vector<float>& srcParameter, float& uSrc, float& uTgt, std::vector<float>& tgtParameter);
	};

	class SBMotionNodeState // the instance of a traversal in motion graph
	{
	public: ///// Constants
		enum GraphWalkType { RANDOM_WALK, DIRECTED_CONTROL, FULL_MANUAL } ;
		typedef std::pair<SBMotionTransitionEdge*, std::vector<float> > SBTransitionRecord;
	protected:
		SBMotionNode *prevNode, *nextNode;
		SBMotionNode *motionNode;
		SBMotionFrameBuffer prevBuffer, curBuffer, nextBuffer; // prevBuffer & nextBuffer are for animation transition
		std::string baseJointName;
		float currentRefTime;
		float transitionLength;
		std::vector<float> blendWeights;
		bool useTransitionInterpolation;
		
		GraphWalkType graphWalkType;
		std::queue<SBTransitionRecord> transitionQueue; // store the list of nodes for future transition in the graph		
	public:
		SBMotionNodeState();
		~SBMotionNodeState();
		bool isRunning();
		float getCurrentRefTime();
		SBMotionNode* getCurrentMotionNode();
		SBMotionFrameBuffer& getCurMotionFrameBuffer();
		GraphWalkType getGraphWalkType() const;
		void setGraphWalkType(GraphWalkType val);

		bool getUseTransitionInterpolation() const { return useTransitionInterpolation; }
		void setUseTransitionInterpolation(bool val) { useTransitionInterpolation = val; }

		bool addNextTransition(SBMotionTransitionEdge* edge, std::vector<float>& transitionWeight);
		void startMotionState(SBMotionNode* startNode, std::vector<float>& startWeights);
		SrMat stepMotionState(float dt);
		
		void initState(SkChannelArray& channelArray, std::vector<std::string>& affectedChannelList, const std::string& baseName);	
	protected:
		float applyTransition(float dt, float curDu); // apply the transition and return the new du	
		void updateNextStateBufferCache();
	};	

	class SBMotionGraph 
	{	
		struct MotionCoordCache
		{
			std::vector<float> avgX1, avgX2, avgZ1, avgZ2;
			std::vector<float> avgF1, avgF2;
			fMatrix xxzzMat, xzzxMat, yyMat;
		};

		struct MotionNodeCache
		{
			std::vector<float> pathDists;
			std::vector<SrVec> pathLocalPts;
			SrMat deltaTransform;
			float totalArcLength;
		};

		struct MotionGraphTraverse
		{
			int curNodeIdx;
			SrMat curTransform;
			std::vector<std::pair<int,int> > graphEdges;
			float traversePathDist;
			float traverseTime;
			float traverseError;
			float distToTarget;
			MotionGraphTraverse& operator = ( const MotionGraphTraverse& rtIn);
		};

	protected:
		std::map<std::string, SBMotionNode*> motionNodeMap;
		std::map<std::string, SBMotionTransitionEdge*> motionEdgeMap;
		std::vector<SBMotionNode*> motionNodes;		
		std::vector<SBMotionTransitionEdge*> motionEdges;
		std::vector<SBAnimationBlend*> motioAnimBlends; // store the temp anim blends created by addMotionNodeFromMotion
		std::vector<SBMotion*> transitionMotions; // store the temp motions created by addMotionNodeFromMotionTransition
		bool useTransitionInterpolation;
	public:		
		SBAPI SBMotionGraph();
		SBAPI ~SBMotionGraph();		

		SBAPI SBMotionNode* addMotionNodeFromMotion( const std::string& nodeName, const std::string& motionName, int startFrame = -1, int endFrame = -1);		

		SBAPI SBMotionNode* addMotionNodeFromMotionRef(const std::string& nodeName,  SmartBody::SBMotion* sbMotion, int startFrame, int endFrame);
		SBAPI SBMotionNode* addMotionNodeFromBlend(SBAnimationBlend* blend);
		SBAPI SBMotionTransitionEdge* addMotionEdge(const std::string& fromNodeName, const std::string& toNodeName);
		SBAPI SBMotionTransitionEdge* addMotionEdgeByIndex(int fromNodeIdx, int toNodeIdx);
		SBAPI SBMotionNode* getMotionNode(const std::string& nodeName);
		SBAPI SBMotionTransitionEdge* getMotionEdge(const std::string& srcNode, const std::string& dstNode);
		SBAPI std::vector<std::string> getMotionNodeNames();
		SBAPI void buildAutomaticMotionGraph(const std::vector<std::string>& motionNames, const std::string& skelName, const std::vector<std::string>& endJointNames);
/*		SBAPI void synthesizePath( SteerPath& desiredPath, const std::string& skeletonName, std::vector<std::pair<std::string,std::string> >& graphTraverseEdges); */
	protected:
		SBMotionNode* addMotionNodeFromMotionTransition( const std::string& nodeName, const std::string& motionName1, std::string& motionName2, int mo1EndFrme, int mo2StartFrame, int transitionLength = 5);
		//void computeMotionTransition(const std::string& moName1, const std::string& moName2, const std::string& skelName, const std::vector<std::string>& affectedJointNames, float threshold, std::vector<std::pair<int,int>>& outTransition);
		void computeMotionTransitionFast(const std::string& moName1, const std::string& moName2, const std::string& skelName, const std::vector<std::string>& affectedJointNames, float threshold, std::vector<std::pair<int,int> >& outTransition);

		//float computeTransitionError(const std::vector<SrVec>& pos1, const std::vector<SrVec>& pos2, int idx1, int idx2);
		float computeTransitionErrorFast(MotionCoordCache& cache, int frame1, int frame2, int windowSize );
		bool findLocalMinimum(dMatrix& mat, int i, int j);
		void writeOutTransitionMap(const std::string& outfilename, const dMatrix& mat, const std::vector<std::pair<int,int> >& transitions);
		int findClosestElement(const std::vector<int>& intList, int val);
/*		float pathError( SteerPath& curPath, const SBMotionNode* curNode, const SrMat& curTransform, MotionNodeCache& nodeCache, float pathLength); */
/*		float traverseGraph(SteerPath& curPath, MotionGraphTraverse& curGraphTraverse, MotionGraphTraverse& bestTraverse, std::map<std::string, MotionNodeCache>& deltaTransformMap, float timeThreshold, float& bestTraverseError); */
	};

	class SBMotionGraphManager
	{
	public:
		SBAPI SBMotionGraphManager();
		SBAPI ~SBMotionGraphManager();	

		SBAPI SBMotionGraph* createMotionGraph(const std::string& moGraphName);
		SBAPI SBMotionGraph* getMotionGraph(const std::string& moGraphName);		
		SBAPI std::vector<std::string> getMotionGraphNames();
		// automatically build a motion graph
		
	protected:
		std::map<std::string, SBMotionGraph*> _motionGraphMap;
	};
}
#endif // namespace