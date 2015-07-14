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

	Symbolizer*	EnumSymbolImpl::Next()
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
		Symbolizer* pSymbol = NULL;
		std::vector<Symbolizer*>::iterator iter;
		for(iter=m_symbols.begin(); iter!=m_symbols.end(); iter++)
		{
			pSymbol = (*iter);
			pSymbol->Release();
		}
		m_symbols.clear();
	}
}