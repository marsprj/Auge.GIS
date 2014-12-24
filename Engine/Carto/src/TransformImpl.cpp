#include "TransformImpl.h"
#include "math.h"

namespace auge
{
	TransformationImpl::TransformationImpl():
	m_canvas_width(1),
		m_canvas_height(1),
		m_scale(1.0f),
		m_canvas_cx(0.5f),
		m_canvas_cy(0.5f),
		m_viewer_cx(0.0f),
		m_viewer_cy(0.0f)
	{

	}

	TransformationImpl::~TransformationImpl()
	{

	}

	void TransformationImpl::ToScreenPoint(double mapX, double mapY, int& screenX, int& screenY)
	{
		screenX = (int)floor(m_scale * (mapX - m_viewer_cx) + m_canvas_cx + 0.5f);
		screenY = (int)floor(m_canvas_cy - m_scale * (mapY - m_viewer_cy) + 0.5f);
	}

	void TransformationImpl::ToMapPoint(int screenX, int screenY, double& mapX, double mapY)
	{
		mapX = ((screenX - m_canvas_cx) / m_scale) + m_viewer_cx;
		mapY = ((m_canvas_cy-  screenY) / m_scale) + m_viewer_cy;
	}

	void TransformationImpl::SetCanvas(int width, int height)
	{
		m_canvas_width  = width;
		m_canvas_height = height;

		m_canvas_cx = width / (2.0f);
		m_canvas_cy = height/ (2.0f);
	}

	void TransformationImpl::SetViewer(GEnvelope& viewer)
	{
		m_viewer = viewer;

		double scale_x = m_canvas_width / m_viewer.GetWidth();
		double sacle_y = m_canvas_height / m_viewer.GetHeight();
		m_scale = scale_x < sacle_y ? scale_x : sacle_y;

		m_viewer_cx = (viewer.m_xmin + viewer.m_xmax) / 2.0f;
		m_viewer_cy = (viewer.m_ymin + viewer.m_ymax) / 2.0f;
	}

	GEnvelope& TransformationImpl::GetViewer()
	{
		return m_viewer;
	}
}
