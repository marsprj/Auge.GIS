#include "AugeStyle.h"
#include "string.h"
#include "LineSymbolizerImpl.h"

namespace auge
{

	LineSymbolizerImpl::LineSymbolizerImpl():
m_perpendicular_offset(0.0f)
{

}

LineSymbolizerImpl::~LineSymbolizerImpl()
{	
	// 	if(m_stroke!=NULL)
	// 	{
	// 		m_stroke->Release();
	// 		m_stroke = NULL;
	// 	}
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
	delete this;
}

augeSymbolizerType LineSymbolizerImpl::GetType()
{
	return augeSymbolLine;
}

}
