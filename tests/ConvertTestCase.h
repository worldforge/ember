#include <cppunit/extensions/HelperMacros.h>

namespace Ember {
	class ConvertTestCase : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(ConvertTestCase);
		CPPUNIT_TEST(testWFMathToOgre);
		CPPUNIT_TEST_SUITE_END();

	public:
		void testWFMathToOgre();
	};
}
