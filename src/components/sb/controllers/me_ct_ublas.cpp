//#include <boost/numeric/bindings/atlas/cblas.hpp>
#include "controllers/me_ct_ublas.hpp"
#include <boost/numeric/bindings/blas/blas.hpp>
#include <boost/numeric/bindings/lapack/lapack.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/banded.hpp> 
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <time.h>

namespace lapack = boost::numeric::bindings::lapack;
namespace blas   = boost::numeric::bindings::blas;
//namespace ublas  = boost::numeric::ublas;

void MeCtUBLAS::matrixMatMult(const dMatrix& mat1, const dMatrix& mat2, dMatrix& mat3)
{
	if (mat3.size1() != mat1.size1() || mat3.size2() != mat2.size2())
		mat3.resize(mat1.size1(),mat2.size2());	
#if !defined(__FLASHPLAYER__)
	blas::gemm(mat1,mat2,mat3);	
#endif
}

void MeCtUBLAS::matrixVecMult(const dMatrix& mat1, const dVector& vin, dVector& vout)
{
	if (vout.size() != mat1.size1())
		vout.resize(mat1.size1());
	if (vin.size() != mat1.size2())
		return;
#if !defined(__FLASHPLAYER__)
	blas::gemv('N',1.0,mat1,vin,0.0,vout);	
#endif
}

bool MeCtUBLAS::inverseMatrix( const dMatrix& mat, dMatrix& inv )
{
	using namespace boost::numeric::ublas;
	dMatrix A(mat);
	inv = identity_matrix<double>(mat.size1());
#if !defined(__FLASHPLAYER__)
	lapack::gesv(A,inv);
#endif
	return true;
}

bool MeCtUBLAS::linearLeastSquare( const dMatrix& A, const dMatrix& B, dMatrix& sol )
{
	dMatrix AtA, AtB, invAtA;
	MeCtUBLAS::matrixMatMult(ublas::trans(A),A,AtA);
	MeCtUBLAS::matrixMatMult(ublas::trans(A),B,AtB);
	MeCtUBLAS::inverseMatrix(AtA,invAtA);
	MeCtUBLAS::matrixMatMult(invAtA,AtB,sol);
	return true;
}

bool MeCtUBLAS::matrixSVD( const dMatrix& A, dVector& S, dMatrix& U, dMatrix& V )
{
#if !defined(__ANDROID__) && !defined(__FLASHPLAYER__)
	dMatrix M(A);
	lapack::gesvd(M,S,U,V);
#endif
	
	return true;
}

double MeCtUBLAS::matrixDeterminant( const dMatrix& mat )
{
	double det = 1.0; 

	dMatrix mLu(mat); 
	ublas::permutation_matrix<std::size_t> pivots(mat.size1() ); 

	int is_singular = lu_factorize(mLu, pivots); 

	if (!is_singular) 
	{ 
		for (std::size_t i=0; i < pivots.size(); ++i) 
		{ 
			if (pivots(i) != i) 
				det *= -1.0; 

			det *= mLu(i,i); 
		} 
	} 
	else 
		det = 0.0; 

	return det;
}
/************************************************************************/
/* MeCtMath Routines                                                    */
/************************************************************************/

float MeCtMath::Random( float r_min, float r_max )
{
	static bool initRand = false;
	if (!initRand)
	{
		srand(int(time(NULL)));
		initRand = true;
	}
	float frand = (float)rand()/(float)RAND_MAX; 
	frand = r_min + frand*(r_max-r_min);
	return frand;
}
