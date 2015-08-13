#ifndef __AUGE_MARKER_SYMBOL_H__
#define __AUGE_MARKER_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
	class SimpleMarker : public SimpleMarkerSymbol
	{
	protected:
		SimpleMarker(){}
		virtual ~SimpleMarker(){}
	};

	class CapitalMarker : public SimpleMarkerSymbol
	{
	protected:
		CapitalMarker(){}
		virtual ~CapitalMarker(){}
	};

	class CircleMarker : public SimpleMarkerSymbol
	{
	protected:
		CircleMarker(){}
		virtual ~CircleMarker(){}
	};

	class PentagonMarker : public SimpleMarkerSymbol
	{
	protected:
		PentagonMarker(){}
		virtual ~PentagonMarker(){}
	};

	class SquareMarker : public SimpleMarkerSymbol
	{
	protected:
		SquareMarker(){}
		virtual ~SquareMarker(){}
	};

	class StarMarker : public SimpleMarkerSymbol
	{
	protected:
		StarMarker(){}
		virtual ~StarMarker(){}
	};

	class TriangleMarker : public SimpleMarkerSymbol
	{
	protected:
		TriangleMarker(){}
		virtual ~TriangleMarker(){}
	};
}

#endif //__AUGE_MARKER_SYMBOL_H__
