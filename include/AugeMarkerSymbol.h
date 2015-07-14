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

	class CapitalMarker : public MarkerSymbol
	{
	protected:
		CapitalMarker(){}
		virtual ~CapitalMarker(){}
	};

	class CircleMarker : public MarkerSymbol
	{
	protected:
		CircleMarker(){}
		virtual ~CircleMarker(){}
	};

	class PentagonMarker : public MarkerSymbol
	{
	protected:
		PentagonMarker(){}
		virtual ~PentagonMarker(){}
	};

	class SquareMarker : public MarkerSymbol
	{
	protected:
		SquareMarker(){}
		virtual ~SquareMarker(){}
	};

	class StarMarker : public MarkerSymbol
	{
	protected:
		StarMarker(){}
		virtual ~StarMarker(){}
	};

	class TriangleMarker : public MarkerSymbol
	{
	protected:
		TriangleMarker(){}
		virtual ~TriangleMarker(){}
	};
}

#endif //__AUGE_MARKER_SYMBOL_H__
