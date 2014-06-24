#ifndef _SBASSET_H_
#define _SBASSET_H_

#include "SBTypes.h"
#include "SBObject.h"

namespace SmartBody {

class SBAsset : public SBObject
{
	public:
		SBAPI SBAsset();
		SBAPI ~SBAsset();

		SBAPI bool isLoaded();
		SBAPI void load();
		SBAPI void unload();
		SBAPI std::string getFullFilePath();
		SBAPI void setFullFilePath(std::string filename);

	protected:
		bool _loaded;
		std::string fullFilePath;		
};

}

#endif