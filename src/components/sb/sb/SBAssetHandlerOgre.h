#ifndef _SBASSETHANDLERSOGRE_H_
#define _SBASSETHANDLEROGRE_H_

#include <sb/SBTypes.h>
#include <vector>
#include <sb/SBAsset.h>
#include <sb/SBAssetHandler.h>


namespace SmartBody {

class SBAssetHandlerOgre : public SBAssetHandler
{
	public:
		SBAPI SBAssetHandlerOgre();
		SBAPI virtual ~SBAssetHandlerOgre();

		SBAPI virtual std::vector<SBAsset*> getAssets(const std::string& path);

};


}

#endif