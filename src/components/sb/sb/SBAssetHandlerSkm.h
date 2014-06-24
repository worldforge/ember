#ifndef _SBASSETHANDLERSKM_H_
#define _SBASSETHANDLERSKM_H_

#include "SBTypes.h"
#include <vector>
#include "SBAsset.h"
#include "SBAssetHandler.h"

namespace SmartBody {

class SBAssetHandlerSkm : public SBAssetHandler
{
	public:
		SBAPI SBAssetHandlerSkm();
		SBAPI virtual ~SBAssetHandlerSkm();

		SBAPI virtual std::vector<SBAsset*> getAssets(const std::string& path);

};


}

#endif