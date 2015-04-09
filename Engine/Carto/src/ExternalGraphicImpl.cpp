#include "ExternalGraphicImpl.h"

namespace auge
{
	ExternalGraphicImpl::ExternalGraphicImpl()
	{

	}

	ExternalGraphicImpl::~ExternalGraphicImpl()
	{

	}

	const char* ExternalGraphicImpl::GetResource()
	{
		return m_resource.c_str();
	}

	void ExternalGraphicImpl::SetResource(const char* resource)
	{
		if(!resource)
		{
			m_resource.clear();
		}
		else
		{
			m_resource = resource;
		}
	}

	const char*	ExternalGraphicImpl::GetFormat()
	{
		return m_format.c_str();
	}

	void ExternalGraphicImpl::SetFormat(const char* format)
	{
		if(!format)
		{
			m_format.clear();
		}
		else
		{
			m_format = format;
		}
	}
}