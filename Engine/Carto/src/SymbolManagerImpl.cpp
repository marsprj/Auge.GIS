#include "SymbolManagerImpl.h"
#include "EnumSymbolImpl.h"

#include "StarMarkerImpl.h"

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
			break;
		case augeMarkerCircle:
			break;
		case augeMarkerTriangle:
			break;
		case augeMarkerStar:
			pSymbol = new StarMarkerImpl();
			break;
		case augeMarkerCross:
			break;
		case augeMarkerX:
			break;
		case augeMarkerPentagon:
			break;
		case augeMarkerCapital:
			break;
		default:
			break;
		}
		return pSymbol;
	}
}