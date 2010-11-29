#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "framework/tasks/TaskQueue.h"
#include "framework/tasks/ITask.h"
#include "framework/tasks/ITaskExecutionListener.h"
#include "framework/Exception.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

namespace Ember
{

class CounterTask: public Tasks::ITask
{
public:

	int& mCounter;

	int mSleep;

	CounterTask(int& counter, int sleep = 0) :
		mCounter(counter), mSleep(sleep)
	{
		mCounter += 2;
	}

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
	{
		if (mSleep) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(mSleep));
		}
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

class CounterTaskBackgroundException: public CounterTask {
public:
	CounterTaskBackgroundException(int& counter) : CounterTask(counter) {
	}

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
	{
		throw Ember::Exception();
	}
};

class SimpleListener : public Tasks::ITaskExecutionListener {
public:

	bool started;
	bool ended;
	bool error;

	SimpleListener() : started(false), ended(false), error(false) {

	}

	virtual void executionStarted()
	{
		started = true;
	}

	virtual void executionEnded()
	{
		ended = true;
	}

	virtual void executionError(const Ember::Exception& exception)
	{
		error = true;
	}
};

class TaskTestCase: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TaskTestCase);
	CPPUNIT_TEST(testSimpleTaskRun);
	CPPUNIT_TEST(testSimpleTaskRunAndPoll);
	CPPUNIT_TEST(testSimpleTaskRunTwoExecs);
	CPPUNIT_TEST(testSimpleTaskRunTwoTasks);
	CPPUNIT_TEST(testSimpleTaskRunTwoTasksTwoExecs);
	CPPUNIT_TEST(testListener);
	CPPUNIT_TEST(testBackgroundException);

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

	void testSimpleTaskRunAndPoll()
	{
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(1);
			taskQueue.enqueueTask(new CounterTask(counter));
			//200 ms should be enough... This isn't deterministic though.
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
			taskQueue.pollProcessedTasks(100000);
			CPPUNIT_ASSERT(counter == 0);
		}

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
			taskQueue.enqueueTask(new CounterTask(counter, 200));
			taskQueue.enqueueTask(new CounterTask(counter));
		}
		CPPUNIT_ASSERT(counter == 0);
	}

	void testSimpleTaskRunTwoTasksTwoExecs()
	{
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(2);
			taskQueue.enqueueTask(new CounterTask(counter, 200));
			taskQueue.enqueueTask(new CounterTask(counter));
		}
		CPPUNIT_ASSERT(counter == 0);
	}

	void testListener()
	{
		SimpleListener listener;
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(1);
			taskQueue.enqueueTask(new CounterTask(counter), &listener);
		}
		CPPUNIT_ASSERT(counter == 0);
		CPPUNIT_ASSERT(listener.started);
		CPPUNIT_ASSERT(listener.ended);
	}

	void testBackgroundException()
	{
		SimpleListener listener;
		int counter = 0;
		{
			Tasks::TaskQueue taskQueue(1);
			taskQueue.enqueueTask(new CounterTaskBackgroundException(counter), &listener);
		}
		CPPUNIT_ASSERT(counter == 1);
		CPPUNIT_ASSERT(listener.error);
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

