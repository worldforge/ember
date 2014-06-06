#include <map>
#include <boost/foreach.hpp>
#include "controllers/me_ct_barycentric_interpolation.h"
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>


//#define USE_TETGEN
#ifdef USE_TETGEN
#include <external/tetgen/tetgen.h>
#endif

#define FREE_DATA(data) if (data) delete data; data=NULL;

/************************************************************************/
/* Simplex class                                                        */
/************************************************************************/

Simplex& Simplex::operator=( const Simplex& rhs )
{
	numDim = rhs.numDim;
	vertexIndices = rhs.vertexIndices;
	return *this;	
}

void Simplex::getSubSimplex( std::vector<Simplex>& outSimpList )
{
	outSimpList.clear();
	for (unsigned int i=0;i<vertexIndices.size();i++)
	{
		Simplex subSim;
		subSim.numDim = numDim-1;
		for (int k=0;k<numDim;k++)
		{
			int vidx = (i+k)%vertexIndices.size();
			subSim.vertexIndices[k] = vidx;							 
		}
		outSimpList.push_back(subSim);
	}
}
/************************************************************************/
/* Barycentric Interpolator                                             */
/************************************************************************/
BarycentricInterpolator::BarycentricInterpolator()
{
	dataDim = DATA_3D; // only use 3D by default
}

BarycentricInterpolator::~BarycentricInterpolator()
{

}

int BarycentricInterpolator::getPointSimplexIndex(const dVector& pt )
{	
	for (unsigned int i=0;i<simplexList.size();i++)
	{
		VecOfInterpWeight w;
		if (pointInsideSimplex(pt,simplexList[i],w))
		{
			return i;
		}
	}
	return -1;
}

bool BarycentricInterpolator::simplexCoordinate( const dVector& pt, Simplex& tet, VecOfInterpWeight& weight )
{
	int numDim  = tet.numDim;
	int numElem = numDim + 1;
	dMatrix mat(numDim,numDim), invMat(numDim,numDim);	
	InterpolationExample* ex0 = interpExamples[tet.vertexIndices[numDim]];	
	
	for (int i=0;i<numDim;i++)
	{
		int vtxIdx = tet.vertexIndices[i];
		InterpolationExample* ex = interpExamples[vtxIdx];	
		column(mat,i) = ex->parameter - ex0->parameter;
		
	}
	MeCtUBLAS::inverseMatrix(mat,invMat);
	dVector w;
	MeCtUBLAS::matrixVecMult(invMat,(pt-ex0->parameter),w);
	weight.resize(numElem);
	double wEnd = 1.0;
	for (int i=0;i<numDim;i++)
	{
		int vtxIdx = tet.vertexIndices[i];
		weight[i].first = vtxIdx;
		weight[i].second = (float)w[i];
		wEnd -= w[i];
	}
	weight[numDim].first = tet.vertexIndices[numDim];
	weight[numDim].second = (float)wEnd;		

	return true;
}

bool BarycentricInterpolator::pointInsideSimplex(const dVector& pt, Simplex& tet, VecOfInterpWeight& weight )
{	
	int numDim  = tet.numDim;
	int numElem = numDim + 1;
	assert(numElem == tet.vertexIndices.size());
	double prevDet = 0.0, totalDet;
	dMatrix mat(numElem,numElem);
	dVector v(numElem);
	v[numDim] = 1.0;
	for (int i=0;i<numElem;i++)
	{
		int vtxIdx = tet.vertexIndices[i];
		InterpolationExample* ex = interpExamples[vtxIdx];		
		subrange(v,0,numDim) = ex->parameter;
		row(mat,i) = v;
	}
	prevDet = MeCtUBLAS::matrixDeterminant(mat);
	totalDet = prevDet;

	if (prevDet == 0.0)
		return false; // degenerate case

	dVector tempRow, ptRow;
	ptRow = v;
	subrange(ptRow,0,numDim) = pt;
	weight.resize(numElem);
	for (int i=0;i<numElem;i++)
	{
		double curDet;
		tempRow = row(mat,i);
		row(mat,i) = ptRow;
		curDet = MeCtUBLAS::matrixDeterminant(mat);
		if (MeCtMath::sgn(curDet) != MeCtMath::sgn(prevDet))
			return false;

		weight[i].first = tet.vertexIndices[i];
		weight[i].second = (float)(curDet/totalDet);

		prevDet = curDet;
		row(mat,i) = tempRow;
	}
	return true;
}

bool BarycentricInterpolator::buildInterpolator()
{
#ifdef USE_TETGEN
	tetgenio ptIn, tetOut;
	// initialize input points
	ptIn.numberofpoints = interpExamples.size();	
	ptIn.pointlist = new REAL[interpExamples.size()*3];
	for (unsigned int i=0;i<interpExamples.size();i++)
	{
		InterpolationExample* pt = interpExamples[i];
		assert(pt->parameter.size() >= (unsigned int)dataDim);		
		SrVec posIn = SrVec();
		// since tetgen always assume the input to be a 3D point sets, we handle the case when dataDim < 3
		for (int k=0;k<dataDim;k++)
			posIn[k] = (float)pt->parameter[k];
		for (int k=0;k<3;k++)
			ptIn.pointlist[i*3+k] = posIn[k];		
	}
	tetrahedralize((char*)"V",&ptIn,&tetOut);

	for (int i=0;i<tetOut.numberoftetrahedra;i++)
	{
		Simplex tet;
		tet.numDim = dataDim;
		for (int k=0;k<tetOut.numberofcorners;k++)
			tet.vertexIndices.push_back(tetOut.tetrahedronlist[i*tetOut.numberofcorners+k]);
		simplexList.push_back(tet);
	}
#endif
	//ptIn.deinitialize();	
	//tetOut.deinitialize();
	return true;	
}

void BarycentricInterpolator::predictInterpWeights( const dVector& para, VecOfInterpWeight& blendWeights )
{
	int index = getPointSimplexIndex(para);
	if (index != -1)
	{
		Simplex& sip = simplexList[index];
		simplexCoordinate(para,sip,blendWeights);
	}
}

float BarycentricInterpolator::distToSimplex( const dVector& pt, Simplex& simp )
{
	return 1.f;	
}

