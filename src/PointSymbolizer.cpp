#include "Symbolizer.h"
#include "PointSymbolizer.h"

namespace auge
{

	PointSymbolizer::PointSymbolizer():
	fill(true),
	outline(true),
	radius(5.0f),
	outline_width(1.0f),
	outline_cap(augeRoundCap),
	outline_join(augeRoundJoin)
	{

	}

	PointSymbolizer::~PointSymbolizer()
	{

	}

}