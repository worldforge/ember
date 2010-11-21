#include "ConvertTestCase.h"

#include "components/ogre/Convert.h"

using namespace EmberOgre;
using namespace WFMath;

namespace Ember
{
void ConvertTestCase::testWFMathToOgre()
{
	Point<2> wfMathPoint2(10, 20);
	CPPUNIT_ASSERT(wfMathPoint2 == Convert::toWF(Convert::toOgre<Ogre::Vector2>(wfMathPoint2)));
	CPPUNIT_ASSERT(wfMathPoint2 == Convert::toWF<Point<2> >(Convert::toOgre<Ogre::Vector3>(wfMathPoint2)));

	Point<3> wfMathPoint3(10, 20, 30);
	CPPUNIT_ASSERT(wfMathPoint3 == Convert::toWF<Point<3> >(Convert::toOgre(wfMathPoint3)));

	Vector<2> wfMathVector2(10, 20);
	CPPUNIT_ASSERT(Point<2>(wfMathVector2) == Convert::toWF(Convert::toOgre(wfMathVector2)));

	Vector<3> wfMathVector3(10, 20, 30);
	CPPUNIT_ASSERT(wfMathVector3 == Convert::toWF<Vector<3> >(Convert::toOgre(wfMathVector3)));

	Quaternion wfQuaternion(1, 2.4f);
	CPPUNIT_ASSERT(wfQuaternion == Convert::toWF(Convert::toOgre(wfQuaternion)));

	WFMath::AxisBox<3> wfAxisBox3(Point<3>(1,2,3), Point<3>(10, 20, 30));
	CPPUNIT_ASSERT(wfAxisBox3 == Convert::toWF(Convert::toOgre(wfAxisBox3)));

	WFMath::AxisBox<2> wfAxisBox2(Point<2>(1,2), Point<2>(10, 20));
	CPPUNIT_ASSERT(wfAxisBox2 == Convert::toWF(Convert::toOgre(wfAxisBox2)));

}
}
