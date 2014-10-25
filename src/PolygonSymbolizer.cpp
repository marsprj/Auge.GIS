#include "Symbolizer.h"
#include "PolygonSymbolizer.h"

namespace auge
{

	PolygonSymbolizer::PolygonSymbolizer():
	fill(true),
	outline(true),
	radius(5.0f),
	outline_width(1.0f),
	outline_cap(augeRoundCap),
	outline_join(augeRoundJoin)
	{

	}

	PolygonSymbolizer::~PolygonSymbolizer()
	{

	}


}