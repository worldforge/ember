#include "ConvertTestCase.h"

#include "components/ogre/Convert.h"

using namespace Ember::OgreView;
using namespace WFMath;

namespace Ember
{
void ConvertTestCase::testWFMathToOgre()
{

	WFMath::Point<3> wfPoint(10, 5, 2);

	WFMath::AxisBox<3> wfBox(WFMath::Point<3>(0, 0, 0), wfPoint);


	WFMath::Quaternion wfQuat;
	wfQuat.rotation(1, -WFMath::numeric_constants<float>::pi() / 2.0f);

	WFMath::RotMatrix<3> wfRotMatrix(wfQuat);

	WFMath::Quaternion wfQuat2;
	wfQuat2.fromRotMatrix(wfRotMatrix);

	wfPoint.rotate(wfQuat, WFMath::Point<3>::ZERO());

	Ogre::AxisAlignedBox ogreBox(Ogre::Vector3(0, 0, 0), Ogre::Vector3(10, 5, 2));
	Ogre::Vector3 ogrePoint(10, 5, 2);
	Ogre::Quaternion ogreQuat = Convert::toOgre(wfQuat);

	Ogre::Matrix3 ogreMatrix;
	ogreQuat.ToRotationMatrix(ogreMatrix);

	Ogre::Quaternion ogreQuat2(ogreMatrix);

	ogreBox.transform(Ogre::Affine3(Ogre::Vector3::ZERO, ogreQuat2));



	Point<2> wfMathPoint2(10, 20);
	CPPUNIT_ASSERT(wfMathPoint2 == Convert::toWF(Convert::toOgre<Ogre::Vector2>(wfMathPoint2)));
	CPPUNIT_ASSERT(wfMathPoint2 == Convert::toWF<Point<2>>(Convert::toOgre<Ogre::Vector3>(wfMathPoint2)));

	Point<3> wfMathPoint3(10, 20, 30);
	CPPUNIT_ASSERT(wfMathPoint3 == Convert::toWF<Point<3>>(Convert::toOgre(wfMathPoint3)));

	Vector<2> wfMathVector2(10, 20);
	CPPUNIT_ASSERT(Point<2>(wfMathVector2) == Convert::toWF(Convert::toOgre(wfMathVector2)));

	Vector<3> wfMathVector3(10, 20, 30);
	CPPUNIT_ASSERT(wfMathVector3 == Convert::toWF<Vector<3>>(Convert::toOgre(wfMathVector3)));

	Quaternion wfQuaternion(1, 2.4f);
	CPPUNIT_ASSERT(wfQuaternion.isEqualTo(Convert::toWF(Convert::toOgre(wfQuaternion)), 0.0001));

	WFMath::AxisBox<3> wfAxisBox3(Point<3>(1,2,3), Point<3>(10, 20, 30));
	CPPUNIT_ASSERT(wfAxisBox3 == Convert::toWF(Convert::toOgre(wfAxisBox3)));

	WFMath::AxisBox<2> wfAxisBox2(Point<2>(1,2), Point<2>(10, 20));
	CPPUNIT_ASSERT(wfAxisBox2 == Convert::toWF(Convert::toOgre(wfAxisBox2)));
	//TRect has bottom >= top
	CPPUNIT_ASSERT(Convert::toOgre(wfAxisBox2).bottom > Convert::toOgre(wfAxisBox2).top);
	CPPUNIT_ASSERT(Convert::toOgre(wfAxisBox2).right > Convert::toOgre(wfAxisBox2).left);



}
}
