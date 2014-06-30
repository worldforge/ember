#pragma once
#include <map>
#include "me_ct_motion_example.hpp"
#include "me_ct_ublas.hpp"
#include "external/nanoflann/nanoflann.hpp"

class DataInterpolator
{
protected:
	VecOfInterpExample interpExamples;	
	ExampleSet* exampleSet;
public:
	DataInterpolator() {}
	virtual ~DataInterpolator();
	virtual void init(ExampleSet* exSet);
	virtual bool buildInterpolator() = 0;
	virtual void predictInterpWeights(const dVector& para, VecOfInterpWeight& blendWeights) = 0;
	VecOfInterpExample* getInterpExamples() { return &interpExamples; }
	virtual void drawInterpolator() {}
};

// Motion interpolation based on radial-basis function (RBF). 
// A set of motions are first initialized and used for computing the RBF weights.
// At run-time, given an input parameter, corresponding interpolation weights are determined.
// An interpolated motion/pose is generated 
class RBFInterpolator : public DataInterpolator
{
protected:
	dMatrix rbfMatrix, linearMatrix; // prediction matrix for RBF regression
public:
	RBFInterpolator();
	virtual ~RBFInterpolator();

	virtual bool buildInterpolator();
	virtual void predictInterpWeights(const dVector& para, VecOfInterpWeight& blendWeights);

protected:
	double RBFValue(const dVector& p1, const dVector& p2);
};

struct KNNPointCloud
{
	int dim;
	VecOfInterpExample pts;
	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }
	// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
	inline double kdtree_distance(const double *p1, const size_t idx_p2,size_t size) const
	{
		double dist = 0.0;
		for (unsigned int i=0;i<size;i++)
		{
			double dif = p1[i] - pts[idx_p2]->parameter[i];
			dist += dif*dif;
		}
		return dist;
	}

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline double kdtree_get_pt(const size_t idx, int dim) const
	{
		return pts[idx]->parameter[dim];		
	}
	template <class BBOX>
	bool kdtree_get_bbox(BBOX &bb) const { return false; }
};

//using namespace nanoflann;
typedef nanoflann::KDTreeSingleIndexAdaptor<
	nanoflann::L2_Simple_Adaptor<double, KNNPointCloud > ,
	KNNPointCloud,
	-1 /* dim */
> SBKDTree; 


class KNNBaseInterpolator : public DataInterpolator
{
protected:	
	int numKNN;
public:
	static void generateRandomWeight(int nK, vector<float>& outWeights);
protected:
#if USE_ANN
	ANNpointArray dataPts; // data points	
	ANNkd_tree* kdTree;    // search structure	
	int  kdTreeKNN(ANNkd_tree* kdTree, const dVector& inPara,int nKNN, VecOfInterpWeight& outWeight);
#else
	SBKDTree* kdTree;
	KNNPointCloud kdPtSet;
	int kdTreeKNN(SBKDTree* kdTree, const dVector& inPara,int nKNN, VecOfInterpWeight& outWeight);
#endif
	static void mapDistWeightToBlendWeight(VecOfInterpExample& exampleList, VecOfInterpWeight& inDistWeight, VecOfInterpWeight& outBlendWeight);
	static void generateDistWeights(vector<float>& dists, vector<float>& outWeights);	
	static void normalizeBlendWeight(VecOfInterpWeight& weight);
};

// Motion interpolation based on K-nearest neighbor
class KNNInterpolator : public KNNBaseInterpolator
{   
protected:	
	
	float minDist;	
	int resampleSize; // parameters for resampling		
public: // temp set it to public for debug
	VecOfInterpExample resampleData;
	VecOfInterpExample finalExampleData;
	// a grid based hash map to accelerate KNN look up
	ParameterBoundingBox gridBox;
	std::map<int,VecOfInterpExample> exampleHash; 
public:
	KNNInterpolator(int numResample = 500, float sampleDist = 5.f);
	virtual ~KNNInterpolator();		

	virtual bool buildInterpolator();
	virtual void predictInterpWeights(const dVector& para, VecOfInterpWeight& blendWeights);
protected:		
	bool addResample(InterpolationExample* ex);
	// both KNN methods return the indices of KNN samples ( in InterpWeight.first ), and store distances in the InterpWeight.second
	// further processing is needed to infer the blending weights from distances
	int  closestExampleInHash(const dVector& inPara, unsigned int nKNN, VecOfInterpWeight& outWeight);
	int  linearKNN(const VecOfInterpExample& sampleList, const dVector& inPara, 
		           int nKNN, VecOfInterpWeight& outWeight);	
};