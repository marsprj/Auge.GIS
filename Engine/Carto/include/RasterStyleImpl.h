#ifndef __AUGE_RASTER_STYLE_IMPL_H__
#define __AUGE_RASTER_STYLE_IMPL_H__

#include "AugeStyle.h"
#include <string>

namespace auge
{
	class RasterStyleImpl : public RasterStyle
	{
	public:
		RasterStyleImpl();
		virtual ~RasterStyleImpl();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* name);
		virtual augeStyleType	GetType();
		virtual void			Release();

	private:
		std::string	m_name;
	};
}


#endif //__AUGE_RASTER_STYLE_IMPL_H__