#include "sr_path_list.h"
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/version.hpp>
#include <boost/algorithm/string.hpp>
#include <sb/SBScene.h>

srPathList::srPathList()
{
	_curIndex = 0;
}

srPathList::~srPathList()
{
}
		
bool srPathList::insert(std::string path)
{
	for (size_t x = 0; x < _paths.size(); x++)
	{
		if (_paths[x] == path)
			return false;
	}

	_paths.push_back(path);
	return true;	
}

bool srPathList::remove(std::string path)
{
	for (std::vector<std::string>::iterator iter = _paths.begin();
		iter != _paths.end();
		iter++)
	{
		if ((*iter) == path)
		{
			_paths.erase(iter);
			return true;
		}
	}

	return false;	
}
		
void srPathList::removeAll()
{
	_paths.clear();
}
	

void srPathList::reset()
{	
	_curIndex = 0;
}
		
std::string srPathList::next_path(bool withPrefix)
{
	if (size_t(_curIndex) >= _paths.size())
		return "";

	std::stringstream strstr;
	// if the path is an absolute path, don't prepend the media path
	const boost::filesystem::path p = _paths[_curIndex];
	const std::string& pathPrefix = SmartBody::SBScene::getScene()->getMediaPath();
	if (withPrefix && (pathPrefix.size() > 0 && !p.has_root_path()))
	{
		boost::filesystem::path mediapath(pathPrefix);
		mediapath /= p;
		strstr << mediapath.string();
	}
	else
	{
		strstr << p.string();
	}

	boost::filesystem::path finalPath(strstr.str());
#if (BOOST_VERSION > 104400)
	finalPath.normalize();
#else
	finalPath.canonize();
#endif

#ifdef WIN32
	std::string convertedPath = finalPath.string();
	boost::replace_all(convertedPath, "\\", "/");
	_curIndex++;
	return convertedPath;
#else
	_curIndex++;
	return finalPath.string();
#endif

}

std::string srPathList::next_filename(char *buffer, const char *name)
{
	std::string path = next_path();
	if( path.size() > 0 )	{
		std::stringstream strstr;
		strstr << path << "/" << name;
		return( strstr.str() );
	}
	return( "" );
}
