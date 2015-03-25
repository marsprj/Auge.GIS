#ifndef __AUGE_ENUM_MAP_IMPL_H__
#define __AUGE_ENUM_MAP_IMPL_H__

#include "AugeCarto.h"
#include <vector>

namespace auge
{
	class EnumMapImpl : public EnumMap
	{
	public:
		EnumMapImpl();
		virtual ~EnumMapImpl();

	public:
		virtual Map*	Next();
		virtual void	Reset();
		virtual void	Release();

	public:
		void	Add(Map* pMap);
		void	Cleanup();

	private:
		std::vector<Map*>	m_maps;
		std::vector<Map*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_MAP_IMPL_H__
