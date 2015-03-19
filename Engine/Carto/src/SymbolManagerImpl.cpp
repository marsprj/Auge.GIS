#include "SymbolManagerImpl.h"

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

	GSymbol* SymbolManagerImpl::GetSymbol(g_uint id)
	{
		return NULL;
	}
}