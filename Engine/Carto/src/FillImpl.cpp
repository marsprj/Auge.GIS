#include "AugeStyle.h"
#include "FillImpl.h"

namespace auge
{


	FillImpl::FillImpl():
m_fill_opacity(150),
	m_stroke_opacity(255)
{

}

FillImpl::~FillImpl()
{

}

//////////////////////////////////////////////////////////////////////////

void FillImpl::Release()
{
	delete this;
}

//////////////////////////////////////////////////////////////////////////
GColor& FillImpl::GetColor()
{
	return m_color;
}

void FillImpl::SetColor(GColor& color)
{
	m_color.Set(color);
}

void FillImpl::SetOpacity(float opacity)
{
	m_fill_opacity = opacity;
}

float FillImpl::GetOpacity()
{
	return m_fill_opacity;
}

}