#include <cppunit/extensions/HelperMacros.h>

namespace Ember {
	class ModelMountTestCase : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(ModelMountTestCase);
		CPPUNIT_TEST(testModelMountScaling);
		CPPUNIT_TEST_SUITE_END();

	public:
		void testModelMountScaling();
	};
}
