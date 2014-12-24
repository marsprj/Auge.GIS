#include "AugeStyle.h"
#include "FontImpl.h"
#include "string.h"

namespace auge
{

	FontImpl::FontImpl():
m_style(augeFontStyleNormal),
	m_weght(augeFontWeightNormal),
	m_size(10.0f)
{

}

FontImpl::~FontImpl()
{

}

void FontImpl::Release()
{
	delete this;
}
//////////////////////////////////////////////////////////////////////////
const char*	FontImpl::GetFamily()
{
	return m_family.c_str();
}

bool FontImpl::SetFamily(const char* szFamily)
{
	if(szFamily==NULL)
		return false;

	m_family = szFamily;

	return true;
}

augeFontStyle FontImpl::GetStyle()
{
	return m_style;
}

void FontImpl::SetStyle(augeFontStyle style)
{
	m_style = style;
}

augeFontWeight FontImpl::GetWeight()
{
	return m_weght;
}

void FontImpl::SetWeight(augeFontWeight weight)
{
	m_weght = weight;
}

float FontImpl::GetSize()
{
	return m_size;
}

void FontImpl::SetSize(float size)
{
	m_size = size;
}

}