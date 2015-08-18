#include "AugeFilter.h"

#include "RuleImpl.h"

namespace auge
{
	RuleImpl::RuleImpl():
	m_pLegendGraphic(NULL),
	m_pSymbolizer(NULL),
	m_pTextSymbolizer(NULL),
	m_pFilter(NULL),
	m_min_scale(0.0f),
	m_max_scale(0.0f),
	m_limit(0)
	{
	}

	RuleImpl::~RuleImpl()
	{
		if(m_pLegendGraphic!=NULL)
		{
			m_pLegendGraphic->Release();
			m_pLegendGraphic = NULL;
		}
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

	//////////////////////////////////////////////////////////////////////////

	const char*	RuleImpl::GetName()
	{
		return m_strName.c_str();
	}

	void RuleImpl::SetName(const char* szName)
	{
		if(szName!=NULL)
		{
			m_strName = szName;
		}
	}

	const char*	RuleImpl::GetTitle()
	{
		return m_strTitle.c_str();
	}

	void RuleImpl::SetTitle(const char* szTitle)
	{
		if(szTitle!=NULL)
		{
			m_strTitle = szTitle;
		}
	}

	const char*	RuleImpl::GetAbstract()
	{
		return m_strAbstract.c_str();
	}

	void RuleImpl::SetAbstract(const char* szAbstract)
	{
		if(szAbstract!=NULL)
		{
			m_strAbstract = szAbstract;
		}
	}

	Graphic* RuleImpl::GetLegendGraphic()
	{
		return m_pLegendGraphic;
	}

	void RuleImpl::SetLegendGraphic(Graphic* pagGraphic)
	{
		if(m_pLegendGraphic!=NULL)
		{
			m_pLegendGraphic->Release();
			m_pLegendGraphic = NULL;
		}
		m_pLegendGraphic = pagGraphic;
	}

	Symbolizer* RuleImpl::GetSymbolizer()
	{
		return m_pSymbolizer;
	}

	void RuleImpl::SetSymbolizer(Symbolizer *pSymbolizer)
	{
		if(m_pSymbolizer!=NULL)
		{
			m_pSymbolizer->Release();
			m_pSymbolizer = NULL;
		}
		m_pSymbolizer = pSymbolizer;
	}

	TextSymbolizer*	RuleImpl::GetTextSymbolizer()
	{
		return m_pTextSymbolizer;
	}

	void RuleImpl::SetTextSymbolizer(TextSymbolizer *pSymbolizer)
	{
		if(m_pTextSymbolizer!=NULL)
		{
			m_pTextSymbolizer->Release();
			m_pTextSymbolizer = NULL;
		}
		m_pTextSymbolizer = pSymbolizer;
	}

	double RuleImpl::GetMinScale()
	{
		return m_min_scale;
	}

	void RuleImpl::SetMinScale(double scale)
	{
		m_min_scale = scale;
	}

	double RuleImpl::GetMaxScale()
	{
		return m_max_scale;
	}

	void RuleImpl::SetMaxScale(double scale)
	{
		m_max_scale = scale;
	}

	GFilter* RuleImpl::GetFilter()
	{
		return m_pFilter;
	}

	void RuleImpl::SetFilter(GFilter* pFilter)
	{
		if(m_pFilter!=NULL)
		{
			m_pFilter->Release();
			m_pFilter = NULL;
		}
		m_pFilter = pFilter;
	}

	void RuleImpl::SetLimit(g_uint limit)
	{
		m_limit = limit;
	}

	g_uint RuleImpl::GetLimit()
	{
		return m_limit;
	}

	void RuleImpl::Release()
	{
		delete this;
	}

}
