#include "DataModelService.h"
#include "DataModel.h"
#include "DataObject.h"

namespace dime
{
void DataModelService::dump(PDataObject toDump, std::stringstream & dumpDest, bool recursive, 
								int level)
{	
	//@todo advance functionality by adding dump level (to dump out types, description, etc.)
	if (level == 0)
	{
		dumpDest << toDump->getPath() << ": " << toDump->getStringVal() << std::endl;
	}
	else
	{
		dumpDest << "[" << toDump->getPath();
		
		if (level > 1)
		{
			DataType type = toDump->getType();
			
			dumpDest << ":";
#define DUMPTYPE(t) if (type & t) dumpDest << " "  #t ;

			DUMPTYPE(VOID)
			DUMPTYPE(FOLDER)
			DUMPTYPE(BOOLEAN)
			DUMPTYPE(INTEGER)
			DUMPTYPE(FLOAT)
			DUMPTYPE(STRING)
			DUMPTYPE(CUSTOM_TYPE)
			
			if (level > 4)
			{
				DUMPTYPE(PRE_VALUE_CHANGE)
				DUMPTYPE(POST_VALUE_CHANGE)
				DUMPTYPE(POST_ADDITION)
				DUMPTYPE(PRE_DELETION)
                DUMPTYPE(PRE_LINK_CHANGE)
				DUMPTYPE(POST_LINK_CHANGE)
				DUMPTYPE(FIRE_ON_CHILD_EVENT)
				DUMPTYPE(HAS_CHILDS)
				DUMPTYPE(ADOPTS_CHILDS)
				DUMPTYPE(CAN_SET_VALUE)
				DUMPTYPE(CAN_ADD_CHILDS)
				DUMPTYPE(CAN_REMOVE)
				DUMPTYPE(IS_LINK)
			}

		}		
		dumpDest << "]" << std::endl;

		if ((level > 3) && (toDump->getType() & IS_LINK))
		{
			{
				if (!(toDump->getType() & VOID))
				{
					dumpDest << "-> " << toDump->getLinkDest()->getPath() << std::endl;
				}
				else
				{
					dumpDest << "-> INVALID LINK" << std::endl;
				}
			}
		}

		if (level > 2)
		{
			if (!toDump->getDescription().empty())
			{
				dumpDest << "# " << toDump->getDescription() << std::endl;
			}
		}
		

		dumpDest << toDump->getStringVal() << std::endl;
	}
	
	if (recursive)
	{
		DataObject::ChildList childs;
		toDump->getChildList(childs);

		for (DataObject::ChildList::iterator i = childs.begin(); i != childs.end(); i++)
		{
			dump(*i, dumpDest, true, level);
		}
	}
}
}
