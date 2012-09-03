#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "framework/TinyXmlCodec.h"
#include "framework/AtlasMessageLoader.h"
#include "framework/tinyxml/tinyxml.h"

#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/Root.h>
#include <wfmath/timestamp.h>

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

namespace Ember
{

class FrameworkTestCase: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(FrameworkTestCase);
	CPPUNIT_TEST(testTinyXmlCodec);

	CPPUNIT_TEST_SUITE_END()
	;

public:
	void testTinyXmlCodec()
	{
		TiXmlDocument xmlDoc;
		bool result = xmlDoc.LoadFile(std::string(SRCDIR) + "/atlas.xml", TIXML_DEFAULT_ENCODING);
		CPPUNIT_ASSERT(result);

		std::map<std::string, Atlas::Objects::Root> messages;

		AtlasMessageLoader loader(messages);
		TinyXmlCodec codec(*xmlDoc.RootElement(), loader);

		codec.poll(true);

		CPPUNIT_ASSERT(messages.size() == 2);
	}

};

}

CPPUNIT_TEST_SUITE_REGISTRATION( Ember::FrameworkTestCase);

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

