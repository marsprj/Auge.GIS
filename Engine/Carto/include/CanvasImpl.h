#ifndef __AUGE_CANVAS_IMPL_H__
#define __AUGE_CANVAS_IMPL_H__

#include "AugeCarto.h"
#include "TransformImpl.h"
#include <vector>

namespace auge
{
	class CanvasImpl : public Canvas
	{
	public:
		CanvasImpl(g_uint width, g_uint height);
		virtual ~CanvasImpl();
	public:
		virtual void			SetViewer(GEnvelope& viewer);
		virtual void			DrawBackground(GColor& color);

		virtual void			Draw(Map*pMap);
		virtual void			DrawLayer(Layer* pLayer);
		virtual void			DrawLayer(FeatureLayer* pLayer);
		virtual void			DrawLayer(RasterLayer* pLayer);
		virtual void			DrawLayer(Layer* pLayer, Style* pStyle);
		virtual void			DrawLayer(FeatureLayer* pLayer, FeatureStyle* pStyle);

		virtual Transformation*	GetTransform();
		virtual Renderer*		GetRenderer();

		virtual RESULTCODE		Save(const char* path);

	private:
		void		DrawLayer(FeatureLayer* pLayer, Rule* pRule);
		void		DrawLayer(FeatureLayer* pLayer, Symbolizer* pSymbolizer, GFilter* pFilter);

		void		LabelLayer(FeatureLayer* pLayer, TextSymbolizer* pSymbolizer, GFilter* pFilter);
		void		LabelGeometry(g_uchar* wkb, TextSymbolizer* pSymbolizer, const char* text);
		void		LabelPoint(WKBPoint* pWKBPoint, TextSymbolizer* pSymbolizer,const char* text);

	private:
		void		Cleanup();

	private:
		g_uint			m_width;
		g_uint			m_height;
		GEnvelope		m_viewer;
		Renderer		*m_pRenderer;
		TransformationImpl	m_transform;
	};	
}

#endif //__AUGE_CANVAS_IMPL_H__
