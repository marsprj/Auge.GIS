#ifndef __AUGE_REGION_SYMBOL_H__
#define __AUGE_REGION_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
	//class SimpleRegionSymbol : public RegionSymbol
	//{
	//public:
	//	SimpleRegionSymbol(){}
	//	virtual ~SimpleRegionSymbol(){}
	//};

	class GrassLandSymbol : public SimpleFillSymbol
	{
	protected:
		GrassLandSymbol(){}
		virtual ~GrassLandSymbol(){}
	};

	class FerryRegionSymbol : public SimpleFillSymbol
	{
	protected:
		FerryRegionSymbol(){}
		virtual ~FerryRegionSymbol(){}
	};

	class ContinentShelfRegionSymbol : public SimpleFillSymbol
	{
	protected:
		ContinentShelfRegionSymbol(){}
		virtual ~ContinentShelfRegionSymbol(){}
	};

}

#endif //__AUGE_REGION_SYMBOL_H__
