#include "DataModelService.h"
#include "DataModel.h"
#include "DataObject.h"

namespace dime
{
void DataModelService::dump(PDataObject toDump, std::strstream & dumpDest, bool recursive)
{	
	//@todo advance functionality by adding dump level (to dump out types, description, etc.)
	dumpDest << toDump->getPath() << ": " << toDump->getStringVal() << std::endl;
	
	if (recursive)
	{
		DataObject::ChildList childs;
		toDump->getChildList(childs);

		for (DataObject::ChildList::iterator i = childs.begin(); i != childs.end(); i++)
		{
			dump(*i, dumpDest, true);
		}
	}
}
}