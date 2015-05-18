#ifndef __AUGE_TILE_TEST__H__
#define __AUGE_TILE_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class TileTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(TileTest);
	//CPPUNIT_TEST(WriteTest);
	CPPUNIT_TEST(GetTile);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:

	void	WriteTest();
	void	GetTile();

private:
	auge::TileWorkspace* m_pWorkspace;
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_TILE_TEST__H__
