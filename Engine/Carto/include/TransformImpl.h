#ifndef __AUGE_TRANSFORMATION_IMPL_H__
#define __AUGE_TRANSFORMATION_IMPL_H__

#include "AugeCarto.h"

namespace auge
{
	class TransformationImpl : public Transformation
	{
	public:
		TransformationImpl();
		virtual ~TransformationImpl();

	public:
		void		ToScreenPoint(double mapX, double mapY, int& screenX, int& screenY);
		void		ToMapPoint(int screenX, int screenY, double& mapX, double mapY);

		void		SetCanvas(int width, int height);
		void		SetViewer(GEnvelope& viewer);

		GEnvelope&	GetViewer();
		virtual g_uint			GetCanvasWidth();
		virtual g_uint			GetCanvasHeight();
	private:
		double		m_scale;
		GEnvelope	m_viewer;
		int			m_canvas_width;
		int			m_canvas_height;

		double		m_canvas_cx;
		double		m_canvas_cy;
		double		m_viewer_cx;
		double		m_viewer_cy;
	};
}

#endif //__AUGE_TRANSFORMATION_IMPL_H__
