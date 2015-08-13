#include "stdlib.h"
#include "AugeStyle.h"
#include "PolygonSymbolizerImpl.h"

namespace auge
{

	PolygonSymbolizerImpl::PolygonSymbolizerImpl():
	m_pFill(NULL),
	m_pStroke(NULL),
	m_opacity(0.0f),
	m_displacement_X(0.0f),
	m_displacement_Y(0.0f),
	m_perpendicular_offset(0),
	m_geometry_prop_name(NULL)
	{
		m_pSymbol = NULL;
	}

	PolygonSymbolizerImpl::~PolygonSymbolizerImpl()
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		if(m_geometry_prop_name!=NULL)
		{
			free((void*)m_geometry_prop_name);
			m_geometry_prop_name = NULL;
		}

		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	Fill*	PolygonSymbolizerImpl::GetFill()
	{
		return m_pFill;
	}

	void PolygonSymbolizerImpl::SetFill(Fill* pFill)
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		m_pFill = pFill;
	}

	Stroke* PolygonSymbolizerImpl::GetStroke()
	{
		return m_pStroke;
	}

	void PolygonSymbolizerImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	void PolygonSymbolizerImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float PolygonSymbolizerImpl::GetOpacity()
	{
		return m_opacity;
	}

	float PolygonSymbolizerImpl::GetDisplacement_X()
	{
		return m_displacement_X;
	}

	float PolygonSymbolizerImpl::GetDisplacement_Y()
	{
		return m_displacement_Y;
	}

	void PolygonSymbolizerImpl::SetDisplacement(float x, float y)
	{
		m_displacement_X = x;
		m_displacement_Y = y;
	}

	const char*	PolygonSymbolizerImpl::GetGemetry()
	{
		return m_geometry_prop_name;
	}

	void PolygonSymbolizerImpl::SetGeometry(const char* prop_name)
	{
		if(m_geometry_prop_name!=NULL)
		{
			free((void*)m_geometry_prop_name);
			m_geometry_prop_name = NULL;
		}
		m_geometry_prop_name = prop_name;
	}

	void PolygonSymbolizerImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType PolygonSymbolizerImpl::GetType()
	{
		return augeSymbolPolygon;
	}

	void PolygonSymbolizerImpl::SetSymbol(FillSymbol* pSymbol)
	{
		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
		m_pSymbol = pSymbol;
	}

	FillSymbol* PolygonSymbolizerImpl::GetSymbol()
	{
		return m_pSymbol;
	}
}
