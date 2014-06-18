#ifndef SB_H
#define SB_H

	#include <sb/SmartBody.h>

	#define MOVE_NONE std::string("")
	#define MOVE_IDLE std::string("ChrUtah_Idle001")
	#define MOVE_RUN std::string("ChrUtah_Run001")
	#define MOVE_WALK std::string("ChrUtah_Walk001")
	#define MOVE_TURN std::string("ChrUtah_Turn90Lf01")

	#define MOVE MOVE_NONE

class SbManager
{
public:
	SbManager();
	~SbManager();

	void initialize(std::string const &mediaPath);
	SmartBody::SBScene* getScene(void);
	
	static SmartBody::SBScene* getSbScene(void);
	void startSimulation();
	void updateSimulation();



private:

	SmartBody::SBScene *mScene;
	SmartBody::SBSimulationManager *mSimulation;
	SmartBody::SBBmlProcessor *mProcessor;
};


#endif