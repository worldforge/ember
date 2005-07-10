/***************************************************************************
	OgrePagingLandScapeIntersectionSceneQuery.h  -  description
  -------------------
  begin                : Fri Aug 08 2003
  copyright            : (C) 2003-2005 by Jose A Milan && Tuan Kuranes
  email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#ifndef PAGINGLANDSCAPEINTERSECTIONSCENEQUERY_H
#define PAGINGLANDSCAPEINTERSECTIONSCENEQUERY_H

#include "OgrePagingLandScapePrerequisites.h"

namespace Ogre
{

/** PagingLandScape's specialisation of IntersectionSceneQuery. */
class PagingLandScapeIntersectionSceneQuery : public DefaultIntersectionSceneQuery
{
public:
	PagingLandScapeIntersectionSceneQuery( SceneManager* creator ) : DefaultIntersectionSceneQuery( creator )
	{ 
		mSupportedWorldFragments.insert( SceneQuery::WFT_RENDER_OPERATION );
	}

	virtual ~PagingLandScapeIntersectionSceneQuery( void )
	{
	}

	/** See IntersectionSceneQuery. */
	void execute( IntersectionSceneQueryListener* listener );

private:

};

} // namespace Ogre


#endif
