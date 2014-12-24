#include "AugeStyle.h"
#include "StrokeImpl.h"

namespace auge
{

	StrokeImpl::StrokeImpl():
//m_color(0L),
m_width(1.0f),
	m_opacity(1.0f),
	m_line_cap(augeButtCap),
	m_line_join(augeMiterJoin),
	m_dash_offset(0.0f),
	m_pGraphicFill(NULL),
	m_pGraphicStroke(NULL)
{

}

// StrokeImpl::StrokeImpl(StrokeImpl& other):
// m_color(other.GetColor()),
// m_width(other.GetWidth()),
// m_opacity(other.GetOpacity()),
// m_line_cap(other.GetLinecap()),
// m_line_join(other.GetLinejoin()),
// m_pGraphicFill(NULL),
// m_pGraphicStroke(NULL)
// {
// 
// }

StrokeImpl::StrokeImpl(GColor const& c, float width/*=1.0f*/):
m_color(c),
	m_width(width)
{

}


StrokeImpl::~StrokeImpl()
{

}


//////////////////////////////////////////////////////////////////////////
GColor& StrokeImpl::GetColor()
{
	return m_color;
}

void StrokeImpl::SetColor(GColor& color)
{
	m_color = color;
}

augeLineCap	StrokeImpl::GetLinecap()
{
	return m_line_cap;
}

void StrokeImpl::SetLinecap(augeLineCap linecap)
{
	m_line_cap = linecap;
}

augeLineJoin StrokeImpl::GetLinejoin()
{
	return m_line_join;
}

void StrokeImpl::SetLinejoin(augeLineJoin linejoin)
{
	m_line_join = linejoin;
}

float StrokeImpl::GetOpacity()
{
	return m_opacity;
}

void StrokeImpl::SetOpacity(float opacity)
{
	m_opacity = opacity;
}

float StrokeImpl::GetWidth()
{
	return m_width;
}

void StrokeImpl::SetWidth(float width)
{
	m_width = width;
}

float StrokeImpl::GetDashoffset()
{
	return m_dash_offset;
}

void StrokeImpl::SetDashoffset(float dash_offset)
{
	m_dash_offset;
}

Stroke& StrokeImpl::operator=(const Stroke& rhs)
{
	StrokeImpl& other = (StrokeImpl&)rhs;
	m_color = other.GetColor();
	m_width = other.GetWidth();
	m_opacity = other.GetOpacity();
	m_line_cap = other.GetLinecap();
	m_line_join = other.GetLinejoin();

	return *this;
}

GraphicFill* StrokeImpl::GetGraphicFill()
{
	return NULL;
}

void StrokeImpl::SetGraphicFill(GraphicFill* pGraphicFill)
{
	m_pGraphicFill = pGraphicFill;
}

GraphicStroke* StrokeImpl::GetGraphicStroke()
{
	return m_pGraphicStroke;
}

void StrokeImpl::SetGraphicStroke(GraphicStroke* pGraphicStroke)
{
	m_pGraphicStroke = pGraphicStroke;
}

void StrokeImpl::Release()
{
	delete this;
}

}
