#include "AugeStyle.h"
#include "PointSymbolizerImpl.h"

#include "stdlib.h"
#include "string.h"

namespace auge
{
	PointSymbolizerImpl::PointSymbolizerImpl():
	m_pStroke(NULL),
	m_pFill(NULL),
	m_pGraphic(NULL),
	m_opacity(0.0f),
	m_size(1.0f),
	m_rotation(0.0f),
	m_mark_type(augeMarkerCircle)
{
	//m_pMarker = NULL;
}

PointSymbolizerImpl::~PointSymbolizerImpl()
{
	if(m_pStroke!=NULL)
	{
		m_pStroke->Release();
		m_pStroke = NULL;
	}
	if(m_pFill!=NULL)
	{
		m_pFill->Release();
		m_pFill = NULL;
	}
	if(m_pGraphic!=NULL)
	{
		m_pGraphic->Release();
	}

	//if(m_pMarker!=NULL)
	//{
	//	AUGE_SAFE_RELEASE(m_pMarker);
	//}
}

//////////////////////////////////////////////////////////////////////////
const char*	PointSymbolizerImpl::GetGeometry()
{
	return m_geometry_prop_name.c_str();
}

void PointSymbolizerImpl::SetGeometry(const char* prop_name)
{
	if(prop_name==NULL)
	{
		m_geometry_prop_name.clear();
	}
	m_geometry_prop_name = prop_name;
}

void PointSymbolizerImpl::SetMarkType(augeMarkerType type)
{
	m_mark_type = type;
}

augeMarkerType PointSymbolizerImpl::GetMarkType()
{
	return m_mark_type;
}

void PointSymbolizerImpl::SetGraphic(Graphic* pGraphic)
{
	if(m_pGraphic!=NULL)
	{
		m_pGraphic->Release();
		m_pGraphic = NULL;
	}
	m_pGraphic = pGraphic;
}

Graphic* PointSymbolizerImpl::GetGraphic()
{
	return m_pGraphic;
}

void PointSymbolizerImpl::SetStroke(Stroke* pStroke)
{
	if(m_pStroke!=NULL)
	{
		m_pStroke->Release();
		m_pStroke = NULL;
	}
	m_pStroke = pStroke;
}

Stroke* PointSymbolizerImpl::GetStroke()
{
	return m_pStroke;
}

void PointSymbolizerImpl::SetFill(Fill* pFill)
{
	if(m_pFill!=NULL)
	{
		m_pFill->Release();
		m_pFill = NULL;
	}
	m_pFill = pFill;
}

Fill*	PointSymbolizerImpl::GetFill()
{
	return m_pFill;
}

void PointSymbolizerImpl::SetOpacity(float opacity)
{
	m_opacity = opacity;
}

float PointSymbolizerImpl::GetOpacity()
{
	return m_opacity;
}

void PointSymbolizerImpl::SetSize(float size)
{
	m_size = size;
}

float PointSymbolizerImpl::GetSize()
{
	return m_size;
}

void PointSymbolizerImpl::SetRotation(float rotation)
{
	m_rotation = rotation;
}

float PointSymbolizerImpl::GetRotation()
{
	return m_rotation;
}

void PointSymbolizerImpl::Release()
{
	if(!ReleaseRef())
	{
		delete this;
	}
}

augeSymbolizerType PointSymbolizerImpl::GetType()
{
	return augeSymbolPoint;
}

//void PointSymbolizerImpl::SetMarker(MarkerSymbol* pMarker)
//{
//	AUGE_SAFE_RELEASE(m_pMarker);
//	m_pMarker = pMarker;
//}
//
//MarkerSymbol* PointSymbolizerImpl::GetMarker()
//{
//	return m_pMarker;
//}

}

