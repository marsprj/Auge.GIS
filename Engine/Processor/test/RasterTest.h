#ifndef __AUGE_RASTER_TEST__H__
#define __AUGE_RASTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class RasterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RasterTest);
	//CPPUNIT_TEST(ExtractByRectangle);
	//CPPUNIT_TEST(Stretch);
	//CPPUNIT_TEST(EdgeDetorSobol);
	//CPPUNIT_TEST(Graylize);
	//CPPUNIT_TEST(ToJPG);
	//CPPUNIT_TEST(Reverse);
	//CPPUNIT_TEST(Smooth);
	//CPPUNIT_TEST(Subtract);
	//CPPUNIT_TEST(PixelBlend);
	//CPPUNIT_TEST(HistgramEqualization);
	//CPPUNIT_TEST(Threshold);
	//CPPUNIT_TEST(Thumbnail);
	//CPPUNIT_TEST(RasterMosiac);
	CPPUNIT_TEST(RasterProj);

	//CPPUNIT_TEST(SepiaTone);
	//CPPUNIT_TEST(PenEffect);
	//CPPUNIT_TEST(DEMSlope);
	//CPPUNIT_TEST(DEMSlope_2);
	//CPPUNIT_TEST(DEMAspect);
	//CPPUNIT_TEST(DEMAspect_2);
	//CPPUNIT_TEST(DEMHillShade);
	//CPPUNIT_TEST(DEMStretch);
	//CPPUNIT_TEST(DemInundation);

	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void	ExtractByRectangle();
	void	Stretch();
	void	EdgeDetorSobol();
	void	Graylize();
	void	ToJPG();
	void	Reverse();
	void	Smooth();
	void	Subtract();
	void	PixelBlend();
	void	HistgramEqualization();
	void	Threshold();
	void	Thumbnail();
	void	RasterMosiac();
	void	RasterProj();

	void	SepiaTone();
	void	PenEffect();

	void	DEMSlope();
	void	DEMSlope_2();
	void	DEMAspect();
	void	DEMAspect_2();
	void	DEMHillShade();
	void	DEMStretch();
	void	DemInundation();

private:
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_RASTER_TEST__H__
