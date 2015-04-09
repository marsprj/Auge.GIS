#ifndef __AUGE_EXTERN_GRAPHIC_IMPL_H__
#define __AUGE_EXTERN_GRAPHIC_IMPL_H__

#include "AugeStyle.h"
#include <string>

namespace auge
{
	class ExternalGraphicImpl : public ExternalGraphic
	{
	public:
		ExternalGraphicImpl();
		virtual ~ExternalGraphicImpl();
	public:
		virtual const char* GetResource();
		virtual void		SetResource(const char* resource);

		virtual const char*	GetFormat();
		virtual void		SetFormat(const char* format);

	private:
		std::string m_resource;
		std::string m_format;
	};
}

#endif //__AUGE_EXTERN_GRAPHIC_IMPL_H__