#ifndef __AUGE_COLOR_MAP_IMPL_H__
#define __AUGE_COLOR_MAP_IMPL_H__

#include "AugeStyle.h"
#include <string>

namespace auge
{
	class ColorMapImpl : public ColorMap
	{
	public:
		ColorMapImpl();
		virtual ~ColorMapImpl();
	public:
		virtual	g_uint			GetCount();
		virtual bool			SetCount(g_uint count);
		virtual	GColor*			GetColor(g_uint i);
		virtual GColor&			GetStartColor();
		virtual GColor&			GetEndColor();
		virtual const char*		GetImagePath();
		virtual const char*		GetAbsoluteImagePath();
		virtual void			Release();

	public:
		bool	Create(g_uint id, GColor start, GColor end);
		bool	Create(g_uint id, GColor start, GColor end, const char* img_path, const char* abs_img_path);
		//bool	Create(g_uint count, GColor start, GColor end);

		void	SetImagePath(const char* path);
		void	SetAbsoluteImagePath(const char* path);

	private:
		GColor*	m_pColors;
		g_uint	m_count;
		std::string m_image_path;
		std::string m_absolute_image_path;
		GColor	m_start_color;
		GColor	m_end_color;
	};

}

#endif //__AUGE_COLOR_MAP_IMPL_H__
