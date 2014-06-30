#pragma once
#include "controllers/me_ct_data_interpolation.h"
//#include <sr/sr.h>

// for Inverse Blending Optimize() (steepest descent)
# define OPTM_MAX_ITER 80
# define OPTM_ADDL_ITER 30
# define OPTM_ITER_STEP 0.01f
# define OPTM_WT_UPBOUND 1.0f
# define OPTM_WT_LOBOUND 0.0f
# define OPTM_NO_HIT_TERM_TH 12

// for Inverse Blending OptimizeGradDes() (gradient descent)
# define OPTMGD_MAX_ITER 20
# define OPTMGD_ADDL_ITER 20
# define OPTMGD_NO_HIT_TERM_TH 12
# define OPTMGD_ITER_STEP 0.01f
# define OPTMGD_WT_UPBOUND 1.0f
# define OPTMGD_WT_LOBOUND 0.0f
# define OPTMGD_MAGNI_TH 0.70f // ignore small partial derivatives ( < 0.7x of largest desc)
# define OPTMGD_PATL_DEV_QTY 4 // max qty of partial derivatives


class InverseInterpolation : public KNNBaseInterpolator
{
protected:
	// still need to build a KNN for existing examples	
	VecOfInterpWeight prevWeight; // stored the previous weight	
public:
	InverseInterpolation();
	~InverseInterpolation();

public:
	virtual bool buildInterpolator();
	virtual void predictInterpWeights(const dVector& para, VecOfInterpWeight& blendWeights);
	virtual void drawInterpolator() {} // debugging information	

protected:
	int optimizeBlendWeight(const dVector& para, VecOfInterpWeight& blendWeight); // steepest descent
	int optimizeBlendWeightGradDes(const dVector& para, VecOfInterpWeight& blendWeight); // gradient descent

	float evaluateErrorFunction(const dVector& targetPara, VecOfInterpWeight& blendWeight);

	// sort index (descending order), *val content is unchanged. max value is at val[idx_array[0]].
	void sortIndexDesc(const float* val, int* idx_array, unsigned int size, bool init_idx_arr=true);

};