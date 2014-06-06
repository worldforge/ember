#include "SBAssetHandlerSkm.h"
#include <vhcl.h>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
#include <sb/SBMotion.h>
#include <sb/SBScene.h>

namespace SmartBody {

SBAssetHandlerSkm::SBAssetHandlerSkm()
{
	assetTypes.push_back("skm");
}

SBAssetHandlerSkm::~SBAssetHandlerSkm()
{
}

std::vector<SBAsset*> SBAssetHandlerSkm::getAssets(const std::string& path)
{
	std::vector<SBAsset*> assets;

	std::string convertedPath = checkPath(path);
	if (convertedPath == "")
		return assets;

	SmartBody::SBMotion* motion = new SmartBody::SBMotion();
	bool parseSuccessful = false;

	SrInput in( convertedPath.c_str(), "rt" );
	SrString fullin_string;
	in.getall( fullin_string );
	SrInput fullin( (const char *)fullin_string );
	fullin.filename( convertedPath.c_str() ); // copy filename for error message
	double scale = 1.0;
	if (SmartBody::SBScene::getScene()->getAttribute("globalMotionScale"))
		scale = SmartBody::SBScene::getScene()->getDoubleAttribute("globalMotionScale");
	parseSuccessful = motion->load( fullin, scale );
	if (parseSuccessful)
	{
		motion->filename(convertedPath.c_str());
		assets.push_back(motion);
	}
	else
		LOG("Could not read .skm file %s", convertedPath.c_str());

	return assets;
}

};
