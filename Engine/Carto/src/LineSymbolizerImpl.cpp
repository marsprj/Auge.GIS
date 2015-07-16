#include "AugeStyle.h"
#include "string.h"
#include "LineSymbolizerImpl.h"

namespace auge
{

	LineSymbolizerImpl::LineSymbolizerImpl():
	m_perpendicular_offset(0.0f)
	{
		m_line_type = augeLineSimple;
		m_pSymbol = NULL;
	}

	LineSymbolizerImpl::~LineSymbolizerImpl()
	{	
		// 	if(m_stroke!=NULL)
		// 	{
		// 		m_stroke->Release();
		// 		m_stroke = NULL;
		// 	}

		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	const char* LineSymbolizerImpl::GetGeometry()
	{
		return m_geometry_prop_name.c_str();
	}

	Stroke* LineSymbolizerImpl::GetStroke()
	{
		return &m_stroke;
	}

	bool LineSymbolizerImpl::SetGeometry(const char* prop_name)
	{
		if(prop_name==NULL)
		{
			m_geometry_prop_name.clear();
		}

		m_geometry_prop_name.c_str();

		return true;
	}

	bool LineSymbolizerImpl::SetStroke(Stroke* pStroke)
	{
		if(pStroke!=NULL)
		{
			m_stroke = *pStroke;
		}
		return true;
	}

	void LineSymbolizerImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType LineSymbolizerImpl::GetType()
	{
		return augeSymbolLine;
	}

	void LineSymbolizerImpl::SetLineType(augeLineType type)
	{
		m_line_type = type;
	}

	augeLineType LineSymbolizerImpl::GetLineType()
	{
		return m_line_type;
	}

	void LineSymbolizerImpl::SetSymbol(LineSymbol* pSymbol)
	{
		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
		m_pSymbol = pSymbol;
	}

	LineSymbol* LineSymbolizerImpl::GetSymbol()
	{
		return m_pSymbol;
	}

}
