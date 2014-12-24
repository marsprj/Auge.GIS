#include "RasterStyleImpl.h"

namespace auge
{
	RasterStyleImpl::RasterStyleImpl()
	{

	}

	RasterStyleImpl::~RasterStyleImpl()
	{

	}

	void RasterStyleImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeStyleType RasterStyleImpl::GetType()
	{
		return augeStyleRaster;
	}

	const char*	RasterStyleImpl::GetName()
	{
		return m_name.c_str();
	}

	void RasterStyleImpl::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			m_name.clear();
		}
		m_name = szName;
	}
}