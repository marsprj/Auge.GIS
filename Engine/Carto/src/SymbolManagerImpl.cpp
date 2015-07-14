#include "SymbolManagerImpl.h"
#include "EnumSymbolImpl.h"

#include "CapitalMarkerImpl.h"
#include "CircleMarkerImpl.h"
#include "PentagonMarkerImpl.h"
#include "StarMarkerImpl.h"
#include "SquareMarkerImpl.h"
#include "TriangleMarkerImpl.h"

namespace auge
{
	SymbolManager* augeGetSymbolManagerInstance()
	{
		static SymbolManagerImpl g_symbolManger;
		return &g_symbolManger;
	}

	SymbolManagerImpl::SymbolManagerImpl()
	{

	}

	SymbolManagerImpl::~SymbolManagerImpl()
	{

	}

	EnumSymbol*	SymbolManagerImpl::GetMarkerSymbols()
	{
		return NULL;
	}

	EnumSymbol*	SymbolManagerImpl::GetLineSymbols()
	{
		return NULL;
	}

	EnumSymbol*	SymbolManagerImpl::GetRegionSymbols()
	{
		return NULL;
	}

	MarkerSymbol* SymbolManagerImpl::CreateMarkerSymbol(augeMarkerType type)
	{
		MarkerSymbol* pSymbol = NULL;
		switch(type)
		{
		case augeMarkerSquare:
			pSymbol = new SquareMarkerImpl();
			break;
		case augeMarkerCircle:
			pSymbol = new CircleMarkerImpl();
			break;
		case augeMarkerTriangle:
			pSymbol = new TriangleMarkerImpl();
			break;
		case augeMarkerStar:
			pSymbol = new StarMarkerImpl();
			break;
		case augeMarkerCross:
			break;
		case augeMarkerX:
			break;
		case augeMarkerPentagon:
			pSymbol = new PentagonMarkerImpl();
			break;
		case augeMarkerCapital:
			pSymbol = new CapitalMarkerImpl();
			break;
		default:
			break;
		}
		return pSymbol;
	}
}