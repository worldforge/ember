#include "SBAnimationState.h"
#include <controllers/MotionAnalysis.h>
#include <sb/SBMotion.h>
#include <sb/SBCharacter.h>
#include <sb/SBScene.h>
#include <sb/SBSkeleton.h>
#include <sb/SBEvent.h>
#include <sb/SBAssetManager.h>

#include <boost/algorithm/string.hpp>

#include <sr/sr.h>
#include <sr/sr_lines.h>

namespace SmartBody {

SBAnimationBlend::SBAnimationBlend() : PABlend()
{
	_isFinalized = false;
	motionAnalysis = NULL;
}

SBAnimationBlend::SBAnimationBlend(const std::string& name) : PABlend(name)
{
	_isFinalized = false;
	motionAnalysis = NULL;
}

SBAnimationBlend::~SBAnimationBlend()
{
	if (motionAnalysis)
		delete motionAnalysis;
}

void SBAnimationBlend::setIncrementWorldOffsetY(bool flag)
{
	incrementWorldOffsetY = flag;
}

#ifdef EMBER_SB_SURFACE
SrSnColorSurf* SBAnimationBlend::createFlatSurface( float depth, unsigned int dimension, SrVec2 topLeft, SrVec2 lowerRight )
{
	SrSnColorSurf* surf = new SrSnColorSurf(); surf->ref();	
	SrModel* surf_model = surf->model();

	float xPos, yPos, zPos = depth;
	const int dim = dimension;
	const int size = dim * dim;	
	SrVec pnt;
	float xSize = lowerRight[0] - topLeft[0];
	float ySize = lowerRight[1] - topLeft[1];
	SrColor temp_c;
	SrMaterial mtl;

	SrArray<SrVec> grid_array;
	// generate vertices for grid edges/surf
	for (int i=0; i<dim; i++)
	{		
		yPos = topLeft[1] + float(ySize/dim*i);
		for (int j=0; j<dim; j++)
		{			
			xPos = topLeft[0] + float(xSize/dim*j);
			pnt = SrVec(xPos, yPos, zPos);
			grid_array.push(pnt);
		}
	}

	// build surf
	surf_model->init();
	surf_model->culling = false; // !!! back-face culling can be enabled/disabled here !!!
	for(int i=0; i<dim; i++)
	{
		//theta = -(float)(SR_PI/9) + (float)(SR_PI*4/3/ dim * i);
		yPos = topLeft[1] + float(ySize/dim*i);
		for(int j=0; j<dim; j++)
		{
			//phi = (float)(SR_PI/3.5) + (float)(SR_PI/2 / dim * j); 
			xPos = topLeft[0] + float(xSize/dim*j);
			//pnt = SrVec( float(ctr.x+r*sin(phi)*cos(theta)),float(ctr.y+r*cos(phi)-j/2.2), ctr.z-r*sin(phi)*sin(theta) );
			pnt = SrVec( xPos, yPos, zPos );

			//surf_model->V.push(pnt); // set sphere as surf
			surf_model->V.push().set(grid_array[i*dim + j]);
			VecOfInt adjs;
			for (int x=-1;x<=1;x++) // get adjacent vertices
			{
				for (int y=-1;y<=1;y++)
				{
					if (x==0 && y==0) continue;
					int nx = (i+x); int ny = (j+y);
					if (nx < 0 || nx >= dim || ny < 0 || ny >= dim) continue;
					int adjIdx = nx*dim + (ny);
					adjs.push_back(adjIdx);
				}
			}
			surf->vtxAdjList.push_back(adjs);
			temp_c = SrColor::interphue((float)i / dim);
			mtl.diffuse.set(temp_c.r, temp_c.g, temp_c.b, (srbyte)255);
			surf_model->M.push(mtl);
		}
	}

	// make faces out of vertex
	for (int i=0; i<dim-1; i++)
	{
		for (int j=0; j<dim-1; j++)
		{
			surf_model->F.push().set( i*dim+j, i*dim+j+1, (i+1)*dim+j+1 );
			surf_model->F.push().set( i*dim+j, (i+1)*dim+j+1, (i+1)*dim+j );
		}
	}
	surf->changed(true); // force update surf
	//surf->render_mode(srRenderMode::srRenderModeDefault);
	//surf->render_mode(srRenderMode::srRenderModeFlat);
	return surf;
}


SrSnColorSurf* SBAnimationBlend::createCurveSurface( float radius, unsigned int dimension, SrVec center, SrVec2 phiRange, SrVec2 thetaRange )
{
	SrSnColorSurf* surf = new SrSnColorSurf(); surf->ref(); // color surf
	SrModel* surf_model = surf->model();

	float phi, theta, r = radius;
	const int dim = dimension;
	const int size = dim * dim;
	const SrVec ctr = center;
	SrVec pnt;

	SrColor temp_c;
	SrMaterial mtl;

	SrArray<SrVec> grid_array;

	// generate vertices for grid edges/surf
	for (int i=0; i<dim; i++)
	{
		//theta = -(float)(SR_PI/9) + (float)(SR_PI*4/3 / dim * i);
		theta = thetaRange[0] + float(thetaRange[1]/dim*i);
		for (int j=0; j<dim; j++)
		{
			//phi = (float)(SR_PI/3.5) + (float)(SR_PI/2 / dim * j); 
			phi = phiRange[0] + float(phiRange[1]/dim*j);

			//pnt = SrVec( float(ctr.x+r*sin(phi)*cos(theta)), float(ctr.y+r*cos(phi)-j/2.2), float(ctr.z-r*sin(phi)*sin(theta)) );
			pnt = SrVec( float(ctr.x+r*sin(phi)*sin(theta)), float(ctr.y+r*cos(phi)), float(ctr.z+r*sin(phi)*cos(theta)) );
			grid_array.push(pnt);
		}
	}

	// build surf
	surf_model->init();
	surf_model->culling = false; // !!! back-face culling can be enabled/disabled here !!!
	for(int i=0; i<dim; i++)
	{
		//theta = -(float)(SR_PI/9) + (float)(SR_PI*4/3/ dim * i);
		theta = thetaRange[0] + float(thetaRange[1]/dim*i) ;
		for(int j=0; j<dim; j++)
		{
			//phi = (float)(SR_PI/3.5) + (float)(SR_PI/2 / dim * j); 
			phi = phiRange[0] + float(phiRange[1]/dim*j);
			//pnt = SrVec( float(ctr.x+r*sin(phi)*cos(theta)),float(ctr.y+r*cos(phi)-j/2.2), ctr.z-r*sin(phi)*sin(theta) );
			pnt = SrVec( float(ctr.x+r*sin(phi)*sin(theta)), float(ctr.y+r*cos(phi)), float(ctr.z+r*sin(phi)*cos(theta)) );

			//surf_model->V.push(pnt); // set sphere as surf
			surf_model->V.push().set(grid_array[i*dim + j]);
			VecOfInt adjs;
			for (int x=-1;x<=1;x++) // get adjacent vertices
			{
				for (int y=-1;y<=1;y++)
				{
					if (x==0 && y==0) continue;
					int nx = (i+x); int ny = (j+y);
					if (nx < 0 || nx >= dim || ny < 0 || ny >= dim) continue;
					int adjIdx = nx*dim + (ny);
					adjs.push_back(adjIdx);
				}
			}
			surf->vtxAdjList.push_back(adjs);

			temp_c = SrColor::interphue((float)i / dim);
			mtl.diffuse.set(temp_c.r, temp_c.g, temp_c.b, (srbyte)255);
			surf_model->M.push(mtl);
		}
	}

	// make faces out of vertex
	for (int i=0; i<dim-1; i++)
	{
		for (int j=0; j<dim-1; j++)
		{
			surf_model->F.push().set( i*dim+j, i*dim+j+1, (i+1)*dim+j+1 );
			surf_model->F.push().set( i*dim+j, (i+1)*dim+j+1, (i+1)*dim+j );
		}
	}
	surf->changed(true); // force update surf
	//surf->render_mode(srRenderMode::srRenderModeDefault);
	//surf->render_mode(srRenderMode::srRenderModeFlat);
	return surf;
}

void SBAnimationBlend::createErrorSurfaces( const std::string& type, SrVec center, int segments, int dimensions, std::vector<SrSnColorSurf*>& surfList )
{
	if (type == "curve")
	{
		float minDist = 1e30f, maxDist = -1e30f;
		float minPhi = 1e30f, maxPhi = -1e30f;
		float minTheta = 1e30f, maxTheta = -1e30f;
		float toDegree = 180.f/(float)SR_PI;
		for (unsigned int i=0;i<parameters.size();i++)
		{		
			SrVec offset = parameters[i] - center;
			//LOG("parameter = %f %f %f",parameters[i][0],parameters[i][1],parameters[i][2]);
			//offset.y = 0.f; // ignore y distance		
			float curDist = offset.norm();
			if (curDist < minDist)
				minDist = curDist;
			if (curDist > maxDist)
				maxDist = curDist;
			SrVec offsetDir = parameters[i] - center;		
			offsetDir.normalize();
			float cosValue = offsetDir.y;		
			float tanValue = offsetDir.x/offsetDir.z;
			//float phi = acosf(offsetDir.y);//asinf(sqrtf(1.f-offsetDir.y*offsetDir.y));
			float phi = atan2f(sqrtf(offsetDir.x*offsetDir.x+offsetDir.z*offsetDir.z),offsetDir.y);
			//if (offsetDir.y < 0) phi = -phi;
			float theta = atan2f(offsetDir.x, offsetDir.z);//SR_PI - atan2f(offsetDir.x,offsetDir.z);
			if (theta > (float)SR_PI*3.f/4.f) theta = theta - (float)SR_PI*2;
			//float theta = atan2f(offsetDir.z,offsetDir.x);

			//LOG("tan = %f, theta = %f, cos = %f, phi = %f",tanValue, theta*toDegree, cosValue, phi*toDegree);
			if (theta < minTheta) minTheta = theta;
			if (theta > maxTheta) maxTheta = theta;
			if (phi < minPhi) minPhi = phi;
			if (phi > maxPhi) maxPhi = phi;
		}	 
		minDist = (minDist+maxDist)*0.5f; // make sure "near surf" is not too close
		//LOG("maxPhi = %f, minPhi = %f, maxTheta = %f, minTheta = %f",maxPhi*toDegree, minPhi*toDegree, maxTheta*toDegree, minTheta*toDegree);
		SrVec2 thetaRange = SrVec2(minTheta, maxTheta-minTheta);
		SrVec2 phiRange = SrVec2(minPhi, maxPhi-minPhi);
		if(segments==0 || maxDist-minDist<srtiny) // creat only one surface in the middle
		{
			float radius = (minDist+maxDist)*0.5f;
			SrSnColorSurf* Surf = createCurveSurface(radius, dimensions, center, phiRange, thetaRange);
			surfList.push_back(Surf);
		}
		else
		{
			float distOffset = (maxDist - minDist)/segments;
			for (float radius = minDist; radius <= maxDist + distOffset*0.05f; radius+= distOffset)
			{
				SrSnColorSurf* Surf = createCurveSurface(radius, dimensions, center, phiRange, thetaRange);
				surfList.push_back(Surf);
				//updateErrorSurace(Surf, center);
			}
		}
	}
	else if ( type == "flat")
	{
		SrVec bboxMin = SrVec(1e30f, 1e30f, 1e30f);
		SrVec bboxMax = SrVec(-1e30f, -1e30f, -1e30f);
		for (unsigned int i=0;i<parameters.size();i++)
		{
			SrVec& pos = parameters[i];
			for (int k=0;k<3;k++)
			{
				if (pos[k] < bboxMin[k])
					bboxMin[k] = pos[k];
				if (pos[k] > bboxMax[k])
					bboxMax[k] = pos[k];
			}
		}
		SrVec2 topLeft = SrVec2(bboxMin[0],bboxMin[1]);
		SrVec2 lowerRight = SrVec2(bboxMax[0],bboxMax[1]);
		SrVec2 surfSize = lowerRight - topLeft;
		float surfaceScale = max(surfSize[0],surfSize[1]);

		float minDist = bboxMin[2];
		float maxDist = bboxMax[2];
		if (segments == 0)
		{
			minDist = (minDist+maxDist)*0.5f;
		}
		if(segments==0 || maxDist-minDist<srtiny) // creat only one surface in the middle
		{
			float depth = (minDist+maxDist)*0.5f;
			SrSnColorSurf* Surf = createFlatSurface(depth, dimensions, topLeft, lowerRight);
			surfList.push_back(Surf);
		}
		else
		{
			float distOffset = (maxDist - minDist)/segments;
			for (float depth = minDist; depth <= maxDist; depth+= distOffset)
			{
				SrSnColorSurf* Surf = createFlatSurface(depth, dimensions, topLeft, lowerRight);
				surfList.push_back(Surf);
				Surf->surfaceScale = SrVec(surfaceScale/surfSize[0], surfaceScale/surfSize[1], 1.f);
				//updateErrorSurace(Surf, center);
			}
		}
	}	
}

void SBAnimationBlend::updateSmoothSurface( SrSnColorSurf* surf )
{
	SrModel* surfModel = surf->model();	
	std::vector<VecOfDouble> weightList;
	float maxError = 1e-30f;
	float totalError = 0.f;
	for (int i=0;i<surfModel->V.size();i++)
	{
		SrVec para = surfModel->V[i];
		std::vector<double> weights;
		PABlend::getWeightsFromParameters(para[0],para[1],para[2],weights);		
		weightList.push_back(weights);		
	}


	// compute the smoothness factor based on parameter weights
	std::vector<float> smoothList;
	float maxSmooth = 1e-30f;
	float totalSmooth = 0.f;
	for (int i=0;i<surfModel->V.size();i++)
	{
		const VecOfInt& adjIdx = surf->vtxAdjList[i];
		VecOfDouble laplacian = weightList[i];
		float ratio = 1.f/adjIdx.size();
		for (unsigned int k=0;k<adjIdx.size();k++)
		{
			const VecOfDouble& adjWeight = weightList[adjIdx[k]];
			for (unsigned int idx=0;idx<laplacian.size();idx++)
			{
				laplacian[idx] -= adjWeight[idx]*ratio;
			}			
		}
		float smooth = 0.f;
		for (unsigned int idx=0;idx<laplacian.size();idx++)
		{
			smooth += (float)(laplacian[idx]*laplacian[idx]);
		}
		smooth = sqrtf(smooth);
		totalSmooth += smooth;
		if (smooth > maxSmooth) maxSmooth = smooth;
		smoothList.push_back(smooth);
	}

	totalSmooth /= smoothList.size();
	LOG("total avg smooth = %f",totalSmooth);
	SrColor temp_c;
	SrMaterial mtl;
	surfModel->M.remove(0,smoothList.size());
	for (unsigned int i=0;i<smoothList.size();i++)
	{
		//surfModel->M.push()		
		float curError = (float)(smoothList[i]/maxSmooth);
		if (curError > 1.f) curError = 1.f;

		temp_c = SrColor::interphue(curError);
		mtl.diffuse.set(temp_c.r, temp_c.g, temp_c.b, (srbyte)160);
		surfModel->M.push(mtl);	
	}
	surf->changed(true);
}


void SBAnimationBlend::updateErrorSurace( SrSnColorSurf* errorSurf, SrVec center )
{
	SrModel* surfModel = errorSurf->model();
	std::vector<float> errorList;
	std::vector<VecOfDouble> weightList;
	float maxError = 1e-30f;
	float totalError = 0.f;
	for (int i=0;i<surfModel->V.size();i++)
	{
		SrVec para = surfModel->V[i];
		std::vector<double> weights;
		PABlend::getWeightsFromParameters(para[0],para[1],para[2],weights);
		float x,y,z;
		getParametersFromWeights(x,y,z,weights);

		float paraError = (para - SrVec(x,y,z)).norm();
		if (paraError > maxError)
			maxError = paraError;
		errorList.push_back(paraError);
		weightList.push_back(weights);
		totalError += paraError;
	}

	totalError /= errorList.size();
	LOG("total avg error = %f",totalError);
	SrColor temp_c;
	SrMaterial mtl;
	//maxError = 0.4f;
	surfModel->M.remove(0,errorList.size());
	for (unsigned int i=0;i<errorList.size();i++)
	{
		//surfModel->M.push()		
		float curError = (float)(errorList[i]/maxError);
		if (curError > 1.f) curError = 1.f;

		temp_c = SrColor::interphue(curError);
		mtl.diffuse.set(temp_c.r, temp_c.g, temp_c.b, (srbyte)160);
		surfModel->M.push(mtl);	
	}
	errorSurf->changed(true);
}



void SBAnimationBlend::buildVisSurfaces( const std::string& errorType, const std::string& surfaceType, int segments, int dimensions )
{
	if (errorType != "error" && errorType != "smooth")
	{
		LOG("Warning : errorType must be 'error' or 'smooth'");
		return;
	}
	if (_dimension != "3D")
	{
		LOG("Warning : build Vis Surface only works for 3D parameterization state");
	}

	std::vector<SrSnColorSurf*>& surfaces = (errorType == "error") ? errorSurfaces : smoothSurfaces;
	//SBSkeleton* sbSkel = SBScene::getScene()->getSkeleton(skeletonName);
	SrVec center = SrVec(0,0,0);
	//if (sbSkel && surfaceType == "curve")
	//	center = sbSkel->getJointByMappedName("base")->gmat().get_translation();
	createErrorSurfaces(surfaceType, center, segments, dimensions, surfaces);
	for (unsigned int i=0;i<surfaces.size();i++)
	{
		SrSnColorSurf* surf = surfaces[i];
		if (errorType == "error")
			updateErrorSurace(surf, center);
		else if (errorType == "smooth")
			updateSmoothSurface(surf);
	}	
}
#endif

// Motion Vector Flow visualization, added by David Huang  June 2012
void SBAnimationBlend::createMotionVectorFlow(const std::string& motionName, const std::string& chrName,
											   float plotThreshold, unsigned int slidWinHalfSize, bool clearAll)
{
	SkMotion* mo = SBAnimationBlend::getSkMotion(motionName);
	if(mo==0) return;
	if(mo->frames()<3)
	{
		LOG("createMotionVectorFlow(): motion does not have enough frames, aborting...");
		return;
	}
	SkSkeleton* sk = mo->connected_skeleton();
	if(sk==0)
	{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SBCharacter* sbSk = scene->getCharacter(chrName);
		if(sbSk)
		{
			std::string sbName = sbSk->getSkeleton()->getName();
			sk = SmartBody::SBScene::getScene()->getSkeleton(sbName);
		}
		if(sk)
		{
			LOG("%s connected to %s for plotting.", motionName.c_str(), chrName.c_str());
			mo->connect(sk);
		}
		else
		{
			LOG("createMotionVectorFlow(): motion not connected to any skeleton, aborting...");
			return;
		}
	}

	SR_CLIP(plotThreshold, 0.0f, 1.0f);

	if(clearAll)
		clearMotionVectorFlow(); // clear vector flow SrSnLines

	const std::vector<SkJoint*>& jnts = sk->joints();
	createJointExclusionArray(jnts); // making a list of excluded joints
	const int jsize = jnts.size();
	const int frames = mo->frames();
	const float dur = mo->duration();
	const float intv = dur / (frames-1);
	SrArray<SrArray<SrVec>*> pnts_arr; // jsize X frames
	for(int i=0; i<frames; i++)
	{
		pnts_arr.push(new SrArray<SrVec>);
		// mo->apply_frame(i); // use original frames
		mo->apply(intv * i); // resample motion time uniformly
		sk->invalidate_global_matrices();
		sk->update_global_matrices();
		getJointsGPosFromSkel(sk, *pnts_arr.top(), jnts);
	}

	// LOG("createMotionVectorFlow(): max vector norm = %f \n", getVectorMaxNorm(pnts_arr));

	// compute all vector norms
	SrArray<SrArray<float>*> norm_arr; // jsize X (frames-1)
	for(int i=0; i<frames-1; i++)
	{
		SrArray<float>* n_arr = new SrArray<float>;
		norm_arr.push(n_arr);
		n_arr->capacity(jsize);
		n_arr->size(jsize);
		for(int k=0; k<jsize; k++)
		{
			SkJoint* j = jnts[k]; if(isExcluded(j)) continue;
			SrVec curr_p = pnts_arr[i]->get(k);
			SrVec next_p = pnts_arr[i+1]->get(k);
			n_arr->set(k, dist(curr_p, next_p));
		}
	}

	// compute norm threshold with sliding window
	SrArray<SrArray<float>*> norm_th_arr; // jsize X (frames-1)
	for(int i=0; i<frames; i++)
	{
		SrArray<float>* n_arr = new SrArray<float>;
		norm_th_arr.push(n_arr);
		n_arr->capacity(jsize);
		n_arr->size(jsize);
		float th;
		for(int k=0; k<jsize; k++)
		{
			SkJoint* j = jnts[k]; if(isExcluded(j)) continue;
			th = 0.0f;
			for(unsigned int j=i-slidWinHalfSize; j<=i+slidWinHalfSize; j++)
			{
				int m = j;
				SR_CLIP(m, 0, frames-2);
				th = th + norm_arr[m]->get(k);
			}
			th = th / (2*slidWinHalfSize+1);
			n_arr->set(k, th);
		}
	}

	// plot vector flow
	for(int i=1; i<mo->frames()-1; i++)
	{
		SrSnLines* l = new SrSnLines; l->ref();
		l->resolution(VFLOW_LINE_WIDTH); // change vector flow lines thickness
		vecflowLinesArray.push_back(l);
		SrLines& line = l->shape();
		line.init();
		for(int k=0; k<jsize; k++)
		{
			SkJoint* j = jnts[k]; if(isExcluded(j)) continue;
			const SrVec& curr_p = pnts_arr[i]->get(k);
			const SrVec& next_p = pnts_arr[i+1]->get(k);
			const float& norm = norm_arr[i]->get(k);
			const float& th = norm_th_arr[i]->get(k);
			float c = norm / th;
			if(c > 1.0f+plotThreshold)
			{
				c = c - (1.0f+plotThreshold);
				c = c + 0.5f; // use 0.5~1 hue space
				SR_CLIP(c, 0.0f, 1.0f);
				line.push_color(SrColor::interphue(c));
				line.push_line(curr_p, next_p);
			}
			else if(c < 1.0f-plotThreshold)
			{
				c = c - (1.0f-plotThreshold);
				c = c - 0.5f; // use 0~0.5 hue space
				SR_CLIP(c, 0.0f, 1.0f);
				line.push_color(SrColor::interphue(c));
				line.push_line(curr_p, next_p);
			}
			else
			{
				line.push_color(SrColor::lightgray);
				line.push_line(curr_p, next_p);
			}
		}
	}
}

void SBAnimationBlend::clearMotionVectorFlow(void)
{
	// clear vector flow lines
	for(unsigned int i=0; i<vecflowLinesArray.size(); i++)
		vecflowLinesArray[i]->unref();
	vecflowLinesArray.resize(0);
}

void SBAnimationBlend::plotMotion(const std::string& motionName, const std::string& chrName, unsigned int interval,
								   bool clearAll, bool useRandomColor)
{
	SkMotion* mo = SBAnimationBlend::getSkMotion(motionName);
	if(mo==0) return;
	if(mo->frames()<3)
	{
		LOG("plotMotion(): motion does not have enough frames, aborting...");
		return;
	}
	SkSkeleton* sk = mo->connected_skeleton();
	if(sk==0)
	{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SBCharacter* sbSk = scene->getCharacter(chrName);
		if(sbSk)
		{
			std::string sbName = sbSk->getSkeleton()->getName();
			sk = SmartBody::SBScene::getScene()->getSkeleton(sbName);
		}
		if(sk)
		{
			LOG("%s connected to %s for plotting.", motionName.c_str(), chrName.c_str());
			mo->connect(sk);
		}
		else
		{
			LOG("plotMotion(): motion not connected to any skeleton, aborting...");
			return;
		}
	}

	if(interval < 2) interval = 2;

	// clear plot motion SrSnLines
	if(clearAll)
		clearPlotMotion();


	const SrColor color_begin = SrColor::blue;
	const SrColor color_end = SrColor::red;
	SrColor color_random;
	if(useRandomColor)
	{
		static float hue = 0.0f;
		color_random = SrColor::interphue(hue);
		hue += 0.1f;
		if(hue>1.0f) hue = 0.0f;
	}
	const std::vector<SkJoint*>& jnts = sk->joints();
	createJointExclusionArray(jnts); // making a list of excluded joints
	float mo_dur = mo->duration();
	for(unsigned int i=0; i<=interval; i++)
	{
		mo->apply(mo_dur/interval * i);
		sk->invalidate_global_matrices();
		sk->update_global_matrices();

		SrSnLines* l = new SrSnLines; l->ref();
		plotMotionLinesArray.push_back(l);
		SrLines& line = l->shape();

		line.init();
		if(useRandomColor) line.push_color(color_random);
		else line.push_color(lerp(color_begin, color_end, (float)i/interval));
		for(unsigned int k=0; k<jnts.size(); k++) // set k=1 to skip ref root
		{
			SkJoint* j = jnts[k];
			if(isExcluded(j)) continue;
			for(int m=0; m<j->num_children();m++)
			{
				if(isExcluded(j->child(m))) continue;
				line.push_line(j->gcenter(), j->child(m)->gcenter());
			}
		}
	}
}

void SBAnimationBlend::plotMotionFrameTime(const std::string& motionName, const std::string& chrName,
										   float time, bool useRandomColor)
{
	SkMotion* mo = SBAnimationBlend::getSkMotion(motionName);
	if(mo==0) return;
	if(mo->frames()<3)
	{
		LOG("plotMotionFrame(): motion does not have enough frames, aborting...");
		return;
	}
	SkSkeleton* sk = mo->connected_skeleton();
	if(sk==0)
	{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SBCharacter* sbSk = scene->getCharacter(chrName);
		if(sbSk)
		{
			std::string sbName = sbSk->getSkeleton()->getName();
			sk = SmartBody::SBScene::getScene()->getSkeleton(sbName);
		}
		if(sk)
		{
			LOG("%s connected to %s for plotting.", motionName.c_str(), chrName.c_str());
			mo->connect(sk);
		}
		else
		{
			LOG("plotMotionFrame(): motion not connected to any skeleton, aborting...");
			return;
		}
	}

	const std::vector<SkJoint*>& jnts = sk->joints();
	createJointExclusionArray(jnts); // making a list of excluded joints

	mo->apply(time);
	//sk->invalidate_global_matrices();
	sk->update_global_matrices();

	SrSnLines* l = new SrSnLines; l->ref();
	plotMotionLinesArray.push_back(l);
	SrLines& line = l->shape();
	line.init();
	if(useRandomColor)
	{
		static float hue = 0.0f;
		line.push_color(SrColor::interphue(hue));
		hue += 0.1f;
		if(hue>1.0f) hue = 0.0f;
	}
	else
		line.push_color(SrColor::lightgray);
	for(unsigned int k=0; k<jnts.size(); k++) // set k=1 to skip ref root
	{
		SkJoint* j = jnts[k];
		if(isExcluded(j)) continue;
		for(int m=0; m<j->num_children();m++)
		{
			if(isExcluded(j->child(m))) continue;
			line.push_line(j->gcenter(), j->child(m)->gcenter());
		}
	}
}

void SBAnimationBlend::plotMotionJointTrajectory(const std::string& motionName, const std::string& chrName,
												 const std::string& jointName, float start_t, float end_t, bool useRandomColor)
{

	SkMotion* mo = SBAnimationBlend::getSkMotion(motionName);
	if(mo==0) return;
	if(mo->frames()<3)
	{
		LOG("plotMotionJointTrajectory(): motion does not have enough frames, aborting...");
		return;
	}
	SkSkeleton* sk = mo->connected_skeleton();
	if(sk==0)
	{
		
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SBCharacter* sbSk = scene->getCharacter(chrName);
		if(sbSk)
		{
			std::string sbName = sbSk->getSkeleton()->getName();
			sk = SmartBody::SBScene::getScene()->getSkeleton(sbName);
		}
		if(sk)
		{
			LOG("%s connected to %s for plotting.", motionName.c_str(), chrName.c_str());
			mo->connect(sk);
		}
		else
		{
			LOG("plotMotionJointTrajectory(): motion not connected to any skeleton, aborting...");
			return;
		}
	}
	SkJoint* jnt = sk->search_joint(jointName.c_str());
	if(!jnt)
	{
		LOG("plotMotionJointTrajectory(): specified joint is not found in skeleton, aborting...");
		return;
	}
	
	unsigned int frames = mo->frames();
	const float mo_dur = mo->duration();
	if(end_t <= 0.0f) end_t = mo_dur;
	SR_CLIP(start_t, 0.0f, mo_dur);
	SR_CLIP(end_t, 0.0f, mo_dur);
	const SrColor color_begin = SrColor::blue;
	const SrColor color_end = SrColor::red;
	SrColor color_random;
	if(useRandomColor)
	{
		static float hue = 0.0f;
		color_random = SrColor::interphue(hue);
		hue += 0.1f;
		if(hue>1.0f) hue = 0.0f;
	}
	SrSnLines* l = new SrSnLines; l->ref();
	plotMotionLinesArray.push_back(l);
	SrLines& line = l->shape();
	line.init();
	mo->apply(start_t);
	sk->update_global_matrices();
	SrVec last_jnt_pos = jnt->gcenter(); // first frame
	for(float t=start_t; t<end_t; t+=0.016667f)
	{
		if(useRandomColor) line.push_color(color_random);
		else line.push_color(lerp(color_begin, color_end, (t-start_t)/(end_t-start_t))); // begin~end: blue~red
		mo->apply(t);
		sk->update_global_matrices();
		SrVec cur_jnt_pos = jnt->gcenter();
		line.push_line(last_jnt_pos, cur_jnt_pos);
		last_jnt_pos = cur_jnt_pos;
	}
}


void SBAnimationBlend::clearPlotMotion(void)
{
	for(unsigned int i=0; i<plotMotionLinesArray.size(); i++)
		plotMotionLinesArray[i]->unref();
	plotMotionLinesArray.resize(0);
}

void SBAnimationBlend::setChrPlotMotionTransform(const std::string& chrName)
{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SBCharacter* sbSk = scene->getCharacter(chrName);
		if(sbSk)
		{
			SrVec hpr = sbSk->getHPR();
			setPlotMotionTransform(sbSk->getPosition(), SR_TORAD(hpr.x));
		}
}

void SBAnimationBlend::setPlotMotionTransform(SrVec offset, float yRot)
{
	SrQuat q(SrVec::j, yRot);
	q.get_mat(plotMotionTransform);
	plotMotionTransform.setl4(offset);
}


void SBAnimationBlend::setChrPlotVectorFlowTransform(const std::string& chrName)
{
		SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
		SBCharacter* sbSk = scene->getCharacter(chrName);
		if(sbSk)
		{
			SrVec hpr = sbSk->getHPR();
			setPlotVectorFlowTransform(sbSk->getPosition(), SR_TORAD(hpr.x));
		}
}

void SBAnimationBlend::setPlotVectorFlowTransform(SrVec offset, float yRot)
{
	SrQuat q(SrVec::j, yRot);
	q.get_mat(plotVectorFlowTransform);
	plotVectorFlowTransform.setl4(offset);
}


void SBAnimationBlend::getJointsGPosFromSkel(SkSkeleton* sk, SrArray<SrVec>& pnts_array,
											  const std::vector<SkJoint*>& jnt_list)
{
	const unsigned int size = jnt_list.size();
	pnts_array.capacity(size);
	pnts_array.size(size);

	for(unsigned int k=0; k<size; k++) // set k=1 to skip ref root
	{
		SkJoint* j = jnt_list[k];
		pnts_array[k] = j->gcenter();
	}
}

float SBAnimationBlend::getVectorMaxNorm(SrArray<SrArray<SrVec>*>& pnts_arr)
{
	const int frames = pnts_arr.size();
	const int size = pnts_arr[0]->size();
	float max_err = 0.0f;
	for(int i=1; i<frames; i++)
	{
		for(int k=0; k<size; k++)
		{
			float norm = dist(pnts_arr[i]->get(k), pnts_arr[i-1]->get(k));
			if(norm > max_err)
				max_err = norm;
		}
	}
	return max_err;
}


void SBAnimationBlend::createJointExclusionArray(const std::vector<SkJoint*>& orig_list)
{
	plot_excld_list.resize(0);
	for(unsigned int i=0; i<orig_list.size(); i++)
	{
		SkJoint* j = orig_list[i];
		SrString jname(j->jointName().c_str());
		if(jname.search("face")>=0) { plot_excld_list.push_back(j); continue; }
		if(jname.search("brow")>=0) { plot_excld_list.push_back(j); continue; }
		if(jname.search("eye")>=0)  { plot_excld_list.push_back(j); continue; }
		if(jname.search("nose")>=0) { plot_excld_list.push_back(j); continue; }
		if(jname.search("lid")>=0)  { plot_excld_list.push_back(j); continue; }
		if(jname.search("jaw")>=0)  { plot_excld_list.push_back(j); continue; }
		if(jname.search("tongue")>=0) { plot_excld_list.push_back(j); continue; }
		if(jname.search("lip")>=0)    { plot_excld_list.push_back(j); continue; }
		if(jname.search("cheek")>=0)  { plot_excld_list.push_back(j); continue; }

		if(jname.search("finger")>=0) { plot_excld_list.push_back(j); continue; }
		if(jname.search("thumb")>=0)  { plot_excld_list.push_back(j); continue; }
		if(jname.search("index")>=0)  { plot_excld_list.push_back(j); continue; }
		if(jname.search("middle")>=0) { plot_excld_list.push_back(j); continue; }
		if(jname.search("pinky")>=0)  { plot_excld_list.push_back(j); continue; }
		if(jname.search("ring")>=0)   { plot_excld_list.push_back(j); continue; }
	}
}

bool SBAnimationBlend::isExcluded(SkJoint* j)
{
	for(unsigned int i=0; i<plot_excld_list.size(); i++)
		if(plot_excld_list[i] == j)
			return true;
	return false;
}


void SBAnimationBlend::addCorrespondencePoints(const std::vector<std::string>& motionNames, const std::vector<double>& points)
{
	if (motions.size() == 0)
	{
		LOG("Add motions before add correspondence points for state");
		return;
	}
	if (motionNames.size() != motions.size())
	{
		LOG("Add correspondence points error, input motion number is not the same with that when adding motions");
		return;		
	}
	for (size_t i = 0; i < motionNames.size(); i++)
	{
		if (motionNames[i] != motions[i]->getName())
		{
			LOG("Add correspondence points error, input motion names are not in the same order with that when adding motions");
			return;
		}
	}
	if (motionNames.size() != points.size())
	{
		LOG("Add correspondence points error, input motion number is not the same with points number!");
		return;
	}	
	int num = motionNames.size();

	// find the right place to insert the keys
	int insertPosition = -1;
	if (keys.size() > 0)
	{
		for (size_t i = 0; i < keys[0].size(); i++)
		{
			if (points[0] <= keys[0][i])
			{
				insertPosition = i;
				break;
			}
		}
		if (insertPosition == -1)
		{
			insertPosition = keys[0].size();
		}
	}

	for (int i = 0; i < num; i++)
	{
		keys[i].insert(keys[i].begin() + insertPosition, points[i]);
	}

	validateCorrespondencePoints();
}

void SBAnimationBlend::setCorrespondencePoints(int motionIndex, int pointIndex, double value)
{
	if (motionIndex < 0 || pointIndex < 0 || (keys.size() == 0) || (pointIndex >= (int) keys[0].size()))
		return;

	keys[motionIndex][pointIndex] = value;
	validateCorrespondencePoints();
}

void SBAnimationBlend::removeCorrespondencePoints(int index)
{
	if (index < 0 || (keys.size() == 0) || (index >= (int) keys[0].size()))
		return;

	for (std::vector< std::vector<double> >::iterator iter = keys.begin();
		 iter != keys.end();
		 iter++)
	{
		std::vector<double>& keyArray = (*iter);
		keyArray.erase(keyArray.begin() + index);
	}	
}

int SBAnimationBlend::getNumMotions()
{
	return motions.size();
}

std::string SBAnimationBlend::getMotion(int num)
{
	if (motions.size() > (size_t) num && num >= 0)
	{
		return motions[num]->getName();
	}
	else
	{
		return "";
	}
}

int SBAnimationBlend::getNumCorrespondencePoints()
{
	return getNumKeys();
}

std::vector<double> SBAnimationBlend::getCorrespondencePoints(int num)
{
	if (keys.size() > (size_t) num && num >= 0)
	{
		return keys[num];
	}
	else
	{
		return std::vector<double>();
	}
}

std::string SBAnimationBlend::getDimension()
{
	return _dimension;
}

void SBAnimationBlend::removeMotion(const std::string& motionName)
{
	SmartBody::SBMotion* motion = SmartBody::SBScene::getScene()->getMotion(motionName);
	if (!motion)
	{
		LOG("No motion named %s found, cannot remove from state %s.", motionName.c_str(), this->stateName.c_str());
	}
}

bool SBAnimationBlend::addSkMotion(const std::string& motion)
{	
	SBMotion* sbmotion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(motion);

	if (!sbmotion)
	{
		LOG("SBAnimationBlend add sk motion failure, %s doesn't exist", motion.c_str());
		return false;
	}
	else
		return addMotionRef(sbmotion);	
}

bool SBAnimationBlend::removeSkMotion(const std::string& motionName)
{
	// find the index of the motion
	int index = -1;
	for (int i = 0; i < getNumMotions(); i++)
	{
		SkMotion* m = motions[i];
		if (m->getName() == motionName)
		{
			index = i;
			break;
		}
	}
	if (index < 0)
	{
		LOG("SBAnimationBlend delete motion failure, %s doesn't exist", motionName.c_str());
		return false;
	}

	// first delete corresponding time markers
	removeParameter(motionName);

	// delete the motion and correspondence point
	motions.erase(motions.begin() + index);
	keys.erase(keys.begin() + index);
	return true;
}

/*
	P.S. This is organized way, but is not a efficient way to do it
*/
void SBAnimationBlend::validateCorrespondencePoints()
{
	for (int i = 0; i < getNumMotions(); i++)
	{
		
		SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(motions[i]->getName());		
		for (int j = 1; j < getNumCorrespondencePoints(); j++)
		{
			if (keys[i][j] < keys[i][j - 1])
			{
				for (int k = j; k < getNumCorrespondencePoints(); k++)
					keys[i][k] += motion->duration();
			}
		}
	}
}

bool SBAnimationBlend::validateState()
{
	if (_isFinalized)
		return true;

	for (int i=0; i < getNumMotions(); i++)
	{
		
		SBMotion* motion = SmartBody::SBScene::getScene()->getAssetManager()->getMotion(motions[i]->getName());		
		if ((int)keys.size() < i) // no keys for this state
		{			
			keys.push_back(std::vector<double>());			
		}		
		std::vector<double>& keyVec = keys[i];
		if (keyVec.size() == 0) // if no keys for the motion, automatically set up this based on motion duration
		{
			keyVec.push_back(0.0);
			keyVec.push_back(motion->duration());
		}
	}
	_isFinalized = true;
	return true;
}

void SBAnimationBlend::addEvent(const std::string& motion, double time, const std::string& type, const std::string& parameters, bool onceOnly)
{
	SBMotionEvent* motionEvent = new SBMotionEvent();
	motionEvent->setIsOnceOnly(onceOnly);
	motionEvent->setTime(time);
	motionEvent->setType(type);
	motionEvent->setParameters(parameters);
	addEventToMotion(motion, motionEvent);
}

void SBAnimationBlend::removeEvent(int index)
{
	if (index < 0 || (int) _events.size() > index)
		return;
}

SBMotionEvent* SBAnimationBlend::getEvent(int index)
{
	if (index < 0 || (int) _events.size() > index)
		return NULL;

	return _events[index].first;
}

void SBAnimationBlend::removeAllEvents()
{
	for (std::vector<std::pair<SBMotionEvent*, int> >::iterator iter = _events.begin();
		 iter != _events.end();
		 iter++)
	{
		delete (*iter).first;
	}
	_events.clear();
}

int SBAnimationBlend::getNumEvents()
{
	return _events.size();
}

std::string SBAnimationBlend::saveToString()
{
	SmartBody::SBAnimationBlend* state = this;
	SmartBody::SBAnimationBlend0D* state0D = dynamic_cast<SmartBody::SBAnimationBlend0D*>(state);
	SmartBody::SBAnimationBlend1D* state1D = dynamic_cast<SmartBody::SBAnimationBlend1D*>(state);
	SmartBody::SBAnimationBlend2D* state2D = dynamic_cast<SmartBody::SBAnimationBlend2D*>(state);
	SmartBody::SBAnimationBlend3D* state3D = dynamic_cast<SmartBody::SBAnimationBlend3D*>(state);

	std::string stateNameVariable = state->stateName + "State";
	boost::replace_all(stateNameVariable,"@","");
	boost::replace_all(stateNameVariable,".","dot");
	std::stringstream strstr;
	strstr << "# blend " << stateName << "\n";
	strstr << "# autogenerated by SmartBody\n";
	strstr << "\n";
	strstr << "blendManager = scene.getBlendManager()\n";
	strstr << "\n";
	strstr << "# align motions first if needed\n";
#if 1
	for (int i = 0; i < state->getNumMotions(); i++)
	{
		SBMotion* motion = dynamic_cast<SBMotion*>(state->motions[i]);
		if (!motion)
			continue;
		int alignIndex = motion->getAlignIndex();
		if (alignIndex > 0)
		{
			strstr << "alignmotion = scene.getMotion(\"" << state->getMotionName(i) << "\")\n";
			strstr << "alignmotion.alignToBegin(" << alignIndex << ")\n";
		}
		else if (alignIndex < 0)
		{
			strstr << "alignmotion = scene.getMotion(\"" << state->getMotionName(i) << "\")\n";
			strstr << "alignmotion.alignToEnd(" << -alignIndex << ")\n";
		}
	}
#endif

	if (state0D || state1D || state2D || state3D)
	{
		strstr << "\n";
		// add the motions
		if (state0D)
		{
			strstr << stateNameVariable << " = blendManager.createBlend0D(\"" << stateName << "\")\n";		
		}
		if (state1D)
		{
			strstr << stateNameVariable << " = blendManager.createBlend1D(\"" << stateName << "\")\n";
		}
		else if (state2D)
		{
			strstr << stateNameVariable << " = blendManager.createBlend2D(\"" << stateName << "\")\n";
		}
		else if (state3D)
		{
			strstr << stateNameVariable << " = blendManager.createBlend3D(\"" << stateName << "\")\n";
		}

		strstr << "\n";
		strstr << stateNameVariable << ".setBlendSkeleton(\"" << state->getBlendSkeleton() << "\")\n";
		strstr << "motions = StringVec()\n";
		for (int x = 0; x < state->getNumMotions(); x++)
		{
			strstr << "motions.append(\"" << state->getMotion(x) << "\")\n";
		}
		// add the parameters
		strstr << "\n";
		if (state1D || state2D || state3D)
		{
			strstr << "paramsX = DoubleVec()\n";
		}
		if (state2D || state3D)
		{
			strstr << "paramsY = DoubleVec()\n";
		}
		if (state3D)
		{
			strstr << "paramsZ = DoubleVec()\n";
		}

		for (int x = 0; x < state->getNumMotions(); x++)
		{
			double p1, p2, p3;
			if (state1D)
			{
				state->getParameter(state->getMotion(x), p1);
				strstr << "paramsX.append(" << p1 << ") # " << state->getMotion(x) << " X\n";
			}
			else if (state2D)
			{
				state->getParameter(state->getMotion(x), p1, p2);
				strstr << "paramsX.append(" << p1 << ") # " << state->getMotion(x) << " X\n";
				strstr << "paramsY.append(" << p2 << ") # " << state->getMotion(x) << " Y\n";
			}
			else if (state3D)
			{
				state->getParameter(state->getMotion(x), p1, p2, p3);
				strstr << "paramsX.append(" << p1 << ") # " << state->getMotion(x) << " X\n";
				strstr << "paramsY.append(" << p2 << ") # " << state->getMotion(x) << " Y\n";
				strstr << "paramsZ.append(" << p3 << ") # " << state->getMotion(x) << " Z\n";
			}

		}
		strstr << "for i in range(0, len(motions)):\n";
		if (state0D)
		{
			strstr << "\t" << stateNameVariable << ".addMotion(motions[i])\n";
		}
		else if (state1D)
		{
			strstr << "\t" << stateNameVariable << ".addMotion(motions[i], paramsX[i])\n";
		}
		else if (state2D)
		{
			strstr << "\t" << stateNameVariable << ".addMotion(motions[i], paramsX[i], paramsY[i])\n";
		}
		else if (state3D)
		{
			strstr << "\t" << stateNameVariable << ".addMotion(motions[i], paramsX[i], paramsY[i], paramsZ[i])\n";
		}
		// add the correspondence points
		strstr << "\n";
		for (int c = 0; c < state->getNumKeys(); c++)
		{
			strstr << "points" << c << " = DoubleVec()\n";
			for (int m = 0; m < state->getNumMotions(); m++)
			{
				strstr << "points" << c << ".append(" << state->keys[m][c] << ") # " << state->getMotion(m) << " " << c << "\n";
			}
			strstr << stateNameVariable << ".addCorrespondencePoints(motions, points" << c << ")\n";
		}

	}
	if (state2D)
	{
		// create the triangles
		strstr << "\n";
		std::vector<TriangleInfo>& triangleInfo = state->getTriangles();
		for (size_t t = 0; t < triangleInfo.size(); t++)
		{
			strstr << stateNameVariable << ".addTriangle(\"" << triangleInfo[t].motion1 << "\", \"" <<  triangleInfo[t].motion2 << "\", \"" <<  triangleInfo[t].motion3 << "\")\n"; 
		}
	}
	if (state3D)
	{
		// create the tetrahedrons
		strstr << "\n";
		std::vector<TetrahedronInfo>& tetrahedronInfo = state->getTetrahedrons();
		for (size_t t = 0; t < tetrahedronInfo.size(); t++)
		{
			strstr << stateNameVariable << ".addTetrahedron(\"" << tetrahedronInfo[t].motion1 << "\", \"" <<  tetrahedronInfo[t].motion2 << "\", \"" <<  tetrahedronInfo[t].motion3 << "\", \"" <<  tetrahedronInfo[t].motion4 << "\")\n"; 
		}
	}
	return strstr.str();
}

SkMotion* SBAnimationBlend::getSkMotion( const std::string& motionName )
{
	for(int i=0; i<getNumMotions(); i++)
	{
		SkMotion* m = motions[i];
		if (m->getName() == motionName)
			return m;
	}
	// not found!
	LOG("Error: SBAnimationBlend::getSkMotion(): %s doesn't exist", motionName.c_str());
	return 0;
}

void SBAnimationBlend::addKeyTagValue( const std::string& motionName, int iType, const std::string& tagName, double value )
{
	int motionIdx = getMotionId(motionName);
	if (motionIdx < 0 || motionIdx >= (int)keyTagList.size()) return;
	KeyTagMap& tagMap = keyTagList[motionIdx];
	
	if (tagMap.find(iType) == tagMap.end())
	{
		tagMap[iType] = KeyTag();		
	}
	KeyTag& tags = tagMap[iType];
	if (tags.find(tagName) == tags.end())		
		tags[tagName] = std::vector<double>();		
	tags[tagName].push_back(value);	
}

KeyTagMap* SBAnimationBlend::getKeyTagMap( const std::string& motionName )
{
	int motionIdx = getMotionId(motionName);
	if (motionIdx < 0 || motionIdx >= (int)keyTagList.size()) return NULL;
	return &keyTagList[motionIdx];
}

MotionAnalysis* SBAnimationBlend::getMotionAnalysis()
{
	return motionAnalysis;
}

void SBAnimationBlend::buildMotionAnalysis( const std::string& skeletonName, const std::string& baseName, const std::vector<std::string>& motions, std::string motionPrefix )
{
	if (motionAnalysis)
		delete motionAnalysis;	

	motionAnalysis = new MotionAnalysis();
	motionAnalysis->init(skeletonName,baseName, this, motions, motionPrefix);	
}

void SBAnimationBlend::setBlendSkeleton( std::string skelName )
{
	SmartBody::SBAssetManager* assetManager = SmartBody::SBScene::getScene()->getAssetManager();
	SmartBody::SBSkeleton* skel = assetManager->getSkeleton(skelName);
	if (skel)
	{
		blendSkelName = skelName;
	}
}

std::string SBAnimationBlend::getBlendSkeleton()
{
	return blendSkelName;
}

SBAPI SrBox SBAnimationBlend::getParameterBoundBox()
{
	return SrBox(paraMin, paraMax);
}

SBAPI bool SBAnimationBlend::getWeightsFromParameters( SrVec& para, std::vector<float>& weights )
{
	std::vector<double> doubleWeights;
	if (getParameterDimension() == 1)
	{
		PABlend::getWeightsFromParameters(para[0],doubleWeights);
	}
	else if (getParameterDimension() == 2)
	{
		PABlend::getWeightsFromParameters(para[0],para[1],doubleWeights);
	}
	else if (getParameterDimension() == 3)
	{
		PABlend::getWeightsFromParameters(para[0],para[1],para[2],doubleWeights);
	}
	weights.resize(doubleWeights.size());
	for (unsigned int i=0;i<weights.size();i++)
		weights[i] = (float)doubleWeights[i];
	return true;
}

int SBAnimationBlend::getParameterDimension()
{
	return parameterDim;
}

bool SBAnimationBlend::addMotionRef( SBMotion* sbmotion)
{
	if (sbmotion)
	{
		std::string motionName = sbmotion->getName();
		motions.push_back(sbmotion);
		/*		
		// adding two correspondence points when the first motion got inserted
		if (motions.size() == 1)
		{
		std::vector<double> keyVec;
		keyVec.resize(2);
		keyVec[0] = 0.0f;
		keyVec[1] = skMotion->duration();
		keys.push_back(keyVec);
		}
		else
		*/
		{
			// add a zero-correspondence point for this new motion
			int numPoints = 0;
			if (keys.size() > 0)
				numPoints  = keys[0].size();
			std::vector<double> keyVec;
			if (numPoints > 0)
			{
				keyVec.resize(numPoints);
				double duration = sbmotion->duration();
				if (numPoints >= 2)
				{
					keyVec[0] = 0.0f;
					keyVec[numPoints - 1] = duration;
				}
				// uniformly space the correspondence points
				double step = duration / double(numPoints);
				for (int i = 1; i < numPoints - 1; i++)
				{
					keyVec[i] = double(i) * step;
				}
			}
			keys.push_back(keyVec);
			keyTagList.push_back(KeyTagMap());
		}

		getParameters().push_back(SrVec());
		return true;
	}
	else
	{			
		return false;
	}
}

SBAnimationBlend0D::SBAnimationBlend0D() : SBAnimationBlend("unknown")
{
}

SBAnimationBlend0D::SBAnimationBlend0D(const std::string& name) : SBAnimationBlend(name)
{
	_dimension = "0D";
	setType(0);
	parameterDim = 1;
}

SBAnimationBlend0D::~SBAnimationBlend0D()
{
}

void SBAnimationBlend0D::addMotion(const std::string& motion)
{
	addSkMotion(motion);	
	setParameter(motion, 1.f); // set a dummy parameter since there is only one motion
}


void SBAnimationBlend0D::removeMotion(const std::string& motion)
{
	SBAnimationBlend::removeMotion(motion);

	// remove correspondence points
	removeSkMotion(motion);
}

SBAnimationBlend1D::SBAnimationBlend1D() : SBAnimationBlend("unknown")
{
}


SBAnimationBlend1D::SBAnimationBlend1D(const std::string& name) : SBAnimationBlend(name)
{
	_dimension = "1D";
	setType(0);
	parameterDim = 1;
}

SBAnimationBlend1D::~SBAnimationBlend1D()
{
}

void SBAnimationBlend1D::addMotion(const std::string& motion, float parameter)
{
	addSkMotion(motion);

	setParameter(motion, parameter);
}

void SBAnimationBlend1D::addMotionFromRef( SmartBody::SBMotion* motion, float parameter )
{
	addMotionRef(motion);	
	setParameter(motion->getName(), parameter); // set a dummy parameter since there is only one motion
}


void SBAnimationBlend1D::removeMotion(const std::string& motionName)
{
	SBAnimationBlend::removeMotion(motionName);

	// remove correspondnce points
	removeSkMotion(motionName);
}

void SBAnimationBlend1D::setParameter(const std::string& motion, float parameter)
{
	PABlend::setParameter(motion, parameter);
}

SBAnimationBlend2D::SBAnimationBlend2D() : SBAnimationBlend("unknown")
{
}

SBAnimationBlend2D::SBAnimationBlend2D(const std::string& name) : SBAnimationBlend(name)
{
	_dimension = "2D";
	setType(1);
	parameterDim = 2;
}

SBAnimationBlend2D::~SBAnimationBlend2D()
{
}

void SBAnimationBlend2D::addMotion(const std::string& motion, float parameter1, float parameter2)
{
	addSkMotion(motion);
	PABlend::setParameter(motion, parameter1, parameter2);
}

void SBAnimationBlend2D::removeMotion(const std::string& motionName)
{
	SBAnimationBlend::removeMotion(motionName);

	// remove correspondence points
	removeSkMotion(motionName);

	// do something about triangle
	removeTriangles(motionName);
}

void SBAnimationBlend2D::setParameter(const std::string& motion, float parameter1, float parameter2)
{
	PABlend::setParameter(motion, parameter1, parameter2);
}

void SBAnimationBlend2D::addTriangle(const std::string& motion1, const std::string& motion2, const std::string& motion3)
{
	PABlend::addTriangle(motion1, motion2, motion3);
}

SBAnimationBlend3D::SBAnimationBlend3D() : SBAnimationBlend("unknown")
{
}


SBAnimationBlend3D::SBAnimationBlend3D(const std::string& name) : SBAnimationBlend(name)
{
	_dimension = "3D";
	setType(2);
	parameterDim = 3;
}

SBAnimationBlend3D::~SBAnimationBlend3D()
{
}


void SBAnimationBlend3D::addMotion(const std::string& motion, float parameter1, float parameter2, float parameter3)
{
	addSkMotion(motion);
	
	setParameter(motion, parameter1, parameter2, parameter3);
}

void SBAnimationBlend3D::removeMotion(const std::string& motionName)
{
	SBAnimationBlend::removeMotion(motionName);

	// remove correspondence points
	removeSkMotion(motionName);

	// do something about tetrahedrons
	removeTetrahedrons(motionName);
}

void SBAnimationBlend3D::setParameter(const std::string& motion, float parameter1, float parameter2, float parameter3)
{
	PABlend::setParameter(motion, parameter1, parameter2, parameter3);
}

void SBAnimationBlend3D::addTetrahedron(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4)
{
	PABlend::addTetrahedron(motion1, motion2, motion3, motion4);
}

}