#ifndef _SBASSETHANDLERSK_H_
#define _SBASSETHANDLERSK_H_

#include "SBTypes.h"
#include <vector>
#include "SBAsset.h"
#include "SBAssetHandler.h"


namespace SmartBody {

class SBAssetHandlerSk : public SBAssetHandler
{
	public:
		SBAPI SBAssetHandlerSk();
		SBAPI virtual ~SBAssetHandlerSk();

		SBAPI virtual std::vector<SBAsset*> getAssets(const std::string& path);

};


}

#endif