#include "GraphicImpl.h"

namespace auge
{
	GraphicImpl::GraphicImpl():
m_opacity(0.0f),
	m_size(0.0f),
	m_rotation(0.0f),
	m_anchor_x(0.0f),
	m_anchor_y(0.0f),
	m_displacement_x(0.0f),
	m_displacement_y(0.0f),
	m_pMark(NULL),
	m_pExternalGraphic(NULL)
	{

	}

	GraphicImpl::~GraphicImpl()
	{
		if(m_pMark!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pMark);
		}
		if(m_pExternalGraphic!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pExternalGraphic);
		}
	}

	float GraphicImpl::GetOpacity()
	{
		return m_opacity;
	}

	void GraphicImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	int GraphicImpl::GetSize()
	{
		return m_size;
	}

	void GraphicImpl::SetSize(int size)
	{
		m_size = size;
	}

	float GraphicImpl::GetRotation()
	{
		return m_rotation;
	}

	void GraphicImpl::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float GraphicImpl::GetAnchor_X()
	{
		return m_anchor_x;
	}

	float GraphicImpl::GetAnchor_Y()
	{
		return m_anchor_y;
	}

	void GraphicImpl::SetAnchor(float x, float y)
	{
		m_anchor_x = x;
		m_anchor_y = y;
	}


	float GraphicImpl::GetDisplacement_X()
	{
		return m_displacement_x;
	}

	float GraphicImpl::GetDisplacement_Y()
	{
		return m_displacement_y;
	}

	void GraphicImpl::SetDisplacement(float x, float y)
	{
		m_displacement_x = x;
		m_displacement_y = y;
	}


	Mark* GraphicImpl::GetMark()
	{
		return m_pMark;
	}

	bool GraphicImpl::SetMark(Mark* pMark)
	{
		if(m_pMark!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pMark);
		}
		m_pMark = pMark;
		return true;
	}

	ExternalGraphic* GraphicImpl::GetExternalGraphic()
	{
		return m_pExternalGraphic;
	}

	bool GraphicImpl::SetExternalGraphic(ExternalGraphic* pExternalGraphic)
	{
		if(m_pExternalGraphic!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pExternalGraphic);
		}
		m_pExternalGraphic = pExternalGraphic;
		return true;
	}


}