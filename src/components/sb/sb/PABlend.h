/*
 *  me_ct_param_animation_data.h - part of Motion Engine and SmartBody-lib
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


#ifndef _PABLEND_H_
#define _PABLEND_H_

#include <sb/SBTypes.h>
#include <sr/sr_triangle.h>
#include <sr/sr_vec.h>
#include <string>
#include <vector>
#include <sb/SBMotion.h>
#include <sb/SBObject.h>

namespace SmartBody {
	class SBMotionEvent;
}


const std::string PseudoIdleState = "PseudoIdle";

struct TriangleInfo
{
	SrTriangle triangle;
	std::string motion1;
	std::string motion2;
	std::string motion3;
};

struct TetrahedronInfo
{
	SrVec v1;
	SrVec v2;
	SrVec v3;
	SrVec v4;
	std::string motion1;
	std::string motion2;
	std::string motion3;
	std::string motion4;
};

class PABlend : public SmartBody::SBObject
{
	public:
		SBAPI PABlend();
		SBAPI PABlend(PABlend* data);
		SBAPI PABlend(const std::string& name);
		SBAPI ~PABlend();
		
		SBAPI virtual bool getWeightsFromParameters(double x, std::vector<double>& weights);
		SBAPI virtual bool getWeightsFromParameters(double x, double y, std::vector<double>& weights);
		SBAPI virtual bool getWeightsFromParameters(double x, double y, double z, std::vector<double>& weights);
		SBAPI virtual void getParametersFromWeights(float& x, std::vector<double>& weights);
		SBAPI virtual void getParametersFromWeights(float& x, float& y, std::vector<double>& weights);
		SBAPI virtual void getParametersFromWeights(float& x, float& y, float& z, std::vector<double>& weights);
		SBAPI void setParameter(const std::string& motion, double x);
		SBAPI void setParameter(const std::string& motion, double x, double y);
		SBAPI void setParameter(const std::string& motion, double x, double y, double z);
		SBAPI void getParameter(const std::string& motion, double& x);
		SBAPI void getParameter(const std::string& motion, double& x, double& y);
		SBAPI void getParameter(const std::string& motion, double& x, double& y, double& z);
		SBAPI void removeParameter(const std::string& motion);
		SBAPI void addTriangle(const std::string& motion1, const std::string& motion2, const std::string& motion3);
		SBAPI int getTriangleIndex(const std::string& motion1, const std::string& motion2, const std::string& motion3);
		SBAPI void removeTriangle(const std::string& motion1, const std::string& motion2, const std::string& motion3);
		SBAPI void removeTriangles(const std::string& motion);
		SBAPI void addTetrahedron(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4);
		SBAPI void removeTetrahedron(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4);
		SBAPI void removeTetrahedrons(const std::string& motion);
		SBAPI int getTetrahedronIndex(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4);
		SBAPI void buildTetrahedron();
		SBAPI int getType();
		SBAPI void setType(int typ);
		SBAPI double getLocalTime(double motionTime, int motionIndex);
		SBAPI double getMotionTime(double localTime, int motionIndex);
		SBAPI double getMotionKey(const std::string& motionName, int iKey);

		SBAPI int getNumParameters();
		SBAPI int getMinVecX();
		SBAPI int getMinVecY();
		SBAPI int getMinVecZ();
		SBAPI int getMaxVecX();
		SBAPI int getMaxVecY();
		SBAPI int getMaxVecZ();
		SBAPI SrVec getVec(const std::string& motion);
		SBAPI SrVec getVec(int id);
		SBAPI const std::string& getMotionName(int id);
		SBAPI int getMotionId(const std::string& name);

		SBAPI int getNumTriangles();
		SBAPI SrTriangle& getTriangle(int id);
		SBAPI float getMinimumDist(SrVec& pt, SrVec& a, SrVec& b, SrVec& minimumPt);

		SBAPI std::vector<SrVec>& getParameters() {return parameters;}
		SBAPI std::vector<TriangleInfo>& getTriangles() {return triangles;}
		SBAPI std::vector<TetrahedronInfo> & getTetrahedrons() {return tetrahedrons;}

		SBAPI std::vector<std::pair<SmartBody::SBMotionEvent*, int> >& getEvents();
		SBAPI void addEventToMotion(const std::string& motion, SmartBody::SBMotionEvent* motionEvent);

		std::string stateName;
		std::vector<SmartBody::SBMotion*> motions;
		std::vector<std::vector<double> > keys;

		bool cycle;
		bool incrementWorldOffsetY;

		SBAPI virtual int getNumMotions();
		SBAPI virtual int getNumKeys();

	protected:
		void updateParameterScale();
		bool insideTriangle(SrVec& pt, SrVec& v1, SrVec& v2, SrVec& v3);
		void getWeight(SrVec& pt, SrVec& v1, SrVec& v2, SrVec& v3, double& w1, double& w2, double& w3);
		void getWeight(SrVec& pt, SrVec& v1, SrVec& v2, SrVec& v3, SrVec& v4, double& w1, double& w2, double& w3, double& w4);
		SrVec closestPtPointTriangle(SrVec& pt, SrVec& v1, SrVec& v2, SrVec& v3);
		int PointOutsideOfPlane(SrVec p, SrVec a, SrVec b, SrVec c);
		SrVec vecMultiply(SrVec& vec1, SrVec& vec2);

		int type;
		//std::vector<std::string> motionNames;
		std::vector<SrVec> pseudoParameters;
		std::vector<SrVec> parameters;
		SrVec parameterScale;
		SrVec invParameterScale;
		std::vector<TriangleInfo> triangles;
		std::vector<TetrahedronInfo> tetrahedrons;
		std::string emptyString;
		std::vector<std::pair<SmartBody::SBMotionEvent*, int> > _events;
		SrVec paraMax, paraMin;
};


class MotionParameters
{
	public:
		MotionParameters(SkMotion* m, SkSkeleton* skel, std::string jName = "");
		~MotionParameters();

		void setFrameId(int min, int max);
		void setFrameId(double min, double max);
		double getParameter(int type);

	private:
		double getAvgSpeed();
		double getAccSpeed();
		double getAvgAngularSpeed();
		double getAccAngularSpeed();
		double getTransitionX();
		double getTransitionY();
		double getTransitionZ();
		double getAvgRootJointY();

	private:
		int minFrameId;
		int maxFrameId;
		SkMotion* motion;
		SkSkeleton* skeleton;
		SkJoint* joint;
};

#endif
