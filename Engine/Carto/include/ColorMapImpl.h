#ifndef __AUGE_COLOR_MAP_IMPL_H__
#define __AUGE_COLOR_MAP_IMPL_H__

#include "AugeStyle.h"

namespace auge
{
	class ColorMapImpl : public ColorMap
	{
	public:
		ColorMapImpl();
		virtual ~ColorMapImpl();
	public:
		virtual	g_uint			Count();
		virtual	GColor*			GetColor(g_uint i);
		virtual void			Release();

	public:
		bool	Create(g_uint count);

	private:
		GColor*	m_pColors;
		g_uint	m_count;
	};

}

#endif //__AUGE_COLOR_MAP_IMPL_H__
