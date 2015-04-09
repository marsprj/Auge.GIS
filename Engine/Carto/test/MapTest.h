#ifndef __AUGE_MAP_TEST__H__
#define __AUGE_MAP_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#include "AugeData.h"
#include "AugeStyle.h"

class MapTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(MapTest);
	//CPPUNIT_TEST(DrawMap);
	//CPPUNIT_TEST(Create_Map_Point_Label);

	//CPPUNIT_TEST(Draw_Map_Point_Label);
	//CPPUNIT_TEST(Draw_Map_Point_Label_Anchor);
	CPPUNIT_TEST(Draw_Map_Point_Graphic);


	//CPPUNIT_TEST(DrawMapObj);	
	//CPPUNIT_TEST(CreateMap);
	//CPPUNIT_TEST(CreateLayer);
	//CPPUNIT_TEST(DrawMapSLD);

	//CPPUNIT_TEST(Draw_Map_Polygon_Label);
	
	//CPPUNIT_TEST(DrawCities);
	//CPPUNIT_TEST(DrawCountry);
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
	void		Draw_Map_Point_Label_Anchor();

	void		Draw_Map_Point_Graphic();

	void		Draw_Map_Polygon_Label();

	void		Create_Map_Point_Label();

	void		DrawCities();
	void		DrawCountry();

	void		CreateLayer();

private:
	auge::Style* LoadSLD(const char* path);
private:
	auge::GConnection	*m_pConnection;


};

#endif //__AUGE_MAP_TEST__H__