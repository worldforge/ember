#ifndef _SBSCRIPT_H_
#define _SBSCRIPT_H_

#include <sb/SBTypes.h>
#include <sb/SBAsset.h>

namespace SmartBody {

class SBScript : public SBAsset
{
	public:
		SBAPI SBScript();
		SBAPI ~SBScript();

		SBAPI virtual void setEnable(bool val);
		SBAPI virtual bool isEnable();

		SBAPI virtual void notify(SBSubject* subject);

	protected:
		bool _enabled;

};

}

#endif