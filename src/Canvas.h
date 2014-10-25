#ifndef __AUGE_CANVAS_H__
#define __AUGE_CANVAS_H__

#include "Base.h"
#include "Envelope.h"

namespace auge
{
	class Map;
	class Viewer;
	class Renderer;

	class AUGE_API Canvas : public Base
	{
	public:
		Canvas(Map*pMap, uint width, uint height);
		virtual ~Canvas();

	public:
		int		Draw(Envelope& viewer);
		int		Save(const char* path);

	private:
		uint		m_width;
		uint		m_height;

		Map			*m_pMap;
		Renderer	*m_pRenderer;
	};

}//namespace

#endif //__AUGE_CANVAS_H__