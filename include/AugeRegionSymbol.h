#ifndef __AUGE_REGION_SYMBOL_H__
#define __AUGE_REGION_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
	class SimpleRegionSymbol : public RegionSymbol
	{
	protected:
		SimpleRegionSymbol(){}
		virtual ~SimpleRegionSymbol(){}
	};

	class GrassLandSymbol : public RegionSymbol
	{
	protected:
		GrassLandSymbol(){}
		virtual ~GrassLandSymbol(){}
	};
}

#endif //__AUGE_REGION_SYMBOL_H__
