/*
 *  me_ct_param_animation_data.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Yuyu Xu, USC
 */

#include "PABlend.h"
#include "components/sb/controllers/me_ct_param_animation_utilities.h"
#include "components/sb/sr/sr_euler.h"
#include "components/sb/controllers/me_ct_ublas.hpp"
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include "SBEvent.h"
/*
#ifdef USE_TETGEN
#include <external/tetgen/tetgen.h>
#endif
*/
const double changeLimit = 20;

PABlend::PABlend() : SBObject()
{
	stateName = "unknown";
	parameterScale.x = 1.0f;
	parameterScale.y = 1.0f;
	parameterScale.z = 1.0f;
	incrementWorldOffsetY = false;
}

PABlend::PABlend(PABlend* data) : SBObject()
{
	stateName = data->stateName;
	for (unsigned int i = 0; i < data->motions.size(); i++)
	{
		motions.push_back(data->motions[i]);
	}

	for (unsigned int i = 0; i < data->keys.size(); i++)
	{
		std::vector<double> tempVec;
		for (unsigned int j = 0; j <data->keys[i].size(); j++)
			tempVec.push_back(data->keys[i][j]);
		keys.push_back(tempVec);
	}
	
	cycle = data->cycle;

	type = data->getType();

	incrementWorldOffsetY = data->incrementWorldOffsetY;
	
	for (unsigned int i = 0; i < data->getParameters().size(); i++)
		parameters.push_back(data->getParameters()[i]);

	for (unsigned int i = 0; i < data->getTriangles().size(); i++)
		triangles.push_back(data->getTriangles()[i]);
	for (unsigned int i = 0; i < data->getTetrahedrons().size(); i++)
		tetrahedrons.push_back(data->getTetrahedrons()[i]);

	
	parameterScale.x = 1.0f;
	parameterScale.y = 1.0f;
	parameterScale.z = 1.0f;
}


PABlend::PABlend(const std::string& name) : SBObject()
{
	setName(name);
	stateName = name;
	cycle = false;
	incrementWorldOffsetY = false;

	parameterScale.x = 1.0f;
	parameterScale.y = 1.0f;
	parameterScale.z = 1.0f;
}

PABlend::~PABlend()
{
	for (unsigned int i = 0; i < motions.size(); i++)
	{
		motions[i] = NULL;
	}
	motions.clear();
}


int PABlend::getNumMotions()
{
	return motions.size();
}

int PABlend::getNumKeys()
{
	if (getNumMotions() > 0)
		return keys[0].size();
	else
		return 0;
}

int PABlend::getMotionId(const std::string& motion)
{
	for (int i = 0; i < getNumMotions(); i++)
	{
		const std::string& mName = motions[i]->getName();
		if (motion == mName)
			return i;
	}
	LOG("PABlend::getMotionId Warning: cannot get motion with name %s", motion.c_str());
	return -1;
}

/*
PATransition::PATransition()
{
}

PATransition::PATransition(PATransition* data, PABlend* from, PABlend* to)
{
	this->fromState = from;
	this->toState = to;
	this->fromMotionName = data->fromMotionName;
	this->toMotionName = data->toMotionName;
	for (unsigned int i = 0; i < data->easeOutStart.size(); i++)
		this->easeOutStart.push_back(data->easeOutStart[i]);
	for (unsigned int i = 0; i < data->easeOutEnd.size(); i++)
		this->easeOutEnd.push_back(data->easeOutEnd[i]);
	this->easeInStart = data->easeInStart;
	this->easeInEnd = data->easeInEnd;
}

PATransition::~PATransition()
{
	fromState = NULL;
	toState = NULL;
}

int PATransition::getNumEaseOut()
{
	return easeOutStart.size();
}

*/

bool PABlend::getWeightsFromParameters(double x, std::vector<double>& weights)
{
	if (type != 0)
		return false;

	//double xDiff = fabs(previousParam.x - x);
	//if (xDiff > changeLimit)
	//	x = (previousParam.x + x) * 0.5;
	if (weights.size() != getNumMotions()) weights.resize(getNumMotions());
	double left = -9999.0;
	double right = 9999.0;
	std::string leftMotion = "";
	std::string rightMotion = "";
	int minIndex = -1;
	int maxIndex = -1;
	double minValue = 9999999;
	double maxValue = -999999;
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (parameters[i].x < minValue)
		{
			minValue = parameters[i].x;
			minIndex = i;
		}
		if (parameters[i].x > maxValue)
		{
			maxValue = parameters[i].x;
			maxIndex = i;
		}
		if (parameters[i].x <= x)
			if (parameters[i].x >= left)	
			{
				left = parameters[i].x;
				leftMotion = motions[i]->getName();
			}
		if (parameters[i].x >= x)
			if (parameters[i].x <= right) 
			{
				right = parameters[i].x;
				rightMotion =  motions[i]->getName();
			}
	}
	if (leftMotion == "")
	{
		if (minIndex != -1)
			leftMotion = motions[minIndex]->getName();
	}
	if (rightMotion == "")
	{
		if (minIndex != -1)
			rightMotion = motions[maxIndex]->getName();
	}
	for (int i = 0; i < getNumMotions(); i++)
		weights[i] = 0.0;
	if (right == left)
	{
		int id = getMotionId(leftMotion);
		weights[id] = 1.0;
	}
	else
	{
		double weight = (x - left) / (right - left);
		int leftId = getMotionId(leftMotion);
		int rightId = getMotionId(rightMotion);
		if (leftId >= 0 && rightId >= 0)
		{
			weights[leftId] = 1 - weight;
			weights[rightId] = weight;
		}
		if (leftId >=0 && rightId < 0)
		{
			weights[leftId] = 1.0;
		}
		if (rightId >=0 && leftId < 0)
		{
			weights[rightId] = 1.0;
		}
	}
	return true;
}

bool PABlend::getWeightsFromParameters(double x, double y, std::vector<double>& weights)
{
	if (type != 1)
		return false;
	
	if (weights.size() != getNumMotions()) weights.resize(getNumMotions());
	SrVec point = SrVec((float)x, (float)y, 0);
	for (int i = 0; i < getNumTriangles(); i++)
	{
		SrVec v1 = triangles[i].triangle.a;
		SrVec v2 = triangles[i].triangle.b;
		SrVec v3 = triangles[i].triangle.c;
		int id1 = getMotionId(triangles[i].motion1);
		int id2 = getMotionId(triangles[i].motion2);
		int id3 = getMotionId(triangles[i].motion3);
		bool in = insideTriangle(point, v1, v2, v3);
		if (in)
		{
			for (int i = 0; i < getNumMotions(); i++)
				weights[i] = 0.0;
			
			getWeight(point, v1, v2, v3, weights[id1], weights[id2], weights[id3]);
			return true;
		}
	}

	// find the nearest dist
	SrVec param;
	float minDist = 9999;
	int triangleId = -1;
	for (int i = 0; i < getNumTriangles(); i++)
	{
		SrVec v1 = triangles[i].triangle.a;
		SrVec v2 = triangles[i].triangle.b;
		SrVec v3 = triangles[i].triangle.c;
		SrVec pt = point;
		v1 = vecMultiply(v1, parameterScale);
		v2 = vecMultiply(v2, parameterScale);
		v3 = vecMultiply(v3, parameterScale);
		pt = vecMultiply(pt, parameterScale);

		SrVec vec;
		float dist = getMinimumDist(pt, v1, v2, vec);
		if (dist <= minDist)
		{
			minDist = dist;
			param = vec;
			triangleId = i;
		}
		dist = getMinimumDist(pt, v3, v2, vec);
		if (dist <= minDist)
		{
			minDist = dist;
			param = vec;
			triangleId = i;
		}
		dist = getMinimumDist(pt, v1, v3, vec);
		if (dist <= minDist)
		{
			minDist = dist;
			param = vec;
			triangleId = i;
		}
	}

	if (triangleId >= 0)
	{
		for (int i = 0; i < getNumMotions(); i++)
			weights[i] = 0.0;

		SrVec v1 = triangles[triangleId].triangle.a;
		SrVec v2 = triangles[triangleId].triangle.b;
		SrVec v3 = triangles[triangleId].triangle.c;
		int id1 = getMotionId(triangles[triangleId].motion1);
		int id2 = getMotionId(triangles[triangleId].motion2);
		int id3 = getMotionId(triangles[triangleId].motion3);

		SrVec invParameterScale(1.f / parameterScale.x, 1.f / parameterScale.y, 1.f / parameterScale.z);
		param = vecMultiply(param, invParameterScale);
		getWeight(param, v1, v2, v3, weights[id1], weights[id2], weights[id3]);
	}
	else
	{
		LOG("Not inside triangle.");
	}
	return true;
}

bool PABlend::getWeightsFromParameters(double x, double y, double z, std::vector<double>& weights)
{
	if (type != 2)
		return false;

	if (weights.size() != getNumMotions()) weights.resize(getNumMotions());
	SrVec point = SrVec((float)x, (float)y, (float)z);
	for (unsigned int i = 0; i < tetrahedrons.size(); i++)
	{
		SrVec v1 = tetrahedrons[i].v1;
		SrVec v2 = tetrahedrons[i].v2;
		SrVec v3 = tetrahedrons[i].v3;
		SrVec v4 = tetrahedrons[i].v4;
		int id1 = getMotionId(tetrahedrons[i].motion1);
		int id2 = getMotionId(tetrahedrons[i].motion2);
		int id3 = getMotionId(tetrahedrons[i].motion3);
		int id4 = getMotionId(tetrahedrons[i].motion4);

		if (id1 < 0 || id2 < 0 || id3 < 0 || id4 < 0)
			return false;
		double w1 = 0.0;	
		double w2 = 0.0;
		double w3 = 0.0;
		double w4 = 0.0;

		getWeight(point, v1, v2, v3, v4, w1, w2, w3, w4);

		if (w1 >= 0 && w2 >= 0 && w3 >= 0 && w4 >= 0)
		{
			for (int i = 0; i < getNumMotions(); i++)
				weights[i] = 0.0;

			weights[id1] = w1;
			weights[id2] = w2;
			weights[id3] = w3;
			weights[id4] = w4;
			//LOG("weight = %f %f %f %f",w1,w2,w3,w4);
			return true;
		} 
	}

	// if it's not inside any tetrahedron, find the most close one
	// refer to <Real-Time Collision Detection>
	int id = -1;
	float min = 99999;
	SrVec param;
	for (unsigned int i = 0; i < tetrahedrons.size(); i++)
	{
		float bestSqDist = 9999;
		SrVec a = tetrahedrons[i].v1;
		SrVec b = tetrahedrons[i].v2;
		SrVec c = tetrahedrons[i].v3;
		SrVec d = tetrahedrons[i].v4;
		SrVec pt = point;

		a = vecMultiply(a, parameterScale);
		b = vecMultiply(b, parameterScale);
		c = vecMultiply(c, parameterScale);
		d = vecMultiply(d, parameterScale);
		pt = vecMultiply(pt, parameterScale);

		SrVec closestPt = a;

		if (PointOutsideOfPlane(pt, a, b, c))
		{
			SrVec q = closestPtPointTriangle(pt, a, b, c);
			float sqDist = dot(q - pt, q - pt);
			if (sqDist < bestSqDist)
			{
				bestSqDist = sqDist;
				closestPt = q;
			}
		}
		if (PointOutsideOfPlane(pt, a, c, d))
		{
			SrVec q = closestPtPointTriangle(pt, a, c, d);
			float sqDist = dot(q - pt, q - pt);
			if (sqDist < bestSqDist)
			{
				bestSqDist = sqDist;
				closestPt = q;
			}
		}
		if (PointOutsideOfPlane(pt, a, d, b))
		{
			SrVec q = closestPtPointTriangle(pt, a, d, b);
			float sqDist = dot(q - pt, q - pt);
			if (sqDist < bestSqDist)
			{
				bestSqDist = sqDist;
				closestPt = q;
			}
		}
		if (PointOutsideOfPlane(pt, b, d, c))
		{
			SrVec q = closestPtPointTriangle(pt, b, d, c);
			float sqDist = dot(q - pt, q - pt);
			if (sqDist < bestSqDist)
			{
				bestSqDist = sqDist;
				closestPt = q;
			}
		}

		if (bestSqDist <= min)
		{
			param = closestPt;
			id = i;
			min = bestSqDist;
		}
	}

	if (id >= 0)
	{
		SrVec v1 = tetrahedrons[id].v1;
		SrVec v2 = tetrahedrons[id].v2;
		SrVec v3 = tetrahedrons[id].v3;
		SrVec v4 = tetrahedrons[id].v4;
		int id1 = getMotionId(tetrahedrons[id].motion1);
		int id2 = getMotionId(tetrahedrons[id].motion2);
		int id3 = getMotionId(tetrahedrons[id].motion3);
		int id4 = getMotionId(tetrahedrons[id].motion4);
		double w1 = 0.0;	
		double w2 = 0.0;
		double w3 = 0.0;
		double w4 = 0.0;

		// scale back
		param = vecMultiply(param, invParameterScale);
		getWeight(param, v1, v2, v3, v4, w1, w2, w3, w4);

		if (w1 >= 0 && w2 >= 0 && w3 >= 0 && w4 >= 0)
		{
			for (int i = 0; i < getNumMotions(); i++)
				weights[i] = 0.0;
			weights[id1] = w1;
			weights[id2] = w2;
			weights[id3] = w3;
			weights[id4] = w4;
			return true;
		}
		else
		{
			//LOG("Not inside tetrahedron.");
		}
	}
	return false;
}

void PABlend::getParametersFromWeights(float& x, std::vector<double>& weights)
{
	x = 0.0f;
	for (int i = 0; i < getNumParameters(); i++)
	{
		int id = getMotionId(motions[i]->getName());
		x += (float)weights[id] * parameters[i].x;
	}
}

void PABlend::getParametersFromWeights(float& x, float& y, std::vector<double>& weights)
{
	std::vector<int> indices;
	for (int i = 0; i < getNumMotions(); i++)
	{
		if (weights[i] > 0.0)
			indices.push_back(i);
	}
	if (indices.size() == 0)
		return;
	else if (indices.size() == 1)
	{
		int id = getMotionId(motions[indices[0]]->getName());
		if (id >= 0)
		{
			x = getVec(id).x;
			y = getVec(id).y;
		}
		else
			return;
	}
	else
	{
		std::vector<SrVec> vecs;
		for (size_t i = 0; i < indices.size(); i++)
		{
			int id = getMotionId(motions[indices[i]]->getName());
			if (id >= 0)
				vecs.push_back(getVec(id));
			else
				return;
		}
		SrVec vec;
		for (size_t i = 0; i < indices.size(); i++)
			vec = vec + vecs[i] * (float)weights[indices[i]];
		x = vec.x;
		y = vec.y;
	}
}

void PABlend::getParametersFromWeights(float& x, float& y, float& z, std::vector<double>& weights)
{
	std::vector<int> indices;
	for (int i = 0; i < getNumMotions(); i++)
	{
		if (weights[i] > 0.0)
			indices.push_back(i);
	}
	if (indices.size() == 0)
		return;
	else if (indices.size() == 1)
	{
		int id = getMotionId(motions[indices[0]]->getName());
		if (id >= 0)
		{
			x = getVec(id).x;
			y = getVec(id).y;
			z = getVec(id).z;
		}
		else
			return;
	}
	else
	{
		std::vector<SrVec> vecs;
		for (size_t i = 0; i < indices.size(); i++)
		{
			int id = getMotionId(motions[indices[i]]->getName());
			if (id >= 0)
				vecs.push_back(getVec(id));
			else
				return;
		}
		SrVec vec;
		for (size_t i = 0; i < indices.size(); i++)
			vec = vec + vecs[i] * (float)weights[indices[i]];
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
}

void PABlend::updateParameterScale()
{
	float xMin = 9999.0f;
	float xMax = -9999.0f;
	float yMin = 9999.0f;
	float yMax = -9999.0f;
	float zMin = 9999.0f;
	float zMax = -9999.0f;

	// find the min and max values
	for (size_t i = 0; i < parameters.size(); i++)
	{
		if (parameters[i].x < xMin)
			xMin = parameters[i].x;
		if (parameters[i].y < yMin)
			yMin = parameters[i].y;
		if (parameters[i].z < zMin)
			zMin = parameters[i].z;

		if (parameters[i].x > xMax)
			xMax = parameters[i].x;
		if (parameters[i].y > yMax)
			yMax = parameters[i].y;
		if (parameters[i].z > zMax)
			zMax = parameters[i].z;
	}

	paraMax = SrVec(xMax,yMax,zMax);
	paraMin = SrVec(xMin,yMin,zMin);
	float xSpan = xMax - xMin;
	float ySpan = yMax - yMin;
	float zSpan = zMax - zMin;
	if (xSpan > 0)
	{
		parameterScale.x = 1.f / xSpan;
	}
	if (ySpan > 0)
	{
		parameterScale.y = 1.f / ySpan;
	}
	if (zSpan > 0)
	{
		parameterScale.z = 1.f / zSpan;
	}

	if (parameterScale.x != 0.0f)
		invParameterScale.x = 1.f / parameterScale.x;
	if (parameterScale.y != 0.0f)
		invParameterScale.y = 1.f / parameterScale.y;
	if (parameterScale.z != 0.0f)
		invParameterScale.z = 1.f / parameterScale.z;
}

void PABlend::setParameter(const std::string& motion, double x)
{
	type = 0;
	SrVec vec;
	vec.x = (float)x;
	for (size_t i = 0; i <  motions.size(); i++)
	{
		if ( motions[i]->getName() == motion)
		{
			if (parameters.size() <= i)
				parameters.push_back(vec);
			else
			parameters[i] = vec;
			updateParameterScale();
			return;
		}
	} 
	parameters.push_back(vec);
	updateParameterScale();
}

void PABlend::setParameter(const std::string& motion, double x, double y)
{
	type = 1;
	SrVec vec;
	vec.x = (float)x;
	vec.y = (float)y;
	for (size_t i = 0; i < motions.size(); i++)
	{
		if (motions[i]->getName() == motion)
		{
			if (parameters.size() <= i)
				parameters.push_back(vec);
			else
				parameters[i] = vec;
			updateParameterScale();
			return;
		}
	}
	parameters.push_back(vec);
	updateParameterScale();
}

void PABlend::setParameter(const std::string& motion, double x, double y, double z)
{
	type = 2;
	SrVec vec;
	vec.x = (float)x;
	vec.y = (float)y;
	vec.z = (float)z;
	for (size_t i = 0; i  <motions.size(); i++)
	{
		if (motions[i]->getName() == motion)
		{
			if (parameters.size() <= i)
				parameters.push_back(vec);
			else
				parameters[i] = vec;
			updateParameterScale();
			return;
		}
	}

	parameters.push_back(vec);
	updateParameterScale();
}

void PABlend::getParameter(const std::string& motion, double& x)
{
	type = 0;
	SrVec vec;
	vec.x = (float)x;
	for (size_t i = 0; i <  motions.size(); i++)
	{
		if ( motions[i]->getName() == motion)
		{
			x = parameters[i].x;
			return;
		}
	}
}

void PABlend::getParameter(const std::string& motion, double& x, double& y)
{
	type = 1;
	SrVec vec;
	vec.x = (float)x;
	vec.y = (float)y;
	for (size_t i = 0; i < motions.size(); i++)
	{
		if (motions[i]->getName() == motion)
		{
			x = parameters[i].x;
			y = parameters[i].y;
			return;
		}
	}
}

void PABlend::getParameter(const std::string& motion, double& x, double& y, double& z)
{
	type = 2;
	SrVec vec;
	vec.x = (float)x;
	vec.y = (float)y;
	vec.z = (float)z;
	for (size_t i = 0; i  <motions.size(); i++)
	{
		if (motions[i]->getName() == motion)
		{
			x = parameters[i].x;
			y = parameters[i].y;
			z = parameters[i].z;
			return;
		}
	}
}

void PABlend::removeParameter(const std::string& motion)
{
	for (size_t i = 0; i < motions.size(); i++)
	{
		if (motions[i]->getName() == motion)
		{
			if (parameters.size() > i)
				parameters.erase(parameters.begin() + i);
			return;
		}
	}	
}


void PABlend::addTriangle(const std::string& motion1, const std::string& motion2, const std::string& motion3)
{
	if (getTriangleIndex(motion1, motion2, motion3) >= 0)
	{
		LOG("PABlend::addTriangle ERR: triangle with motion %s, %s, %s already exist!", motion1.c_str(), motion2.c_str(), motion3.c_str());
		return;
	}

	// add triangle
	TriangleInfo tInfo;
	SrVec v1 = getVec(motion1);
	SrVec v2 = getVec(motion2);
	SrVec v3 = getVec(motion3);
	tInfo.triangle = SrTriangle(v1, v2, v3);
	tInfo.motion1 = motion1;
	tInfo.motion2 = motion2;
	tInfo.motion3 = motion3;
	triangles.push_back(tInfo);
}

int PABlend::getTriangleIndex(const std::string& motion1, const std::string& motion2, const std::string& motion3)
{
	for (size_t i = 0; i < triangles.size(); i++)
	{
		std::vector<std::string> inputTriangle;
		inputTriangle.push_back(motion1);
		inputTriangle.push_back(motion2);
		inputTriangle.push_back(motion3);
		std::vector<std::string> triangle;
		triangle.push_back(triangles[i].motion1);
		triangle.push_back(triangles[i].motion2);
		triangle.push_back(triangles[i].motion3);
		std::sort(inputTriangle.begin(), inputTriangle.end());
		std::sort(triangle.begin(), triangle.end());

		bool isEqual = true;
		for (int j = 0; j < 3; j++)
		{
			if (inputTriangle[j] != triangle[j])
			{
				isEqual = false;
				break;
			}
		}
		if (isEqual)
		{
			return i;
		}
	}
	return -1;
}

void PABlend::removeTriangle(const std::string& motion1, const std::string& motion2, const std::string& motion3)
{
	int id = getTriangleIndex(motion1, motion2, motion3);
	if (id >= 0)
		triangles.erase(triangles.begin() + id);
}


void PABlend::removeTriangles(const std::string& motion)
{
	// what is a good way to reconstruct triangles?

	for (size_t i = 0; i < triangles.size(); i++)
	{
		TriangleInfo tInfo = triangles[i];
		if (tInfo.motion1 == motion ||
			tInfo.motion2 == motion ||
			tInfo.motion3 == motion)
		{
			triangles.erase(triangles.begin() + i);
			i--;
		}
	}
}

void PABlend::addTetrahedron(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4)
{
	if (getTetrahedronIndex(motion1, motion2, motion3, motion4) >= 0)
	{
		LOG("PABlend::addTetrahedron ERR: triangle with motion %s, %s, %s, %s already exist!", motion1.c_str(), motion2.c_str(), motion3.c_str(), motion4.c_str());
		return;
	}

	// add tetrahedron
	TetrahedronInfo tetraInfo;
	tetraInfo.v1 = getVec(motion1);
	tetraInfo.v2 = getVec(motion2);
	tetraInfo.v3 = getVec(motion3);
	tetraInfo.v4 = getVec(motion4);
	tetraInfo.motion1 = motion1;
	tetraInfo.motion2 = motion2;
	tetraInfo.motion3 = motion3;
	tetraInfo.motion4 = motion4;
	tetrahedrons.push_back(tetraInfo);
}

int PABlend::getTetrahedronIndex(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4)
{
	for (size_t i = 0; i < tetrahedrons.size(); i++)
	{
		std::vector<std::string> inputTetra;
		inputTetra.push_back(motion1);
		inputTetra.push_back(motion2);
		inputTetra.push_back(motion3);
		inputTetra.push_back(motion4);
		std::vector<std::string> tetra;
		tetra.push_back(tetrahedrons[i].motion1);
		tetra.push_back(tetrahedrons[i].motion2);
		tetra.push_back(tetrahedrons[i].motion3);
		tetra.push_back(tetrahedrons[i].motion4);
		std::sort(inputTetra.begin(), inputTetra.end());
		std::sort(tetra.begin(), tetra.end());

		bool isEqual = true;
		for (int j = 0; j < 4; j++)
		{
			if (inputTetra[j] != tetra[j])
			{
				isEqual = false;
				break;
			}
		}
		if (isEqual)
		{
			return i;
		}
	}
	return -1;
}

void PABlend::removeTetrahedron(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4)
{
	int id = getTetrahedronIndex(motion1, motion2, motion3, motion4);
	if (id >= 0)
		tetrahedrons.erase(tetrahedrons.begin() + id);
}

void PABlend::removeTetrahedrons(const std::string& motion)
{
	// what is a good way to reconstruct tetrahedrons?

	for (size_t i = 0; i < tetrahedrons.size(); i++)
	{
		TetrahedronInfo tetraInfo = tetrahedrons[i];
		if (tetraInfo.motion1 == motion ||
			tetraInfo.motion2 == motion ||
			tetraInfo.motion3 == motion ||
			tetraInfo.motion4 == motion)
		{
			tetrahedrons.erase(tetrahedrons.begin() + i);
			i--;
		}
	}	
}
/*
void PABlend::buildTetrahedron()
{
#if USE_TETGEN
	tetgenio ptIn, tetOut;
	// initialize input points
	ptIn.numberofpoints = parameters.size();
	ptIn.pointlist = new REAL[parameters.size() * 3];
	for (unsigned int i = 0; i < parameters.size(); i++)
	{
		ptIn.pointlist[i*3+0] = parameters[i].x;
		ptIn.pointlist[i*3+1] = parameters[i].y;
		ptIn.pointlist[i*3+2] = parameters[i].z;		
	}
	tetrahedralize((char*)"V",&ptIn,&tetOut);
//	std::cout << "Built Tetrahedron:" << std::endl;
	for (int i = 0; i < tetOut.numberoftetrahedra; i++)
	{
		std::string motions[4];
		for (int k = 0;k < tetOut.numberofcorners; k++)
		{
			int id = tetOut.tetrahedronlist[i * tetOut.numberofcorners + k];
			motions[k] = motionNames[id];
		}
//		std::cout << "[" << i << "]: " << motions[0] << ", " << motions[1] << ", " << motions[2] << ", " << motions[3] << std::endl; 
		addTetrahedron(motions[0], motions[1], motions[2], motions[3]);
	}
#endif
}
*/

int PABlend::getType()
{
	return type;
}

void PABlend::setType(int typ)
{
	type = typ;
}

int PABlend::getNumParameters()
{
	return parameters.size();
}

int PABlend::getMinVecX()
{
	float min = 9999;
	int ret = -1;
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (parameters[i].x < min)
		{
			min = parameters[i].x;
			ret = i;
		}
	}
	return ret;
}

int PABlend::getMinVecY()
{
	float min = 9999;
	int ret = -1;
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (parameters[i].y < min)
		{
			min = parameters[i].y;
			ret = i;
		}
	}
	return ret;
}

int PABlend::getMinVecZ()
{
	float min = 9999;
	int ret = -1;
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (parameters[i].z < min)
		{
			min = parameters[i].z;
			ret = i;
		}
	}
	return ret;
}


int PABlend::getMaxVecX()
{
	float max = -9999;
	int ret = -1;
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (parameters[i].x > max)
		{
			max = parameters[i].x;
			ret = i;
		}
	}
	return ret;
}

int PABlend::getMaxVecY()
{
	float max = -9999;
	int ret = -1;
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (parameters[i].y > max)
		{
			max = parameters[i].y;
			ret = i;
		}
	}
	return ret;
}

int PABlend::getMaxVecZ()
{
	float max = -9999;
	int ret = -1;
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (parameters[i].z > max)
		{
			max = parameters[i].z;
			ret = i;
		}
	}
	return ret;
}

SrVec PABlend::getVec(const std::string& motion)
{
	if (motions.size() < (size_t) getNumParameters())
		return SrVec();
	for (int i = 0; i < getNumParameters(); i++)
	{
		if (motions[i]->getName() == motion)
			return parameters[i];
	}
	return SrVec();
}

SrVec PABlend::getVec(int id)
{
	if (id < 0 || id > getNumParameters())
		return SrVec();
	return parameters[id];
}

const std::string& PABlend::getMotionName(int id)
{
	if (id < 0 || id > getNumParameters())
		return emptyString;

	return motions[id]->getName();	
}

int PABlend::getNumTriangles()
{
	return triangles.size();
}

SrTriangle& PABlend::getTriangle(int id)
{
	return triangles[id].triangle;
}

float PABlend::getMinimumDist(SrVec& c, SrVec& a, SrVec& b, SrVec& minimumPt)
{
	SrVec ab = b - a;
	SrVec ac = c - a;
	float f = dot(ab, ac);
	float d = dot(ab, ab);
	if (f < 0)
	{
		minimumPt = a;
		return dist(c, a);
	}
	if (f > d)
	{
		minimumPt = b;
		return dist(c, b);
	}
	f = f / d;
	minimumPt = a + f * ab;
	return dist(c, minimumPt);
}

bool PABlend::insideTriangle(SrVec& pt, SrVec& v1, SrVec& v2, SrVec& v3)
{
	SrVec ptToV1 = v1 - pt;
	SrVec ptToV2 = v2 - pt;
	SrVec ptToV3 = v3 - pt;
	SrVec cross1 = cross(ptToV1, ptToV2);
	SrVec cross2 = cross(ptToV2, ptToV3);
	SrVec cross3 = cross(ptToV3, ptToV1);
	float dot1 = dot(cross1, cross2);
	float dot2 = dot(cross2, cross3);
	float dot3 = dot(cross3, cross1);
	if (dot1 >= 0 && dot2 >= 0 && dot3 >= 0)
		return true;
	else
		return false;
}

void PABlend::getWeight(SrVec& pt, SrVec& v1, SrVec& v2, SrVec& v3, double& w1, double& w2, double& w3)
{
	SrVec ptToV1 = v1 - pt;
	SrVec ptToV2 = v2 - pt;
	SrVec ptToV3 = v3 - pt;
	SrVec cross1 = cross(ptToV1, ptToV2);
	double area1 = cross1.len();
	SrVec cross2 = cross(ptToV2, ptToV3);
	double area2 = cross2.len();
	SrVec cross3 = cross(ptToV3, ptToV1);
	double area3 = cross3.len();
	double totalArea = area1 + area2 + area3;
	if (totalArea == 0)
	{
		w1 = 1.0;
		w2 = 0.0;
		w3 = 0.0;
		LOG("ParameterManager::getWeight Warning: parameters not set correctly, check the initalization.");
	}
	else
	{
		w1 = area2 / totalArea;
		w2 = area3 / totalArea;
		w3 = area1 / totalArea;
	}
}

void PABlend::getWeight(SrVec& pt, SrVec& v1, SrVec& v2, SrVec& v3, SrVec& v4, double& w1, double& w2, double& w3, double& w4)
{
	dMatrix mat(3, 3);
	mat(0, 0) = v1.x - v4.x;
	mat(0, 1) = v2.x - v4.x;
	mat(0, 2) = v3.x - v4.x;
	mat(1, 0) = v1.y - v4.y;
	mat(1, 1) = v2.y - v4.y;
	mat(1, 2) = v3.y - v4.y;
	mat(2, 0) = v1.z - v4.z;
	mat(2, 1) = v2.z - v4.z;
	mat(2, 2) = v3.z - v4.z;
	dMatrix invMat(3, 3);
	MeCtUBLAS::inverseMatrix(mat,invMat);
	dVector vecIn(3);
	dVector vecOut(3);
	vecIn(0) = pt.x - v4.x;
	vecIn(1) = pt.y - v4.y;
	vecIn(2) = pt.z - v4.z;
	MeCtUBLAS::matrixVecMult(invMat, vecIn, vecOut);
	w1 = vecOut(0);
	w2 = vecOut(1);
	w3 = vecOut(2);
	w4 = 1 - vecOut(0) - vecOut(1) - vecOut(2);
	if (fabs(w1) <= 0.00001)	
		w1 = 0.0;
	if (fabs(w2) <= 0.00001)	
		w2 = 0.0;
	if (fabs(w3) <= 0.00001)	
		w3 = 0.0;
	if (fabs(w4) <= 0.00001)	
		w4 = 0.0;
}


SrVec PABlend::closestPtPointTriangle(SrVec& p, SrVec& a, SrVec& b, SrVec& c)
{
	SrVec ab = b - a;
	SrVec ac = c - a;
	SrVec ap = p - a;
	float d1 = dot(ab, ap);
	float d2 = dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) return a;
	
	SrVec bp = p - b;
	float d3 = dot(ab, bp);
	float d4 = dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3) return b;

	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return a + v * ab;
	}

	SrVec cp = p - c;
	float d5 = dot(ab, cp);
	float d6 = dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6) return c;

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return a + w * ac;
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b);
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return a + ab * v + ac * w;
}

int PABlend::PointOutsideOfPlane(SrVec p, SrVec a, SrVec b, SrVec c)
{
	return dot(p - a, cross(b - a, c - a)) >= 0.0f;
}

SrVec PABlend::vecMultiply(SrVec& vec1, SrVec& vec2)
{
	SrVec retVec;
	retVec.x = vec1.x * vec2.x;
	retVec.y = vec1.y * vec2.y;
	retVec.z = vec1.z * vec2.z;
	return retVec;
}

double PABlend::getLocalTime(double motionTime, int motionIndex)
{
	if (keys.size() <= (size_t) motionIndex)
	{
		LOG("Only %d motions for state %s, cannot find local time at index %d.", getNumMotions(), stateName.c_str(), motionIndex);
		return motionTime;
	}
	if (keys[motionIndex].size() == 0)
	{
		return motionTime;
	}
	double localTime = motionTime;
	if (localTime < keys[motionIndex][0])
		localTime = motions[motionIndex]->duration() + localTime;

	return localTime;
}

double PABlend::getMotionTime(double localTime, int motionIndex)
{
	if (keys.size() <= (size_t) motionIndex)
	{
		LOG("Only %d motions for state %s, cannot find local time at index %d.", getNumMotions(), stateName.c_str(), motionIndex);
		return localTime;
	}
	if (keys[motionIndex].size() == 0)
	{
		return localTime;
	}	
	double motionTime = localTime;

	double d = motions[motionIndex]->duration();
	int times = (int) (motionTime / d);
	if (times > 0)
	{
		motionTime = motionTime - float(times) * d;
	}
	return motionTime;
}

const bool ascendingTime2(std::pair<SmartBody::SBMotionEvent*, int>* a, std::pair<SmartBody::SBMotionEvent*, int>* b)
{
	return (a->first->getTime() < b->first->getTime());
}

std::vector<std::pair<SmartBody::SBMotionEvent*, int> >& PABlend::getEvents()
{
	return _events;
}

void PABlend::addEventToMotion(const std::string& motion, SmartBody::SBMotionEvent* motionEvent)
{
	// determine the motion index
	int index = getMotionId(motion);
	if (index == -1)
	{
		LOG("Could not add event to state %s: no motion named %s found.", stateName.c_str(), motion.c_str());
	}

	_events.push_back(std::pair<SmartBody::SBMotionEvent*, int>(motionEvent, index));
	// make sure that the motion events are ordered by time
//	std::sort(_events.begin(), _events.end(), ascendingTime2);
}

double PABlend::getMotionKey( const std::string& motionName, int iKey )
{
	int index = getMotionId(motionName);
	return keys[index][iKey];
}


MotionParameters::MotionParameters(SkMotion* m, SkSkeleton* skel, std::string j)
{
	motion = m;
	skeleton = new SkSkeleton(skel);
	skeleton->ref();
	motion->connect(skeleton);
	if (j == "")
	{
		joint = skeleton->search_joint(motion->channels().name(0).c_str());
	}
	else
		joint = skeleton->search_joint(j.c_str());
	minFrameId = 0;
	maxFrameId = motion->frames();
}

MotionParameters::~MotionParameters()
{
	motion->disconnect();
	if (skeleton)
		delete skeleton;
		//skeleton->unref();
}


void MotionParameters::setFrameId(int min, int max)
{
	minFrameId = min;
	maxFrameId = max;
}

void MotionParameters::setFrameId(double min, double max)
{
	double dt = motion->duration() / double(motion->frames() - 1);
	minFrameId = int(min / dt);
	maxFrameId = int(max / dt);
}

// TODO: Need Normalize
double MotionParameters::getParameter(int type)
{
	if (type == 0)
		return getAvgSpeed();
	if (type == 1)
		return getAccSpeed();
	if (type == 2)
		return getAvgAngularSpeed()*(180.0/ (M_PI)); // return radian ?
	if (type == 3)
		return getAccAngularSpeed()*(180.0/ (M_PI));
	if (type == 4)
		return getTransitionX();
	if (type == 5)
		return getTransitionY();
	if (type == 6)
		return getTransitionZ();
	if (type == 7)
		return getAvgRootJointY();
	return -1.0;
}

double MotionParameters::getAvgSpeed()
{
	motion->apply_frame(minFrameId);
	skeleton->update_global_matrices();
	const SrMat& srcMat = joint->gmat();
	SrVec srcPt = SrVec(srcMat.get(12), srcMat.get(13), srcMat.get(14));
	motion->apply_frame(maxFrameId);
	skeleton->update_global_matrices();
	const SrMat& destMat = joint->gmat();
	SrVec destPt = SrVec(destMat.get(12), destMat.get(13), destMat.get(14));
	float distance = dist(srcPt, destPt);
	double avgSpd = double(distance / motion->duration());
	return avgSpd;
}

double MotionParameters::getAccSpeed()
{
	float distance = 0;
	for (int i = minFrameId; i < maxFrameId - 1; i++)
	{
		motion->apply_frame(i);
		skeleton->update_global_matrices();
		const SrMat& srcMat = joint->gmat();
		SrVec srcPt = SrVec(srcMat.get(12), srcMat.get(13), srcMat.get(14));
		motion->apply_frame(i + 1);
		skeleton->update_global_matrices();
		const SrMat& destMat = joint->gmat();
		SrVec destPt = SrVec(destMat.get(12), destMat.get(13), destMat.get(14));
		distance += dist(srcPt, destPt);
	}
	double accSpd = double(distance / motion->duration());
	return accSpd;
}

double MotionParameters::getAvgAngularSpeed()
{
	motion->apply_frame(minFrameId);
	skeleton->update_global_matrices();
	const SrMat& srcMat = joint->gmat();
	float rx, ry, rz;
	sr_euler_angles(rotType, srcMat, rx, ry, rz);
	float srcRotY = ry;
	motion->apply_frame(maxFrameId);
	skeleton->update_global_matrices();
	const SrMat& destMat = joint->gmat();
	sr_euler_angles(rotType, destMat, rx, ry, rz);
	float destRotY = ry;
	float diffRotY = destRotY - srcRotY;
	double avgAngularSpd = double(diffRotY / motion->duration());
	return avgAngularSpd;
}

double MotionParameters::getAccAngularSpeed()
{
	float diffRotY = 0.0;
	for (int i = minFrameId; i < maxFrameId - 1; i++)
	{
		motion->apply_frame(i);
		skeleton->update_global_matrices();
		const SrMat& srcMat = joint->gmat();
		float rx, ry, rz;
		sr_euler_angles(rotType, srcMat, rx, ry, rz);
		float srcRotY = ry;
		motion->apply_frame(i + 1);
		skeleton->update_global_matrices();
		const SrMat& destMat = joint->gmat();
		sr_euler_angles(rotType, destMat, rx, ry, rz);
		float destRotY = ry;
		float diff;
		if (destRotY * srcRotY < 0 && fabs(destRotY) > 1.0f)
			diff = - destRotY - srcRotY;
		else
			diff = destRotY - srcRotY;
		diffRotY += diff;
	}
	double accAngularSpd = double(diffRotY / motion->duration());
	return accAngularSpd;
}

double MotionParameters::getTransitionX()
{
	motion->apply_frame(minFrameId);
	skeleton->update_global_matrices();
	const SrMat& srcMat = joint->gmat();
	const SrMat& srcMat0 = joint->gmatZero();
	SrVec srcPnt = SrVec(srcMat.get(12), srcMat.get(13), srcMat.get(14));
	float rx, ry, rz;
	float rx0, ry0, rz0;
	sr_euler_angles(rotType, srcMat, rx, ry, rz);
	sr_euler_angles(rotType, srcMat0, rx0, ry0, rz0);
	motion->apply_frame(maxFrameId);
	skeleton->update_global_matrices();
	const SrMat& destMat = joint->gmat();
	SrVec destPnt = SrVec(destMat.get(12), destMat.get(13), destMat.get(14));
	SrVec transitionVec = destPnt - srcPnt;
	SrVec heading = SrVec(sin(ry - ry0 - 1.57f), 0, cos(ry - ry0 - 1.57f));
	double x = dot(transitionVec, heading);
	return x;
}

double MotionParameters::getTransitionY()
{
	motion->apply_frame(minFrameId);
	skeleton->update_global_matrices();
	const SrMat& srcMat = joint->gmat();
	const SrMat& srcMat0 = joint->gmatZero();
	SrVec srcPnt = SrVec(srcMat.get(12), srcMat.get(13), srcMat.get(14));
	float rx, ry, rz;
	float rx0, ry0, rz0;
	sr_euler_angles(rotType, srcMat, rx, ry, rz);
	sr_euler_angles(rotType, srcMat0, rx0, ry0, rz0);
	motion->apply_frame(maxFrameId);
	skeleton->update_global_matrices();
	const SrMat& destMat = joint->gmat();
	SrVec destPnt = SrVec(destMat.get(12), destMat.get(13), destMat.get(14));
	SrVec transitionVec = destPnt - srcPnt;
	SrVec heading = SrVec(sin(ry - ry0), 0, cos(ry - ry0));
	double y = dot(transitionVec, heading);
	return y;
}

double MotionParameters::getTransitionZ()
{
	motion->apply_frame(minFrameId);
	skeleton->update_global_matrices();
	const SrMat& srcMat = joint->gmat();
	motion->apply_frame(maxFrameId);
	skeleton->update_global_matrices();
	const SrMat& destMat = joint->gmat();
	double z = destMat.get(14) - srcMat.get(14);
	return z;
}

double MotionParameters::getAvgRootJointY()
{
	int numFrames = maxFrameId - minFrameId;
	if (numFrames == 0)
		return 0;
	double y = 0.0;
	for (int i = 0; i < numFrames; i++)
	{
		motion->apply_frame(minFrameId + i);
		skeleton->update_global_matrices();
		const SrMat& mat = joint->gmat();
		y += mat.get(13);
	}
	y = y / (double)numFrames;
	return y;
}


