#include "stdlib.h"
#include "Rule.h"
#include "PointSymbolizer.h"
#include "LineSymbolizer.h"
#include "PolygonSymbolizer.h"
#include "TextSymbolizer.h"
#include "Filter.h"

namespace auge
{

	Rule::Rule():
	m_pSymbolizer(NULL),
	m_pTextSymbolizer(NULL),
	m_pFilter(NULL)
	{

	}

	Rule::~Rule()
	{
		if(m_pSymbolizer!=NULL)
		{
			m_pSymbolizer->Release();
			m_pSymbolizer = NULL;
		}
		if(m_pTextSymbolizer!=NULL)
		{
			m_pTextSymbolizer->Release();
			m_pTextSymbolizer = NULL;
		}
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
	}

	Symbolizer*	Rule::GetSymbolizer()
	{
		return m_pSymbolizer;
	}

	void Rule::SetSymbolizer(Symbolizer *pSymbolizer)
	{
		m_pSymbolizer = pSymbolizer;
	}

	TextSymbolizer*	Rule::GetTextSymbolizer()
	{
		return m_pTextSymbolizer;
	}

	void Rule::SetTextSymbolizer(TextSymbolizer *pSymbolizer)
	{
		m_pTextSymbolizer = pSymbolizer;
	}

	Filter*	Rule::GetFilter()
	{
		return m_pFilter;
	}

	void Rule::SetFilter(Filter* pFilter)
	{
		if(pFilter==m_pFilter)
		{
			return;
		}
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
		m_pFilter = pFilter;
	}
}