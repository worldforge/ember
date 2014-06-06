#include "SBAssetHandlerSk.h"
#include <vhcl.h>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
#include <sb/SBMotion.h>
#include <sb/SBScene.h>
#include <sb/SBSkeleton.h>


namespace SmartBody {

SBAssetHandlerSk::SBAssetHandlerSk()
{
	assetTypes.push_back("sk");
}

SBAssetHandlerSk::~SBAssetHandlerSk()
{
}

std::vector<SBAsset*> SBAssetHandlerSk::getAssets(const std::string& path)
{
	std::vector<SBAsset*> assets;

	std::string convertedPath = checkPath(path);
	if (convertedPath == "")
		return assets;
	
	boost::filesystem::path p(convertedPath);
	std::string fileName = boost::filesystem::basename( p );
	std::string extension =  boost::filesystem::extension( p );
	FILE* myfile = fopen(convertedPath.c_str(), "rt");
	SrInput input(myfile);
	SmartBody::SBSkeleton* skeleton = new SmartBody::SBSkeleton();
	double scale = 1.0;
	if (SmartBody::SBScene::getScene()->getAttribute("globalSkeletonScale"))
		scale = SmartBody::SBScene::getScene()->getDoubleAttribute("globalSkeletonScale");
	if( skeleton->loadSk( input, scale) )
	{
		skeleton->ref();
		skeleton->setFileName(convertedPath);
		skeleton->setName(fileName + extension);
		assets.push_back(skeleton);
	}
	else
	{
		LOG("Could not load .sk file %s", convertedPath.c_str());
		delete skeleton;
	}

	return assets;
}

};
