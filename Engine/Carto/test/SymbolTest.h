#ifndef __AUGE_SYMBOL_TEST__H__
#define __AUGE_SYMBOL_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#include "AugeData.h"
#include "AugeStyle.h"

class SymbolTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(SymbolTest);
	//CPPUNIT_TEST(DrawStar);
	CPPUNIT_TEST(DrawStars);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void		DrawStar();
	void		DrawStars();

private:
	auge::GConnection	*m_pConnection;


};

#endif //__AUGE_SYMBOL_TEST__H__