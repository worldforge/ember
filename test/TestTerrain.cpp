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
#include "components/ogre/terrain/TerrainMod.h"

#include "framework/Exception.h"

#include <Eris/Entity.h>

#include <Atlas/Message/Element.h>

#include <Mercator/Terrain.h>

#include <wfmath/timestamp.h>
#include <wfmath/atlasconv.h>

#include <Ogre.h>
#include <sigc++/signal.h>
#include <sigc++/trackable.h>

#include <boost/thread/condition_variable.hpp>

using namespace Ember::OgreView;
using namespace Ember::OgreView::Terrain;
using namespace Ember::Domain;

namespace Ember
{

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

class DummyEntity: public Eris::Entity
{
public:
	DummyEntity() :
		Eris::Entity("0", 0)
	{
	}

	Eris::TypeService* getTypeService() const
	{
		return 0;
	}

	void removeFromMovementPrediction()
	{
	}

	void addToMovementPredition()
	{
	}

	Eris::Entity* getEntity(const std::string& id)
	{
		return 0;
	}

	void setAttr(const std::string &p, const Atlas::Message::Element &v)
	{
		Eris::Entity::setAttr(p, v);
	}

	void setFromMessage(const Atlas::Message::MapType& attrs)
	{
		beginUpdate();

		Atlas::Message::MapType::const_iterator A;

		for (A = attrs.begin(); A != attrs.end(); ++A) {
			if (A->first == "loc" || A->first == "id" || A->first == "contains")
				continue;

			Eris::Entity::AttrMap::iterator I = m_attrs.find(A->first);
			if ((I != m_attrs.end()) && (I->second == A->second))
				continue;

			setAttr(A->first, A->second);
		}

		endUpdate();
	}

};

class EntityHolder
{
public:
	Eris::Entity& entity;
	EntityHolder(Eris::Entity& entity) :
		entity(entity)
	{

	}
	~EntityHolder()
	{
		entity.shutdown();
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
			boost::lock_guard < boost::mutex > lock(mMutex);
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
		boost::unique_lock < boost::mutex > lock(mMutex);
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

class TestTerrainHandler: public Terrain::TerrainHandler
{
public:

	TestTerrainHandler(int pageIndexSize, ICompilerTechniqueProvider& compilerTechniqueProvider) :
		Terrain::TerrainHandler(pageIndexSize, compilerTechniqueProvider)
	{

	}

	Mercator::Terrain* getTerrain()
	{
		return mTerrain;
	}

	/**
	 * Checks the height, using an integer.
	 * This is because of the floats being used, we can't be 100% certain that a piece of terrain should be exactly the specified height.
	 */
	bool checkTerrainHeight(float x, float y, int height)
	{
		float realHeight = 0;
		getHeight(TerrainPosition(x,y), realHeight);
		return (int)(realHeight) == height;
	}
};

class TerrainSetup
{
public:
	DummyCompilerTechniqueProvider compilerTechniqueProvider;
	TestTerrainHandler terrainHandler;

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
	CPPUNIT_TEST( testApplyMod);
	CPPUNIT_TEST( testUpdateMod);

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

	void testApplyMod()
	{

		Ogre::Root root;

		TerrainSetup terrainSetup;
		Terrain::TerrainHandler& terrainHandler = terrainSetup.terrainHandler;
		DummyEntity entity;
		EntityHolder entityHolder(entity);
		entity.setAttr("pos", WFMath::Point<3>::ZERO().toAtlas());

		float terrainHeight = 25.0f;

		CPPUNIT_ASSERT(terrainSetup.createBaseTerrain(terrainHeight));
		CPPUNIT_ASSERT(terrainSetup.createPages());
		CPPUNIT_ASSERT(terrainSetup.reloadTerrain());

		Atlas::Message::ListType polygon;

		polygon.push_back(WFMath::Point<2>(-10, -10).toAtlas());
		polygon.push_back(WFMath::Point<2>(-10, 10).toAtlas());
		polygon.push_back(WFMath::Point<2>(10, 10).toAtlas());
		polygon.push_back(WFMath::Point<2>(10, -10).toAtlas());

		Atlas::Message::MapType shape;
		shape["points"] = polygon;
		shape["type"] = "polygon";

		Atlas::Message::MapType mod;
		mod["shape"] = shape;
		mod["type"] = "levelmod";
		mod["heightoffset"] = 2.0f;

		entity.setAttr("terrainmod", mod);

		OgreView::Terrain::TerrainMod* terrainMod = new Terrain::TerrainMod(entity);
		terrainMod->init();
		terrainHandler.addTerrainMod(terrainMod);

	}

	void testUpdateMod()
	{

		Ogre::Root root;

		TerrainSetup terrainSetup;
		TestTerrainHandler& terrainHandler = terrainSetup.terrainHandler;
		DummyEntity entity;
		EntityHolder entityHolder(entity);

		entity.setAttr("pos", WFMath::Point<3>::ZERO().toAtlas());

		float terrainHeight = 25.0f;

		CPPUNIT_ASSERT(terrainSetup.createBaseTerrain(terrainHeight));
		CPPUNIT_ASSERT(terrainSetup.createPages());
		CPPUNIT_ASSERT(terrainSetup.reloadTerrain());

		Atlas::Message::ListType polygon;

		polygon.push_back(WFMath::Point<2>(-10, -10).toAtlas());
		polygon.push_back(WFMath::Point<2>(-10, 10).toAtlas());
		polygon.push_back(WFMath::Point<2>(10, 10).toAtlas());
		polygon.push_back(WFMath::Point<2>(10, -10).toAtlas());

		Atlas::Message::MapType shape;
		shape["points"] = polygon;
		shape["type"] = "polygon";

		Atlas::Message::MapType mod;
		mod["shape"] = shape;
		mod["type"] = "levelmod";
		mod["heightoffset"] = 2.0f;

		entity.setAttr("terrainmod", mod);

		OgreView::Terrain::TerrainMod* terrainMod = new Terrain::TerrainMod(entity);
		terrainMod->init();

		{
			AfterTerrainUpdateListener afterTerrainUpdateListener(terrainHandler.EventAfterTerrainUpdate);

			//The shape should affect all four pages.
			terrainHandler.addTerrainMod(terrainMod);

			{
				Timer timer;
				do {
					terrainHandler.pollTasks();
				} while (afterTerrainUpdateListener.getCompletedCount() < 4);
			}
			CPPUNIT_ASSERT(afterTerrainUpdateListener.getCompletedCount() == 4);

			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(-5, -5, 2.0f));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(-5, 5, 2.0f));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(5, 5, 2.0f));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(5, -5, 2.0f));

		}

		mod["heightoffset"] = 20.0f;
		//Setting the attribute should trigger a mod change
		entity.setAttr("terrainmod", mod);

		{
			AfterTerrainUpdateListener afterTerrainUpdateListener(terrainHandler.EventAfterTerrainUpdate);

			{
				Timer timer;
				do {
					terrainHandler.pollTasks();
//				} while ((!timer.hasElapsed(5000)) && afterTerrainUpdateListener.getCompletedCount() < 4);
				} while (afterTerrainUpdateListener.getCompletedCount() < 4);
			}
			CPPUNIT_ASSERT(afterTerrainUpdateListener.getCompletedCount() == 4);
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(-5, -5, 20.0f));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(-5, 5, 20.0f));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(5, 5, 20.0f));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(5, -5, 20.0f));

		}

		//Move the mod out of the way
		entity.setAttr("pos", WFMath::Point<3>(40, 40, 0).toAtlas());

		{
			AfterTerrainUpdateListener afterTerrainUpdateListener(terrainHandler.EventAfterTerrainUpdate);

			{
				Timer timer;
				do {
					terrainHandler.pollTasks();
				} while ((!timer.hasElapsed(5000)) && afterTerrainUpdateListener.getCompletedCount() < 4);
			}
			CPPUNIT_ASSERT(afterTerrainUpdateListener.getCompletedCount() == 4);
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(-5, -5, 25));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(-5, 5, 25));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(5, 5, 25));
			CPPUNIT_ASSERT(terrainHandler.checkTerrainHeight(5, -5, 25));
		}
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

