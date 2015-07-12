#ifndef __AUGE_CANVAS_TEST__H__
#define __AUGE_CANVAS_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

class CanvasTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(CanvasTest);
	//CPPUNIT_TEST(BackgroundTest);
	CPPUNIT_TEST(DrawPoint);
	//CPPUNIT_TEST(DrawLine);
	//CPPUNIT_TEST(DrawPolygon);
	//CPPUNIT_TEST(DrawShpPoint);
	//CPPUNIT_TEST(DrawShpLine);
	//CPPUNIT_TEST(DrawShpPolygon);
	//CPPUNIT_TEST(DrawColorMap);
	//CPPUNIT_TEST(ReadColor);
	
	//CPPUNIT_TEST(DrawRasterLayer);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void BackgroundTest();
	void DrawPoint();
	void DrawLine();
	void DrawPolygon();

	//void DrawShpPoint();
	//void DrawShpLine();
	void DrawShpPolygon();

	void DrawColorMap();
	void ReadColor();

	

	//void DrawRasterLayer();
	
};

#endif //__AUGE_CANVAS_TEST__H__