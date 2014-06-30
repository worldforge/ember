#ifndef _SBGESTUREMAP_H_
#define _SBGESTUREMAP_H_

#include <sb/SBTypes.h>
#include <string>
#include <vector>
#include <sb/SBObject.h>

namespace SmartBody {

class SBGestureMap : public SBObject 
{
	public:
		SBAPI struct GestureInfo
		{
			std::string _animation;
			std::string _lexeme;
			std::string _type;
			std::string _hand;
			std::string _style;
			std::string _posture;
		};

	public:
		SBAPI SBGestureMap();
		SBAPI SBGestureMap(const std::string& name);
		SBAPI ~SBGestureMap();

		SBAPI SBGestureMap* copy();

		SBAPI void addGestureMapping(const std::string& name, const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture);
		SBAPI std::string getGestureByInfo(const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture, const std::string& policy);
		SBAPI std::vector<std::string> getGestureListByInfo(const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture);
		SBAPI GestureInfo& getGestureByIndex(int i);
		SBAPI std::string getGestureAnimation(int i);
		SBAPI int getNumMappings();
		SBAPI void validate();

	private:
		bool gestureInfoCompare(const std::string& glexeme, const std::string& gtype, const std::string& ghand, const std::string& gstyle, const std::string& gposture,
								const std::string& lexeme, const std::string& type, const std::string& hand, const std::string& style, const std::string& posture, 
								std::string compType);

	protected:
		std::vector<GestureInfo> _gestureMaps;
		GestureInfo defaultGestureInfo;
};

}

#endif