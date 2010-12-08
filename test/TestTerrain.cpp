#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "components/ogre/terrain/ICompilerTechniqueProvider.h"
#include "components/ogre/terrain/TerrainHandler.h"
#include "components/ogre/terrain/Types.h"
#include "components/ogre/terrain/TerrainDefPoint.h"

#include "framework/Exception.h"
#include "main/Application.h"

#include <wfmath/timestamp.h>

#include <Ogre.h>
#include <sigc++/signal.h>
#include <boost/thread/condition_variable.hpp>

using namespace Ember::OgreView;
using namespace Ember::OgreView::Terrain;

namespace Ember
{

template<> Application *Singleton<Application>::ms_Singleton = 0;

void Application::requestQuit()
{
}

class DummyCompilerTechniqueProvider: public ICompilerTechniqueProvider
{
public:
	virtual TerrainPageSurfaceCompilerTechnique* createTechnique(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow) const
	{
		return 0;
	}

};

/**
 * @brief Base class for listening to events. After the event is triggered isCompleted() will return true.
 *
 * For background events, call waitForCompletion(...).
 */
class CompleteEventListener
{
protected:
	boost::condition_variable mCondition;
	boost::mutex mMutex;
	bool mCompleted;

	void handleEvent()
	{
		{
			boost::lock_guard<boost::mutex> lock(mMutex);
			mCompleted = true;
		}
		mCondition.notify_all();
	}

public:
	CompleteEventListener() :
		mCompleted(false)
	{
	}

	bool waitForCompletion(long milliseconds)
	{
		boost::unique_lock<boost::mutex> lock(mMutex);
		if (mCompleted) {
			return true;
		}
		mCondition.timed_wait(lock, boost::posix_time::milliseconds(milliseconds));
		return mCompleted;
	}

	bool isCompleted()
	{
		return mCompleted;
	}
};

class WorldSizeChangedListener: public CompleteEventListener
{
protected:

	void eventListener()
	{
		handleEvent();
	}

public:
	WorldSizeChangedListener(sigc::signal<void>& event)
	{
		event.connect(sigc::mem_fun(*this, &WorldSizeChangedListener::eventListener));
	}
};

class TerrainTestCase: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TerrainTestCase);
	CPPUNIT_TEST( testCreateTerrain);

CPPUNIT_TEST_SUITE_END();

public:
	void testCreateTerrain()
	{

		Ogre::Root root;

		sigc::signal<void, float> signal;
		DummyCompilerTechniqueProvider compilerTechniqueProvider;
		Terrain::TerrainHandler terrainHandler(513, compilerTechniqueProvider);
		WorldSizeChangedListener worldSizeChangedListener(terrainHandler.EventWorldSizeChanged);

		TerrainDefPointStore terrainDefPoints;
		terrainDefPoints.push_back(TerrainDefPoint(-1, -1, -1));
		terrainDefPoints.push_back(TerrainDefPoint(-1, 1, 1));
		terrainDefPoints.push_back(TerrainDefPoint(1, 1, -1));
		terrainDefPoints.push_back(TerrainDefPoint(1, -1, 1));

		terrainHandler.updateTerrain(terrainDefPoints);

		WFMath::TimeStamp startTime = WFMath::TimeStamp::now();

		do {
			terrainHandler.pollTasks();
		} while (((WFMath::TimeStamp::now() - startTime).milliseconds() < 5000) && !worldSizeChangedListener.isCompleted());

		CPPUNIT_ASSERT(worldSizeChangedListener.isCompleted());
	}

};

}

CPPUNIT_TEST_SUITE_REGISTRATION( Ember::TerrainTestCase);

int main(int argc, char **argv)
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest(registry.makeTest());

	// Shows a message as each test starts
	CppUnit::BriefTestProgressListener listener;
	runner.eventManager().addListener(&listener);

	bool wasSuccessful = runner.run("", false);
	return !wasSuccessful;
}

