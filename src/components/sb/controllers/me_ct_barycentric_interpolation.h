#pragma once
#include "controllers/me_ct_data_interpolation.h"

// general simplex
// a 1D simplex is a line segment, a 2D simplex is a triangle, and a 3D simplex is a tetrahedron
// we assume only 1D to 3D simplex in the barycentric interpolator
class Simplex
{
public:
	int numDim;
	VecOfInt vertexIndices;
public:
	Simplex& operator=(const Simplex& rhs);	
	void getSubSimplex(std::vector<Simplex>& outSimpList);
};

typedef std::vector<Simplex> VecOfSimplex;

class BarycentricInterpolator : public DataInterpolator
{
public:
	enum DataDimension { DATA_1D = 1, DATA_2D, DATA_3D };
	VecOfSimplex  simplexList;
protected:
	DataDimension dataDim;
public:
	BarycentricInterpolator();
	~BarycentricInterpolator();
public:
	virtual bool buildInterpolator();
	virtual void predictInterpWeights(const dVector& para, VecOfInterpWeight& blendWeights);
	virtual void drawInterpolator() {} // debugging information
	int getPointSimplexIndex(const dVector& pt);
protected:
	bool pointInsideSimplex(const dVector& pt, Simplex& tet, VecOfInterpWeight& weight);
	bool simplexCoordinate(const dVector& pt, Simplex& tet, VecOfInterpWeight& weight);
	float distToSimplex(const dVector& pt, Simplex& simp);
};