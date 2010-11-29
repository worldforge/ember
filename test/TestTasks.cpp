#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "framework/tasks/TaskQueue.h"
#include "framework/tasks/ITask.h"

namespace Ember
{

class CounterTask: public Tasks::ITask
{
public:

	int& mCounter;

	CounterTask(int& counter) :
		mCounter(counter)
	{
		mCounter += 2;
	}

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
	{
		mCounter--;
	}

	/**
	 * @brief Executes the task in the main thread, after executeTaskInBackgroundThread() has been called.
	 * Since this will happen in the main thread you shouldn't do any time consuming things here, since it will lock up the rendering.
	 */
	virtual void executeTaskInMainThread()
	{
		mCounter--;
	}

	virtual std::string getName() const {
		return "CounterTask";
	}

};

class TaskTestCase: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TaskTestCase);
	CPPUNIT_TEST( testSimpleTaskRun);
	CPPUNIT_TEST( testSimpleTaskRunTwoExecs);
	CPPUNIT_TEST( testSimpleTaskRunTwoTasks);
	CPPUNIT_TEST( testSimpleTaskRunTwoTasksTwoExecs);
	CPPUNIT_TEST_SUITE_END();

public:
	void testSimpleTaskRun()
	{
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(1);
			taskQueue.enqueueTask(new CounterTask(counter));
		}
		CPPUNIT_ASSERT(counter == 0);
	}

	void testSimpleTaskRunTwoExecs()
	{
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(2);
			taskQueue.enqueueTask(new CounterTask(counter));
		}
		CPPUNIT_ASSERT(counter == 0);
	}

	void testSimpleTaskRunTwoTasks()
	{
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(1);
			taskQueue.enqueueTask(new CounterTask(counter));
			taskQueue.enqueueTask(new CounterTask(counter));
		}
		CPPUNIT_ASSERT(counter == 0);
	}

	void testSimpleTaskRunTwoTasksTwoExecs()
	{
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(2);
			taskQueue.enqueueTask(new CounterTask(counter));
			taskQueue.enqueueTask(new CounterTask(counter));
		}
		CPPUNIT_ASSERT(counter == 0);
	}
};

}

CPPUNIT_TEST_SUITE_REGISTRATION( Ember::TaskTestCase);

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

