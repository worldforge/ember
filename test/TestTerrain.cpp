#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "components/ogre/terrain/ICompilerTechniqueProvider.h"
#include "components/ogre/terrain/ITerrainPageBridge.h"
#include "components/ogre/terrain/TerrainHandler.h"
#include "components/ogre/terrain/Types.h"
#include "components/ogre/terrain/TerrainDefPoint.h"
#include "components/ogre/terrain/TerrainInfo.h"

#include "framework/Exception.h"
#include "main/Application.h"

#include <wfmath/timestamp.h>

#include <Ogre.h>
#include <sigc++/signal.h>
#include <sigc++/trackable.h>

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

class DummyTerrainBridge: public ITerrainPageBridge
{
public:

	bool pageReady;

	DummyTerrainBridge() :
		pageReady(false)
	{
	}

	/**
	 *    @brief Updates the Ogre mesh after changes to the underlying heightdata.
	 *
	 * Call this when the heightdata has changed and you want the Ogre representation to be updated to reflect this.
	 */
	virtual void updateTerrain(TerrainPageGeometry& geometry)
	{
	}

	/**
	 *    @brief Notifies class in the ogre side about the page being ready (after being created or so).
	 */
	virtual void terrainPageReady()
	{
		pageReady = true;
	}

};

class Timer
{
public:

	WFMath::TimeStamp startTime;

	Timer() :
		startTime(WFMath::TimeStamp::now())
	{

	}

	bool hasElapsed(long milliseconds)
	{
		return (WFMath::TimeStamp::now() - startTime).milliseconds() > milliseconds;
	}

};

/**
 * @brief Base class for listening to events. After the event is triggered isCompleted() will return true.
 *
 * For background events, call waitForCompletion(...).
 */
class CompleteEventListener: public virtual sigc::trackable
{
protected:
	boost::condition_variable mCondition;
	boost::mutex mMutex;
	int mCompletedCount;

	void handleEvent()
	{
		{
			boost::lock_guard<boost::mutex> lock(mMutex);
			mCompletedCount++;
		}
		mCondition.notify_all();
	}

public:
	CompleteEventListener() :
		mCompletedCount(0)
	{
	}

	int waitForCompletion(long milliseconds)
	{
		boost::unique_lock<boost::mutex> lock(mMutex);
		if (mCompletedCount) {
			return true;
		}
		mCondition.timed_wait(lock, boost::posix_time::milliseconds(milliseconds));
		return mCompletedCount;
	}

	int getCompletedCount()
	{
		return mCompletedCount;
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

class AfterTerrainUpdateListener: public CompleteEventListener
{
protected:

	void eventListener(const std::vector<WFMath::AxisBox<2> >& areas, const std::set<TerrainPage*>& pages)
	{
		handleEvent();
	}

public:
	AfterTerrainUpdateListener(sigc::signal<void, const std::vector<WFMath::AxisBox<2> >&, const std::set<TerrainPage*>&>& event)
	{
		event.connect(sigc::mem_fun(*this, &AfterTerrainUpdateListener::eventListener));
	}
};

class TerrainSetup
{
public:
	DummyCompilerTechniqueProvider compilerTechniqueProvider;
	Terrain::TerrainHandler terrainHandler;

	TerrainSetup() :
		terrainHandler(513, compilerTechniqueProvider)
	{
	}

	bool createBaseTerrain(float height)
	{
		WorldSizeChangedListener worldSizeChangedListener(terrainHandler.EventWorldSizeChanged);

		TerrainDefPointStore terrainDefPoints;
		terrainDefPoints.push_back(TerrainDefPoint(-1, -1, height));
		terrainDefPoints.push_back(TerrainDefPoint(-1, 0, height));
		terrainDefPoints.push_back(TerrainDefPoint(-1, 1, height));
		terrainDefPoints.push_back(TerrainDefPoint(0, -1, height));
		terrainDefPoints.push_back(TerrainDefPoint(0, 0, height));
		terrainDefPoints.push_back(TerrainDefPoint(0, 1, height));
		terrainDefPoints.push_back(TerrainDefPoint(1, -1, height));
		terrainDefPoints.push_back(TerrainDefPoint(1, 0, height));
		terrainDefPoints.push_back(TerrainDefPoint(1, 1, height));

		terrainHandler.updateTerrain(terrainDefPoints);
		Timer timer;
		do {
			terrainHandler.pollTasks();
		} while (!timer.hasElapsed(5000) && !worldSizeChangedListener.getCompletedCount());
		return worldSizeChangedListener.getCompletedCount() > 0;
	}

	bool createPages()
	{
		DummyTerrainBridge* bridge1 = new DummyTerrainBridge();
		DummyTerrainBridge* bridge2 = new DummyTerrainBridge();
		DummyTerrainBridge* bridge3 = new DummyTerrainBridge();
		DummyTerrainBridge* bridge4 = new DummyTerrainBridge();

		terrainHandler.setUpTerrainPageAtIndex(TerrainIndex(0, 0), bridge1);
		terrainHandler.setUpTerrainPageAtIndex(TerrainIndex(0, 1), bridge2);
		terrainHandler.setUpTerrainPageAtIndex(TerrainIndex(-1, 0), bridge3);
		terrainHandler.setUpTerrainPageAtIndex(TerrainIndex(-1, 1), bridge4);

		{
			Timer timer;
			do {
				terrainHandler.pollTasks();
			} while ((!timer.hasElapsed(5000)) && (!bridge1->pageReady || !bridge2->pageReady || !bridge3->pageReady || !bridge4->pageReady));
		}
		CPPUNIT_ASSERT(bridge1->pageReady);
		CPPUNIT_ASSERT(bridge2->pageReady);
		CPPUNIT_ASSERT(bridge3->pageReady);
		CPPUNIT_ASSERT(bridge4->pageReady);
		return true;
	}

	bool reloadTerrain()
	{
		AfterTerrainUpdateListener afterTerrainUpdateListener(terrainHandler.EventAfterTerrainUpdate);
		std::vector<TerrainPosition> positions;
		positions.push_back(TerrainPosition(-32, -32));
		positions.push_back(TerrainPosition(-32, 32));
		positions.push_back(TerrainPosition(32, 32));
		positions.push_back(TerrainPosition(32, -32));

		terrainHandler.reloadTerrain(positions);

		{
			Timer timer;
			do {
				terrainHandler.pollTasks();
			} while ((!timer.hasElapsed(5000)) && afterTerrainUpdateListener.getCompletedCount() < 4);
		}
		CPPUNIT_ASSERT(afterTerrainUpdateListener.getCompletedCount() == 4);
		return true;
	}
};

class TerrainTestCase: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TerrainTestCase);
	CPPUNIT_TEST( testCreateTerrain);
	CPPUNIT_TEST( testAlterTerrain);

CPPUNIT_TEST_SUITE_END();

protected:

public:
	void testCreateTerrain()
	{

		Ogre::Root root;

		TerrainSetup terrainSetup;

		float terrainHeight = 25.0f;

		CPPUNIT_ASSERT(terrainSetup.createBaseTerrain(terrainHeight));
		CPPUNIT_ASSERT(terrainSetup.createPages());
		CPPUNIT_ASSERT(terrainSetup.reloadTerrain());

		CPPUNIT_ASSERT(terrainSetup.terrainHandler.getTerrainInfo().getTotalNumberOfPagesX() == 2);
		CPPUNIT_ASSERT(terrainSetup.terrainHandler.getTerrainInfo().getTotalNumberOfPagesY() == 2);
		float height = 0;
		CPPUNIT_ASSERT(terrainSetup.terrainHandler.getHeight(TerrainPosition(-1, -1), height));
		CPPUNIT_ASSERT(height == terrainHeight);

	}

	void testAlterTerrain()
	{

		Ogre::Root root;
		TerrainSetup terrainSetup;

		Terrain::TerrainHandler& terrainHandler = terrainSetup.terrainHandler;
		CPPUNIT_ASSERT(terrainSetup.createBaseTerrain(25));
		CPPUNIT_ASSERT(terrainSetup.createPages());
		CPPUNIT_ASSERT(terrainSetup.reloadTerrain());

		TerrainDefPointStore terrainDefPoints;
		terrainDefPoints.push_back(TerrainDefPoint(-1, -1, -200));
		terrainDefPoints.push_back(TerrainDefPoint(-1, 0, -200));
		terrainDefPoints.push_back(TerrainDefPoint(0, -1, -200));
		terrainDefPoints.push_back(TerrainDefPoint(0, 0, -200));

		terrainHandler.updateTerrain(terrainDefPoints);

		AfterTerrainUpdateListener afterTerrainUpdateListener(terrainHandler.EventAfterTerrainUpdate);

		{
			Timer timer;
			do {
				terrainHandler.pollTasks();
			} while ((!timer.hasElapsed(5000)) && afterTerrainUpdateListener.getCompletedCount() < 4);
		}
		CPPUNIT_ASSERT(afterTerrainUpdateListener.getCompletedCount() == 4);
		CPPUNIT_ASSERT(terrainHandler.getTerrainInfo().getTotalNumberOfPagesX() == 2);
		CPPUNIT_ASSERT(terrainHandler.getTerrainInfo().getTotalNumberOfPagesY() == 2);
		float height = 0;
		CPPUNIT_ASSERT(terrainHandler.getHeight(TerrainPosition(-32, -32), height));
		CPPUNIT_ASSERT(height == -200);
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

