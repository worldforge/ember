#ifndef SB_H
#define SB_H

	#include <sb/SmartBody.h>

class SbManager
{
public:
	SbManager();
	~SbManager();

	void initialize(std::string const &mediaPath);
	SmartBody::SBScene* getScene(void);
	
	static SmartBody::SBScene* getSbScene(void);



private:

	SmartBody::SBScene *mScene;
};


#endif