#include "SBAssetHandler.h"
#include <vhcl.h>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>

namespace SmartBody {

SBAssetHandler::SBAssetHandler()
{
}

SBAssetHandler::~SBAssetHandler()
{
}

std::vector<std::string> SBAssetHandler::getAssetTypes()
{
	return assetTypes;
}

std::vector<SBAsset*> SBAssetHandler::getAssets(const std::string& path)
{
	return std::vector<SBAsset*>();
}

std::string SBAssetHandler::checkPath(const std::string& path)
{
	boost::filesystem::path pathname(path);
	if( !boost::filesystem::exists( pathname ) )
	{
#if (BOOST_VERSION > 104400)
		LOG("Asset path \"%s\" not found.",  pathname.string().c_str());
#else
		LOG("Asset path \"%s\" not found.", pathname.native_file_string().c_str());
#endif
		return "";
	}

	if( boost::filesystem::is_directory( pathname ) ) // path indicates a directory
	{
		#if (BOOST_VERSION > 104400)
		LOG("Asset path \"%s\" is a directory.",  pathname.string().c_str());
#else
		LOG("Asset path \"%s\" is a directory.", pathname.native_file_string().c_str());
#endif
		return "";
	}

	std::string convertedPath = pathname.string();
#ifdef WIN32
	boost::replace_all(convertedPath, "\\", "/");
#endif

	return convertedPath;
}

};

