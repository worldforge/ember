#ifndef SBM_DEFORMABLE_MESH_H
#define SBM_DEFORMABLE_MESH_H

#include "components/sb/sb/SBTypes.h"
#include <vector>
#include <string>
#include <map>
#include "components/sb/sr/sr_sn_shape.h"
#include "components/sb/sk/sk_skeleton.h"
#include "components/sb/sr/sr_model.h"
#include "components/sb/sb/SBAsset.h"
#include "components/sb/sb/SBCharacter.h"


typedef std::vector<SkJoint*> SkJointList;

class SkinWeight
{
public:
	std::vector<std::string>	infJointName;	// name array
	std::vector<SkJoint*>		infJoint;         // corresponding joint for each infJointName
	std::vector<float>			bindWeight;		// weight array
	std::vector<SrMat>			bindPoseMat;	// each joint, binding pose transformation matrix
	SrMat						bindShapeMat;	// overall bind shape transformation matrix
	std::string					sourceMesh;		// skin Source Name
	std::vector<unsigned int>	numInfJoints;	// number of influenced joints for very vertex
	std::vector<unsigned int>	weightIndex;	// looking up the weight according to this index
	std::vector<unsigned int>	jointNameIndex;	// looking up the joint name according to this index

public:
	SBAPI SkinWeight();
	SBAPI ~SkinWeight();
	SBAPI void normalizeWeights();
};

class SbmSubMesh
{
public:
	SrMaterial  material;
	bool isHair;
	std::string matName;
	std::string texName;
	std::string normalMapName;
	std::string specularMapName;
	int numTri;
#if defined(__ANDROID__) || defined(SB_IPHONE)
	std::vector<SrModel::Face> triBuf;
#else
	std::vector<SrVec3i> triBuf;
#endif
};

class DeformableMeshInstance;
namespace SmartBodyBinary
{
	class StaticMesh;
}

/* This class is used to simulate and represent deformed mesh
   for Smartbody Characters.
*/
class DeformableMesh : public SmartBody::SBAsset
{
public:
	std::string                 meshName;
	std::vector<SrSnModel*>		dMeshDynamic_p;
	std::vector<SrSnModel*>		dMeshStatic_p;
	std::vector<SkinWeight*>	skinWeights;
	std::map<std::string, std::vector<SrSnModel*> > blendShapeMap; // morphTargets the key store the base shape name, vector stores morph target SrModels. first one in the vector is always the base one
	std::map<std::string, std::vector<std::string> > morphTargets; // morphTargets stores a vector of morph target names, first one is always the base one
	std::string                 skeletonName; // binding skeleton for this deformable model
	SkSkeleton*					skeleton;			// pointer to current skeleton
	bool						binding;			// whether in deformable mesh mode
	// unrolled all vertices into a single buffer for faster GPU rendering
	bool initStaticVertexBuffer, initSkinnedVertexBuffer;	
	std::vector<SrVec>          posBuf;	
	std::vector<SrVec>          normalBuf;
	std::vector<SrVec>          tangentBuf;
	std::vector<SrVec>          binormalBuf;
	std::vector<SrVec>          skinColorBuf;
	std::vector<SrVec>          meshColorBuf;
	std::vector<SrVec2>         texCoordBuf;	
	std::vector<SrVec3i>        triBuf;
	std::vector<SbmSubMesh*>    subMeshList;

	std::vector<int>			boneCountBuf;
	std::vector<SrVec4i>        boneIDBuf[2];
	std::vector<SrVec4>         boneIDBuf_f[2];
	std::vector<SrVec4>         boneWeightBuf[2];
	std::map<std::string,int>   boneJointIdxMap;
	std::vector<SkJoint*>		boneJointList;	
	std::vector<std::string>    boneJointNameList;
	std::vector<SrMat>          bindPoseMatList;	
	std::map<int,std::vector<int> > vtxNewVtxIdxMap;

	// blend shape
	std::map<std::string, std::vector<SrSnModel*> >	visemeShapeMap;
	std::map<std::string, float>	visemeWeightMap;
	std::vector<SrSnModel*>		dMeshBlend_p;
	bool hasVertexColor;
public:
	DeformableMesh();
	SBAPI virtual ~DeformableMesh();	
	SBAPI void setSkeleton(SkSkeleton* skel);
	virtual void update();
	SkinWeight* getSkinWeight(const std::string& skinSourceName);
	int	getMesh(const std::string& meshName);				// get the position given the mesh name
	int getValidSkinMesh(const std::string& meshName);
    /*! Set the visibility state of the deformable geometry,
        The integers mean 1:show, 0:hide, and -1:don't change the visibility state. */
	void set_visibility(int deformableMesh);
	virtual bool buildSkinnedVertexBuffer(); // unrolled all models inside this deformable mesh into a GPU-friendly format
	SBAPI bool isSkinnedMesh();
#ifdef EMBER_SB_BINARYMESH
	SBAPI bool saveToSmb(std::string inputFileName);
	SBAPI bool saveToDmb(std::string inputFileName);
	SBAPI bool readFromSmb(std::string inputFileName);
	SBAPI bool readFromDmb(std::string inputFileName);
	// helper function
	void saveToStaticMeshBinary(SmartBodyBinary::StaticMesh* mesh);
	void readFromStaticMeshBinary(SmartBodyBinary::StaticMesh* mesh);
#endif
#ifdef EMBER_SB_TEXTURE
	void loadAllFoundTextures(std::string textureDirectory);
#endif
};

class DeformableMeshInstance
{
protected:
	DeformableMesh* _mesh;
	//std::vector<SrSnModel*>	dynamicMesh; 
	SkSkeleton*				_skeleton;
	SmartBody::SBCharacter*	_character;		// pointer to current character
	bool				  _updateMesh;
	std::vector<SkJointList> _boneJointList;
	float _meshScale;
	int  meshVisibleType;
	bool _recomputeNormal;
	bool _isStaticMesh;
public:
	std::vector<SrVec> _deformPosBuf;	
	std::vector<SrMat>  transformBuffer;	
public:
	SBAPI DeformableMeshInstance();
	SBAPI virtual ~DeformableMeshInstance();
	SBAPI void blendShapeStaticMesh();
	SBAPI virtual void setDeformableMesh(DeformableMesh* mesh);
	SBAPI void updateJointList();
	SBAPI virtual void setPawn(SmartBody::SBPawn* pawn);
	SBAPI virtual void setVisibility(int deformableMesh);
	SBAPI virtual void setMeshScale(float scale);
	SBAPI float   getMeshScale() { return _meshScale; }
	SBAPI int    getVisibility();
	SBAPI void    setToStaticMesh(bool isStatic);
	SBAPI bool    isStaticMesh();
	SBAPI SmartBody::SBSkeleton* getSkeleton();	
	SBAPI virtual void update();
	SBAPI virtual void updateFast();
	SBAPI virtual void blendShapes();
	SBAPI DeformableMesh* getDeformableMesh() { return _mesh; }
	void updateTransformBuffer();

protected:
	void cleanUp();
};

#endif
