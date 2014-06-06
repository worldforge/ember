#ifndef _SBMOTIONBLENDBASE_H
#define _SBMOTIONBLENDBASE_H

#include <sb/SBTypes.h>
#include <sb/SBAnimationState.h>
class MeCtBlendEngine;

namespace SmartBody {

	class SBMotionBlendBase : public SBAnimationBlend
	{
		public:		
			SBAPI SBMotionBlendBase();
			SBAPI SBMotionBlendBase(const std::string& name, const std::string& skelName, int dimension);
			SBAPI ~SBMotionBlendBase();			

			
			SBAPI const std::string& getSkeleton();
			SBAPI const std::string& getInterpType();
			SBAPI const std::string& getBlendType();

			SBAPI virtual void addMotion(const std::string& motion, std::vector<double>& parameter);
			SBAPI virtual void setMotionParameter(const std::string& motion, std::vector<double>& parameter);
			SBAPI virtual std::vector<double> getMotionParameter(const std::string& motion);

			SBAPI virtual void removeMotion(const std::string& motionName);
			SBAPI void buildBlendBase(const std::string& motionParameter, const std::string& interpolatorType, bool copySimplex);


			SBAPI virtual bool getWeightsFromParameters(double x, std::vector<double>& weights);
			SBAPI virtual bool getWeightsFromParameters(double x, double y, std::vector<double>& weights);
			SBAPI virtual bool getWeightsFromParameters(double x, double y, double z, std::vector<double>& weights);
			SBAPI virtual void getParametersFromWeights(float& x, std::vector<double>& weights);
			SBAPI virtual void getParametersFromWeights(float& x, float& y, std::vector<double>& weights);
			SBAPI virtual void getParametersFromWeights(float& x, float& y, float& z, std::vector<double>& weights);
			SBAPI void addTetrahedron(const std::string& motion1, const std::string& motion2, const std::string& motion3, const std::string& motion4);

			SBAPI virtual std::string saveToString();

		protected:
			MeCtBlendEngine* blendEngine;						
			std::string interpType;		
			std::string skeletonName;
			std::string blendType;

	};
}
#endif // namespace