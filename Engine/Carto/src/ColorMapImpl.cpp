#include "AugeStyle.h"
#include "ColorMapImpl.h"

namespace auge
{

	ColorMapImpl::ColorMapImpl():
m_pColors(NULL),
	m_count(0)
{

}

ColorMapImpl::~ColorMapImpl()
{
	if(m_pColors!=NULL)
	{
		delete[] m_pColors;
		m_pColors = NULL;
	}
}

g_uint ColorMapImpl::Count()
{
	return m_count;
}

GColor* ColorMapImpl::GetColor(g_uint i)
{
	if(i>=m_count)
	{
		return NULL;
	}
	return m_pColors + i;
}

void ColorMapImpl::Release()
{
	delete this;
}

bool ColorMapImpl::Create(g_uint count)
{
	if(m_pColors!=NULL)
	{
		delete[] m_pColors;
		m_pColors = NULL;
	}
	if(count==0)
	{
		return false;
	}
	m_pColors = new GColor[count];
	m_count = count;
	return true;
}

}
