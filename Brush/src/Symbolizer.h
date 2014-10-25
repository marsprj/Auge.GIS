#ifndef __AUGE_SYMBOLIZER_H__
#define __AUGE_SYMBOLIZER_H__

#include "Base.h"

namespace auge
{
	//========================================================================
	// augeSymbolType Enum
	//========================================================================
	typedef enum SymbolizerType
	{
		augeSymbolNone		= 0,
		augeSymbolPoint		= 1,
		augeSymbolLine		= 2,
		augeSymbolPolygon	= 3,
		augeSymbolText		= 4,
		augeSymbolRaster	= 5
	}SymbolizerType;

	typedef enum LineCap
	{
		augeButtCap		= 0,
		augeSquareCap	= 1,
		augeRoundCap	= 2,
		augeLineCapMAX	= 3
	}LineCap; 

	//========================================================================
	// augeLineJoin Enum
	//========================================================================
	typedef enum LineJoin
	{
		augeMiterJoin		= 0,
		augeMiterRevertJoin	= 1,
		augeRoundJoin		= 2,
		augeBevelJoin		= 3,
		augeLineJoinMax		= 4
	}LineJoin;

	//========================================================================
	// augeMarkerType Enum
	//========================================================================
	typedef enum MarkerType
	{
		augeSquare			= 0,
		augeCircle			= 1,
		augeTriangle		= 2,
		augeStar			= 3,
		augeCross			= 4,
		augeX				= 5
	}MarkerType;
		
	class AUGE_API Symbolizer : public Base
	{
	public:
		Symbolizer();
		virtual ~Symbolizer();

	public:
		virtual	SymbolizerType	GetType(){return augeSymbolNone;}
	};

}//namespace

#endif //__AUGE_SYMBOLIZER_H__
