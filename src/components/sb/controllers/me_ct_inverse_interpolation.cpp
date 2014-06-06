#include <map>
#include <boost/foreach.hpp>
#include "controllers/me_ct_inverse_interpolation.h"


#define FREE_DATA(data) if (data) delete data; data=NULL;
/************************************************************************/
/* Inverse Interpolator                                             */
/************************************************************************/

InverseInterpolation::InverseInterpolation()
{
	numKNN  = 8;
}

InverseInterpolation::~InverseInterpolation()
{

}

bool InverseInterpolation::buildInterpolator()
{
	// we will use k-nearest neighbor as a starting point for optimization
	// so a KD-tree is still built for all examples.
	VecOfInterpExample& exampleData = exampleSet->getExamples();
#if USE_ANN
	VecOfInterpExample& exampleData = exampleSet->getExamples();
	// build KD-tree for KNN search
	int nPts = exampleData.size(); // actual number of data points
	int dim = exampleSet->parameterDim();
	dataPts = annAllocPts(nPts, dim); // allocate data points	
	for (int i=0;i<nPts;i++)
	{
		memcpy(dataPts[i],(const double*)(&exampleData[i]->parameter[0]),sizeof(ANNcoord)*dim);
	}	
	kdTree = new ANNkd_tree( // build search structure
		dataPts, // the data points
		nPts, // number of points
		dim); // dimension of space	 
#else
	kdPtSet.dim = exampleSet->parameterDim();
	kdPtSet.pts = exampleData;
	kdTree = new SBKDTree(exampleSet->parameterDim(),kdPtSet);
	kdTree->buildIndex();
#endif

	prevWeight.clear();
	prevWeight.push_back(InterpWeight(0,1.f));

	return true;
}


void InverseInterpolation::predictInterpWeights( const dVector& para, VecOfInterpWeight& blendWeights )
{
	vector<int> KNNIdx;
	vector<float> KNNDists, KNNWeights;
	VecOfInterpWeight tempWeight;

	//LOG("numKNN = %d",numKNN);
	// calculate the initial weights
	kdTreeKNN(kdTree,para,numKNN,tempWeight);
	pairToVec(tempWeight,KNNIdx,KNNDists);
	generateDistWeights(KNNDists,KNNWeights);
	//generateRandomWeight(numKNN, KNNWeights);
	vecToPair(KNNIdx,KNNWeights,tempWeight);	
	mapDistWeightToBlendWeight(exampleSet->getExamples(),tempWeight,blendWeights);	


	//	//printf("rough blend weight : %d\n",blendWeights.size());
	//// 	for (unsigned int i=0;i<blendWeights.size();i++)
	//// 	{
	//// 		//printf("%d : %.4f ", blendWeights[i].first, blendWeights[i].second);
	//// 		blendWeights[i].second = 1.f/blendWeights.size();	
	//// 	}
	//	//printf("\n");
	//	float prevError = evaluateErrorFunction(para,prevWeight);
	//	float initError = evaluateErrorFunction(para,blendWeights);
	//	//printf("prev error = %f, init error = %f\n",prevError,initError);
	//	if (prevError < initError)
	//		blendWeights = prevWeight;

	// Inverse Blending Optimization procedure
	int nIteration = optimizeBlendWeight(para,blendWeights); // steepest descent
	//int nIteration = optimizeBlendWeightGradDes(para,blendWeights); // gradient descent

	float finalError = evaluateErrorFunction(para,blendWeights);
	// 	printf("final blend weight : ");
	// 	for (unsigned int i=0;i<blendWeights.size();i++)
	// 	{
	// 		printf("%d : %.2f ", blendWeights[i].first, blendWeights[i].second);
	// 	}
	// 	printf("\n");


	// printf("init error = %f, final error = %f, num iterations = %d\n",initError, finalError,nIteration);

	prevWeight = blendWeights;

}


/* Inverse Blending steepest descent optimization method
see header file for definition of the following param:
OPTM_MAX_ITER (300)			max iteration
OPTM_ADDL_ITER (50)			additional iteration on steepest direction
OPTM_ITER_STEP (0.005f)		step to increase/decrease weight
OPTM_WT_UPBOUND (1.0f)		upper-bound for weight
OPTM_WT_LOBOUND (0.0f)		lower-bound for weight
OPTM_NO_HIT_TERM_TH (15)	terminate if no further desent occurs after # iterations
*/
int InverseInterpolation::optimizeBlendWeight( const dVector& para, VecOfInterpWeight& blendWeight )
{
	const unsigned int max_iter = OPTM_MAX_ITER; // maximum iterations before termination
	const unsigned int add_iter = OPTM_ADDL_ITER;  // additional iterations on steepest weight descent
	const float up_bound = OPTM_WT_UPBOUND; // upper-bound for weight increase
	const float lo_bound = OPTM_WT_LOBOUND; // lwoer-bound for weight decrease

	const float step =  OPTM_ITER_STEP; // step for weight variation
	bool dir; // direction for weight variation, true:+  false:-
	int steepest_idx = 0;
	bool steepest_dir = false; // true:+  false:-

	float fmin_inner = 0.0f; // fmin_inner = min(f_p, f_n)
	float fmin = evaluateErrorFunction(para,blendWeight);// fmin = min(fmin_inner), init with ObjEval()
	float f_p = 0.0f; // f(w+step)
	float f_n = 0.0f; // f(w-step)

	unsigned int counter = 0;
	unsigned int no_hit_counter = 0;	
	unsigned int total_iter = 0; // total iterations, iter + add_iter (for debug...)

	VecOfInterpWeight tempWeight = blendWeight;

	while(counter<max_iter)
	{
		for(unsigned int i=0; i<blendWeight.size(); i++)
		{
			if(blendWeight[i].second + step < up_bound) // limit weight within bound
			{
				tempWeight = blendWeight;
				tempWeight[i].second += step;
				normalizeBlendWeight(tempWeight);
				f_p = evaluateErrorFunction(para,tempWeight);
			}
			else f_p = fmin;

			if(blendWeight[i].second - step > lo_bound)
			{
				tempWeight = blendWeight;
				tempWeight[i].second -= step;				
				normalizeBlendWeight(tempWeight);			
				f_n = evaluateErrorFunction(para,tempWeight);				
			}
			else f_n = fmin;

			if(f_p < f_n)
			{
				fmin_inner = f_p;
				dir = true;
			}
			else
			{
				fmin_inner = f_n;
				dir = false;
			}

			if(fmin_inner < fmin)
			{
				steepest_idx = i;
				steepest_dir = dir;
				fmin = fmin_inner;				
				if(dir) blendWeight[i].second += step;
				else blendWeight[i].second -= step;
				normalizeBlendWeight(blendWeight);
				no_hit_counter = 0;
				total_iter ++;
			}
			else no_hit_counter++;
			if(no_hit_counter > OPTM_NO_HIT_TERM_TH) // goto terminate_iter;
				return total_iter;
		}
		counter++;

		// additional iterations on steepest weight
		for(int j =1; j<add_iter; j++)
		{
			if(steepest_dir && (blendWeight[steepest_idx].second+j*step < up_bound))
			{				
				tempWeight = blendWeight;
				tempWeight[steepest_idx].second += step*j;				
			}
			else if(!steepest_dir && (blendWeight[steepest_idx].second-j*step > lo_bound))
			{
				tempWeight = blendWeight;
				tempWeight[steepest_idx].second -= step*j;						
			}
			else break;

			normalizeBlendWeight(tempWeight);
			fmin_inner = evaluateErrorFunction(para,tempWeight);

			if(fmin_inner < fmin)
			{
				fmin = fmin_inner;
				blendWeight = tempWeight;
				total_iter ++;
			}
			else
				break;
		}
	}

	// terminate_iter:
	return total_iter;
}


/* Inverse Blending gradient descent optimization method, will run Optimize() in the end.
see header file for definition of the following param:
OPTMGD_MAX_ITER 100
OPTMGD_ADDL_ITER 50
OPTMGD_NO_HIT_TERM_TH 15
OPTMGD_ITER_STEP 0.005f
OPTMGD_WT_UPBOUND 1.0f
OPTMGD_WT_LOBOUND 0.0f
OPTMGD_MAGNI_TH 0.70f // ignore small partial derivatives ( < 0.7x of largest desc)
OPTMGD_PATL_DEV_QTY 4 // max qty of partial derivatives

note: testings show gradient descent reduces optimization iterations roughly by half
*/
int InverseInterpolation::optimizeBlendWeightGradDes( const dVector& para, VecOfInterpWeight& blendWeight )
{
	const unsigned int max_iter = OPTMGD_MAX_ITER; // maximum iterations before termination
	const unsigned int add_iter = OPTMGD_ADDL_ITER;  // additional iterations on steepest weight descent
	const float up_bound = OPTMGD_WT_UPBOUND; // upper-bound for weight increase
	const float lo_bound = OPTMGD_WT_LOBOUND; // lwoer-bound for weight decrease

	int patl_dev_qty = OPTMGD_PATL_DEV_QTY; // # of partial derivatives to calc gradient

	const float step =  OPTMGD_ITER_STEP; // step for weight variation
	int steepest_idx = 0;
	bool steepest_dir = false; // true:+  false:-

	float fmin = evaluateErrorFunction(para,blendWeight);// fmin = min(fmin_inner), init with ObjEval()
	float f_p = 0.0f; // f(w+step)
	float f_n = 0.0f; // f(w-step)
	float fmin_gd; // fmin on "gradient" direction
	float fmin_pregd = fmin; // fmin before gradient descent

	unsigned int counter = 0;
	unsigned int no_hit_counter = 0;
	unsigned int total_iter = 0; // total iterations, iter + add_iter (for debug...)

	unsigned int size = blendWeight.size();

	float* candit_eval_desct_array = new float[size]; // partial derivatives array (abs val)
	bool* candit_wt_dir_array = new bool[size]; // increase or decrease weight by grad vec. true:+  false:-
	int* sorted_candit_idx_array = new int[size]; // sorted (decending) index of candit_eval_desct_array
	float* grad_wt_step_array = new float[size]; // weight gradient vec (grad desc step)

	VecOfInterpWeight tempWeight = blendWeight;

	while(counter<max_iter)
	{
		for(unsigned int i=0; i<size; i++)
		{
			if(blendWeight[i].second + step < up_bound) // limit weight within bound
			{
				tempWeight = blendWeight;
				tempWeight[i].second += step;
				normalizeBlendWeight(tempWeight);
				f_p = evaluateErrorFunction(para,tempWeight);
			}
			else f_p = fmin;

			if(blendWeight[i].second - step > lo_bound)
			{
				tempWeight = blendWeight;
				tempWeight[i].second -= step;				
				normalizeBlendWeight(tempWeight);			
				f_n = evaluateErrorFunction(para,tempWeight);				
			}
			else f_n = fmin;

			if(f_p<f_n && f_p<fmin) // f_p is further descent
			{
				candit_eval_desct_array[i] = fmin - f_p;
				candit_wt_dir_array[i] = true;
				fmin_pregd = f_p;
			}
			else if(f_n<f_p && f_n<fmin)
			{
				candit_eval_desct_array[i] = fmin - f_n;
				candit_wt_dir_array[i] = false;
				fmin_pregd = f_n;
			}
			else
				candit_eval_desct_array[i] = 0.0f; // no further descent
		}

		// calc weight gradient vec using partial derivatives
		sortIndexDesc(&candit_eval_desct_array[0], sorted_candit_idx_array, size);
		int best_desc_idx = sorted_candit_idx_array[0]; // "best descent" element
		float best_desc = candit_eval_desct_array[best_desc_idx]; // max descent value (abs)
		if(best_desc <= srtiny) // 1E-6, see <sr/sr.h>
			goto TERMINATE_OPTMGD; // descent is not significant, terminate
		else
		{
CALC_GRAD_VEC:
			counter++;
			for(unsigned int i=0; i<size; i++)
				grad_wt_step_array[i] = 0.0f; // init grad_wt_step_array with zeros

			/* select largest N partial derivatives
			// if N=1 then only using largest partial derivative as gradient */
			for(int i=0; i<patl_dev_qty; i++) 
			{
				int sort_idx = sorted_candit_idx_array[i];
				float cur_ev = candit_eval_desct_array[sort_idx];
				float ratio = cur_ev / best_desc;
				if(ratio > OPTMGD_MAGNI_TH) // incld cur_ev for gradient calc
				{
					if(candit_wt_dir_array[sort_idx])
						grad_wt_step_array[sort_idx] = ratio * step;
					else
						grad_wt_step_array[sort_idx] = -ratio * step;
				}
				else break; // stop if magnitude too small
			} // weight gradient vec is ready
		}

		// descent along weight gradient vec
		bool out_bound = false;
		bool minima_reached = false;
		for(int i=1; !minima_reached && !out_bound; i++)
		{
			tempWeight = blendWeight;
			for(unsigned int j=0; j<size; j++)
			{
				if(blendWeight[j].second + grad_wt_step_array[j] * i > up_bound) // out of bound
				{
					out_bound = true;
					break; // stop addl iter
				}
				else if(blendWeight[j].second + grad_wt_step_array[j] * i < lo_bound)
				{
					out_bound = true;
					break;
				}
				else
					tempWeight[j].second += grad_wt_step_array[j] * i;
			}
			if(out_bound) // need to re-calc grad_wt_step_array !!
			{
				if(patl_dev_qty>1)
				{
					patl_dev_qty--;
					goto CALC_GRAD_VEC; // partial restart
				}
				else // patl_dev_qty==1
					goto TERMINATE_OPTMGD;
			}

			total_iter++;

			normalizeBlendWeight(tempWeight);
			fmin_gd = evaluateErrorFunction(para,tempWeight);

			if(fmin_gd >= fmin) // no further descent occured
			{
				minima_reached = true;
				if(i==1) // no further desc
					goto TERMINATE_OPTMGD;
				else
					break; // restart from beginning
			}
			else // continue grad desc
			{
				fmin = fmin_gd;
				blendWeight = tempWeight; // update weights
			}

		}// end of grad desc iterations

	} // end of while(counter<max_iter)

TERMINATE_OPTMGD:
	delete [] sorted_candit_idx_array;
	delete [] candit_eval_desct_array;
	delete [] grad_wt_step_array;
	delete [] candit_wt_dir_array;

	return total_iter + optimizeBlendWeight(para,blendWeight);
}


float InverseInterpolation::evaluateErrorFunction( const dVector& targetPara, VecOfInterpWeight& blendWeight )
{
	static InterpolationExample* ex = NULL;
	if (!ex) ex = exampleSet->createPseudoExample();
	dVector exPara;
	ex->weight = blendWeight;
	ex->getExampleParameter(exPara);	
	//exPara = exPara - targetPara;
	double error = 0.f;
	//printf("target para : ");
	for (unsigned int i=0;i<exPara.size();i++)
	{
		//printf("%f ", targetPara(i));
		double diff = exPara(i) - targetPara(i);
		error += diff*diff;
	}
	//printf("\n");
	return (float)sqrtf((float)error);
	//return (float)ublas::norm_2(exPara);
}

// sort index (descending order), *val content is unchanged. max value is at val[idx_array[0]].
void InverseInterpolation::sortIndexDesc(const float* val, int* idx_array, unsigned int size, bool init_idx_arr)
{
	float* _val = new float[size];
	memcpy(_val, val, sizeof(float)*size); // copy val array

	if(init_idx_arr)
	{
		for(unsigned int i=0; i<size; i++)
			idx_array[i]=i; // init sort index array
	}

	float f_tmp;
	int i_tmp;
	for(unsigned int i=0; i<size-1 ;i++)
	{
		for(unsigned int j = i+1 ; j<size ;j++)
		{
			if(_val[i] < _val[j])
			{
				f_tmp = _val[i]; _val[i] = _val[j]; _val[j] = f_tmp;
				i_tmp = idx_array[i]; idx_array[i] = idx_array[j]; idx_array[j] = i_tmp;
			}
		}
	}

	delete [] _val;
}