#ifndef _SBASSETHANDLER_H_
#define _SBASSETHANDLER_H_

#include <sb/SBTypes.h>
#include <sb/SBAsset.h>
#include <vector>
#include <string>

namespace SmartBody {

class SBAssetHandler
{
	public:
		SBAPI SBAssetHandler();
		SBAPI virtual ~SBAssetHandler();

		SBAPI virtual std::vector<std::string> getAssetTypes();
		SBAPI virtual std::vector<SBAsset*> getAssets(const std::string& path);

	protected:
		std::string checkPath(const std::string& path);

	protected:
		std::vector<std::string> assetTypes;
};


}

#endif