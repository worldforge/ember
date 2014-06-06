#include "SBAsset.h"

namespace SmartBody {

SBAsset::SBAsset()
{
	_loaded = false;
	fullFilePath = "";
}

SBAsset::~SBAsset()
{
}


bool SBAsset::isLoaded()
{
	return _loaded;
}

void SBAsset::load()
{
}

void SBAsset::unload()
{
}

std::string SBAsset::getFullFilePath()
{
	return fullFilePath;
}

void SBAsset::setFullFilePath( std::string filename )
{
	fullFilePath = filename;
}
}

