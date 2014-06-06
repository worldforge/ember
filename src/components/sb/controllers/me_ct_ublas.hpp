#pragma once
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric;
typedef boost::numeric::ublas::matrix<double, boost::numeric::ublas::column_major> dMatrix;
typedef boost::numeric::ublas::vector<double> dVector;

typedef boost::numeric::ublas::matrix<float, boost::numeric::ublas::column_major> fMatrix;
typedef boost::numeric::ublas::vector<float> fVector;

template <class T>
struct ublasT
{
	typedef boost::numeric::ublas::vector<T> VecT;
	typedef boost::numeric::ublas::matrix<T, boost::numeric::ublas::column_major> MatT;		
};

template <class T>
T* getPtr(boost::numeric::ublas::vector<T>& vec) { return &(vec.data()[0]); }
template <class T>
T* getPtr(boost::numeric::ublas::matrix<T, boost::numeric::ublas::column_major>& mat) { return &(mat.data()[0]); }





// some linear algebra routines based on boost numeric binding and Lapack
namespace MeCtUBLAS	
{	
	bool inverseMatrix(const dMatrix& mat, dMatrix& inv);
	bool linearLeastSquare(const dMatrix& A, const dMatrix& B, dMatrix& sol);
	bool matrixSVD(const dMatrix& A, dVector& S, dMatrix& U, dMatrix& V); // perform singular value decomposition
	void matrixMatMult(const dMatrix& mat1, const dMatrix& mat2, dMatrix& mat3);
	void matrixVecMult(const dMatrix& mat1, const dVector& vin, dVector& vout);	
	double matrixDeterminant(const dMatrix& mat);
};

namespace MeCtMath
{
	float Random(float r_min, float r_max);
	template <typename T> int sgn(T val)
	{
		return (val > T(0)) - (val < T(0));
	}
}
