#ifndef __AUGE_LINE_SYMBOL_H__
#define __AUGE_LINE_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
	class SimpleLineSymbol : public LineSymbol
	{
	protected:
		SimpleLineSymbol(){}
		virtual ~SimpleLineSymbol(){}
	};

	class RailwaySymbol : public LineSymbol
	{
	protected:
		RailwaySymbol(){}
		virtual ~RailwaySymbol(){}
	};
}

#endif //__AUGE_LINE_SYMBOL_H__
