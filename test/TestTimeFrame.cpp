#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "framework/TimeFrame.h"

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

namespace Ember
{

class TimeFrameTestCase: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(TimeFrameTestCase);
	CPPUNIT_TEST(testElapsedTime);
	CPPUNIT_TEST(testRemainingTime);

	CPPUNIT_TEST_SUITE_END()
	;

public:
	void testElapsedTime()
	{
		TimeFrame tested = TimeFrame(boost::posix_time::microseconds(10));
		const boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
		boost::this_thread::sleep(boost::posix_time::microseconds(5));

		boost::posix_time::time_duration local_elapsed = (boost::posix_time::microsec_clock::local_time() - start);
		boost::posix_time::time_duration elapsed = tested.getElapsedTime();
		CPPUNIT_ASSERT(elapsed >= local_elapsed);
	}

	void testRemainingTime()
	{
		TimeFrame tested = TimeFrame(boost::posix_time::microseconds(10));
		boost::this_thread::sleep(boost::posix_time::microseconds(5));

		boost::posix_time::time_duration remaining = tested.getRemainingTime();
		CPPUNIT_ASSERT(remaining <= boost::posix_time::microseconds(5));
	}

	void testRemainingTimeTruncAtZero()
	{
		TimeFrame tested = TimeFrame(boost::posix_time::microseconds(2));
		boost::this_thread::sleep(boost::posix_time::microseconds(5));

		CPPUNIT_ASSERT(tested.getRemainingTime() == boost::posix_time::microseconds(0));
	}

};

}

CPPUNIT_TEST_SUITE_REGISTRATION( Ember::TimeFrameTestCase);

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

