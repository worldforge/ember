#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "framework/tasks/TaskQueue.h"
#include "framework/tasks/ITask.h"
#include "framework/tasks/ITaskExecutionListener.h"
#include "framework/Exception.h"

#include <wfmath/timestamp.h>

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

struct TimeHolder {
public:
	WFMath::TimeStamp time;

};

class TimeTask: public Tasks::ITask
{
public:

	TimeHolder& timeHolder;

	TimeTask(TimeHolder& timeHolder)
	: timeHolder(timeHolder)
	{
	}

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
	{
	}

	/**
	 * @brief Executes the task in the main thread, after executeTaskInBackgroundThread() has been called.
	 * Since this will happen in the main thread you shouldn't do any time consuming things here, since it will lock up the rendering.
	 */
	virtual void executeTaskInMainThread()
	{
		timeHolder.time = WFMath::TimeStamp::now();
	}

	virtual std::string getName() const {
		return "TimeTask";
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
	WFMath::TimeStamp startedTime;
	WFMath::TimeStamp endedTime;
	WFMath::TimeStamp errorTime;

	SimpleListener() : started(false), ended(false), error(false) {

	}

	virtual void executionStarted()
	{
		startedTime = WFMath::TimeStamp::now();
		started = true;
	}

	virtual void executionEnded()
	{
		endedTime = WFMath::TimeStamp::now();
		ended = true;
	}

	virtual void executionError(const Ember::Exception& exception)
	{
		errorTime = WFMath::TimeStamp::now();
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
	CPPUNIT_TEST(testTaskOrder);

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

	void testTaskOrder()
	{
		SimpleListener listener1;
		SimpleListener listener2;
		SimpleListener listener3;
		TimeHolder time1;
		TimeHolder time2;
		TimeHolder time3;
		{
			Tasks::TaskQueue taskQueue(1);
			taskQueue.enqueueTask(new TimeTask(time1), &listener1);
			taskQueue.enqueueTask(new TimeTask(time2), &listener2);
			taskQueue.enqueueTask(new TimeTask(time3), &listener3);
		}
		CPPUNIT_ASSERT(listener1.startedTime < listener2.startedTime);
		CPPUNIT_ASSERT(listener2.startedTime < listener3.startedTime);
		CPPUNIT_ASSERT(listener1.endedTime < listener2.endedTime);
		CPPUNIT_ASSERT(listener2.endedTime < listener3.endedTime);
		CPPUNIT_ASSERT(time1.time < time2.time);
		CPPUNIT_ASSERT(time2.time < time3.time);
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

