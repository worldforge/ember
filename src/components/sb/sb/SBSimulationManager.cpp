#include "SBSimulationManager.h"
#include "SBScene.h"
#include "SBScript.h"
//#include "SBSteerManager.h"
#include "components/sb/sbm/time_regulator.h"
#include "components/sb/sbm/time_profiler.h"


namespace SmartBody {

SBProfiler::SBProfiler()
{
	_profiler = new TimeIntervalProfiler();
}

SBProfiler::~SBProfiler()
{
}

void SBProfiler::printLegend()
{
	_profiler->print_legend();
}

void SBProfiler::printStats()
{
	_profiler->print();
}


SBSimulationManager::SBSimulationManager()
{
	_simStarted = false;
	_simPlaying = false;
	_hasTimer = false;
	_simStopped = false;

	internal_profiler_p = NULL;
	external_profiler_p = NULL;
	profiler_p = NULL;
	internal_timer_p = NULL;
	external_timer_p = NULL;
	timer_p = NULL;
	time = 0.0;
	time_dt = 0.16;

	_profiler = new SBProfiler();
}

SBSimulationManager::~SBSimulationManager()
{
	if (_hasTimer)
	{
		timer_p;
	}

	internal_profiler_p = NULL;
	external_profiler_p = NULL;
	profiler_p = NULL;
	internal_timer_p = NULL;
	external_timer_p = NULL;
	timer_p = NULL;

	delete _profiler;
}

void SBSimulationManager::printInfo()
{
	if (timer_p)	
		timer_p->print();
	else	
	{
		LOG( "TIME:%.3f ~ DT:%.3f %.2f:FPS\n",
			time,
			time_dt,
			1.0 / time_dt
		);
	}
}

void SBSimulationManager::printPerf(float v)
{
	if (timer_p)
	{
		if (v > 0.0) 
			timer_p->set_perf(v);
		else	
			timer_p->set_perf(10.0);	
	}
	else
		LOG("Time regulator does not exist!");
}

double SBSimulationManager::getTime()
{
	return time;
}

double SBSimulationManager::queryTime()
{
	if (timer_p)
		return timer_p->get_time();
	else
		return time;
}

double SBSimulationManager::getTimeDt()
{
	return time_dt;
}

void SBSimulationManager::setTime(double time)
{
	updateTimer(time);	
}

void SBSimulationManager::update()
{
	if (timer_p)
	{
		bool doUpdate = updateTimer();
		if (!doUpdate)
			return;
	}
}

bool SBSimulationManager::isStarted()
{
	if (timer_p)
		return timer_p->isStarted();
	else
	{
		if (_simStarted)
			return true;
		else
			return false;
	}
}

bool SBSimulationManager::isRunning()
{
	if (timer_p)
		return timer_p->isRunning();
	else
	{
		if (_simPlaying)
			return true;
		else
			return false;
	}
}


bool SBSimulationManager::isStopped()
{
	return _simStopped;
}

void SBSimulationManager::reset()
{
	if (timer_p)	
		timer_p->reset();
	else
	{
		return;
	}
}

void SBSimulationManager::start()
{
	// run the start scripts
	std::map<std::string, SBScript*>& scripts = SmartBody::SBScene::getScene()->getScripts();
	for (std::map<std::string, SBScript*>::iterator iter = scripts.begin();
		 iter != scripts.end();
		 iter++)
	{
		(*iter).second->start();
	}
	

	if (timer_p)	
	{
		timer_p->start();
	}
	else
	{
		_simStarted = true;
	}
}

void SBSimulationManager::stop()
{
	// run the stop scripts
	std::map<std::string, SBScript*>& scripts = SmartBody::SBScene::getScene()->getScripts();
	for (std::map<std::string, SBScript*>::iterator iter = scripts.begin();
		 iter != scripts.end();
		 iter++)
	{
		(*iter).second->stop();
	}
	
	if (timer_p)	
	{
		timer_p->stop();
	}
	else
	{
		_simStarted = false;
		_simPlaying = false;
		_simStopped = true;
	}
}

void SBSimulationManager::pause()
{
	if (timer_p)
	{
		timer_p->pause();
	}
	else
	{
		_simPlaying = false;
	}
}

void SBSimulationManager::resume()
{
	if (timer_p)
	{
		timer_p->resume();
	}
	else
	{
		_simPlaying = true;
	}
}

void SBSimulationManager::step(int numSteps)
{
	if (timer_p)
	{
		timer_p->step(numSteps);
	}
}

void SBSimulationManager::setSleepFps(float v)
{
	if (!timer_p)	
	{
		LOG("Time regulator not exist!");
		return;
	}
	timer_p->set_sleep_fps(v);
}

void SBSimulationManager::setEvalFps(float v)
{
	if (!timer_p)	
	{
		LOG("Time regulator does not exist!");
		return;
	}
	timer_p->set_eval_fps(v);
}

void SBSimulationManager::setSimFps(float v)
{
	if (!timer_p)	
	{
		LOG("Time regulator does not exist!");
		return;
	}
	timer_p->set_sim_fps(v);
}

void SBSimulationManager::setSleepDt(float v)
{
	if (!timer_p)	
	{
		LOG("Time regulator not exist!");
		return;
	}
	timer_p->set_sleep_dt(v);
}

void SBSimulationManager::setEvalDt(float v)
{
	if (!timer_p)	
	{
		LOG("Time regulator does not exist!");
		return;
	}
	timer_p->set_eval_dt(v);
}

void SBSimulationManager::setSimDt(float v)
{
	if (!timer_p)	
	{
		LOG("Time regulator does not exist!");
		return;
	}
	timer_p->set_sim_dt(v);
}

void SBSimulationManager::setSpeed(float v)
{
	if (timer_p)	
		timer_p->set_speed(v);
	else
		LOG("Time regulator does not exist!");
}

void SBSimulationManager::setupTimer()
{
	TimeRegulator* timer = new TimeRegulator();
	_hasTimer = true;

	register_timer( *timer );
}

void SBSimulationManager::setSleepLock()
{
	if (!timer_p)	
	{
		LOG("Time regulator does not exist!");
		return;
	}
	timer_p->set_sleep_lock();
}

SBProfiler* SBSimulationManager::getProfiler()
{
	return _profiler;
}

void SBSimulationManager::setupProfiler()
{
	external_profiler_p = new TimeIntervalProfiler();
	profiler_p = external_profiler_p;
}
void SBSimulationManager::switch_internal_profiler( void )	{
	if( internal_profiler_p == NULL ) internal_profiler_p = new TimeIntervalProfiler;
	profiler_p = internal_profiler_p;
}
void SBSimulationManager::mark( const char* group_name, int level, const char* label )	{
	if( profiler_p ) profiler_p->mark( group_name, level, label );
}
int SBSimulationManager::mark( const char* group_name )	{
	if( profiler_p ) return( profiler_p->mark( group_name ) );
	return( 0 );
}

void SBSimulationManager::set_perf(float val)
{
	if (timer_p)
		timer_p->set_perf(val);
}

void SBSimulationManager::register_timer( TimeRegulator& time_reg )	{
	external_timer_p = &( time_reg );
	timer_p = external_timer_p;
}
void SBSimulationManager::switch_internal_timer( void )	{
	if( internal_timer_p == NULL ) internal_timer_p = new TimeRegulator;
	timer_p = internal_timer_p;
}
void SBSimulationManager::updateProfiler( double in_time )
{
	if( profiler_p )	{
		profiler_p->update();
	}
}
bool SBSimulationManager::updateTimer( double in_time)
{
	if( timer_p )	{
		bool ret = timer_p->update( in_time );
		time = timer_p->get_time();
		time_dt = timer_p->get_dt();
		return( ret );
	}
	double prev = time;
	time = in_time;
	time_dt = time - prev;
	return( true );
}

SBAPI void SBSimulationManager::stepDt( double dt )
{
	double newTime = time + dt;
	updateTimer(newTime);
	//time_dt = dt;	
}

}

