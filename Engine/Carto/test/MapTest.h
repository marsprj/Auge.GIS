#ifndef __AUGE_MAP_TEST__H__
#define __AUGE_MAP_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#include "AugeData.h"

class MapTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(MapTest);
	//CPPUNIT_TEST(DrawMap);
	CPPUNIT_TEST(Create_Map_Point_Label);
	//CPPUNIT_TEST(Draw_Map_Point_Label);
	//CPPUNIT_TEST(DrawMapObj);	
	CPPUNIT_TEST(CreateMap);
	CPPUNIT_TEST(CreateLayer);
	CPPUNIT_TEST(DrawMapSLD);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void		DrawMap();
	void		DrawMapObj();
	void		DrawMapSLD();
	void		CreateMap();

	void		Draw_Map_Point_Label();
	void		Create_Map_Point_Label();

	void		CreateLayer();

private:
	auge::GConnection	*m_pConnection;

};

#endif //__AUGE_MAP_TEST__H__