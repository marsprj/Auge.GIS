#ifndef __AUGE_ENUM_COLOR_MAP_IMPL_H__
#define __AUGE_ENUM_COLOR_MAP_IMPL_H__

#include "AugeStyle.h"
#include <vector>

namespace auge
{
	class ColorMap;

	class EnumColorMapImpl : public EnumColorMap
	{
	public:
		EnumColorMapImpl();
		virtual ~EnumColorMapImpl();
	public:
		virtual	void			Reset();
		virtual ColorMap*		Next();
		virtual void			Release();
	public:
		void	Add(ColorMap* pColorMap);
	private:
		void	Cleanup();
	private:
		std::vector<ColorMap*>	m_maps;
		std::vector<ColorMap*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_COLOR_MAP_IMPL_H__
