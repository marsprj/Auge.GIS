#ifndef __AUGE_MARKER_SYMBOL_H__
#define __AUGE_MARKER_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
	class SimpleMarker : public MarkerSymbol
	{
	protected:
		SimpleMarker(){}
		virtual ~SimpleMarker(){}
	};

	class CircleMarker : public MarkerSymbol
	{
	protected:
		CircleMarker(){}
		virtual ~CircleMarker(){}
	};

	class StarMarker : public MarkerSymbol
	{
	protected:
		StarMarker(){}
		virtual ~StarMarker(){}
	};
}

#endif //__AUGE_MARKER_SYMBOL_H__
