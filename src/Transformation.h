#ifndef __AUGE_TRANSFORMATION_H__
#define __AUGE_TRANSFORMATION_H__

#include "Base.h"
#include "Envelope.h"

namespace auge
{
	class Transformation
	{
		friend class Renderer;
	public:
		Transformation();
		virtual ~Transformation();

	public:
		void		ToScreenPoint(double mapX, double mapY, int& screenX, int& screenY);
		void		ToMapPoint(int screenX, int screenY, double& mapX, double mapY);
		
		void		SetCanvas(int width, int height);
		void		SetViewer(Envelope& viewer);

		Envelope&	GetViewer();
	private:
		double		m_scale;
		Envelope	m_viewer;
		int			m_canvas_width;
		int			m_canvas_height;

		double		m_canvas_cx;
		double		m_canvas_cy;
		double		m_viewer_cx;
		double		m_viewer_cy;
	};
}

#endif //__AUGE_TRANSFORMATION_H__
