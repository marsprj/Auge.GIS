#include "EnumSymbolImpl.h"

namespace auge
{
	EnumSymbolImpl::EnumSymbolImpl()
	{
		m_iter = m_symbols.begin();
	}

	EnumSymbolImpl::~EnumSymbolImpl()
	{
		Cleanup();
	}

	void EnumSymbolImpl::Reset()
	{
		m_iter = m_symbols.begin();
	}

	Symbol*	EnumSymbolImpl::Next()
	{
		if(m_iter==m_symbols.end())
		{
			return NULL;
		}
		return (*m_iter++);
	}

	void EnumSymbolImpl::Release()
	{
		delete this;
	}

	void EnumSymbolImpl::Cleanup()
	{
		Symbol* pSymbol = NULL;
		std::vector<Symbol*>::iterator iter;
		for(iter=m_symbols.begin(); iter!=m_symbols.end(); iter++)
		{
			pSymbol = (*iter);
			pSymbol->Release();
		}
		m_symbols.clear();
	}

	void EnumSymbolImpl::Add(Symbol* pSymbol)
	{
		if(pSymbol!=NULL)
		{
			m_symbols.push_back(pSymbol);
		}
	}
}