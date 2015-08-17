#include "GraphicFillSymbolImpl.h"
#include "RendererCairo.h"

namespace auge
{
	GraphicFillSymbolImpl::GraphicFillSymbolImpl():
	m_cairo(NULL),
	m_icon(NULL),	
	m_opacity(0.0f),
	m_size(20.0f),
	m_rotation(0.0f)
	{

	}

	GraphicFillSymbolImpl::~GraphicFillSymbolImpl()
	{
		if(m_cairo!=NULL)
		{
			cairo_destroy(m_cairo);
			m_cairo = NULL;
		}
		if(m_icon!=NULL)
		{
			cairo_surface_destroy(m_icon);
			m_cairo = NULL;
		}
	}

	const char*	GraphicFillSymbolImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	GraphicFillSymbolImpl::GetPath()
	{
		return m_icon_path.c_str();
	}

	bool GraphicFillSymbolImpl::IsGraphic()
	{
		return true;
	}

	void GraphicFillSymbolImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float GraphicFillSymbolImpl::GetOpacity()
	{
		return m_opacity;
	}

	void GraphicFillSymbolImpl::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}

	void GraphicFillSymbolImpl::SetPath(const char* icon_path)
	{
		if(icon_path==NULL)
		{
			m_icon_path.clear();
		}
		else
		{
			m_icon_path = icon_path;
		}
	}

	void GraphicFillSymbolImpl::SetFilePath(const char* file_path)
	{
		if(file_path==NULL)
		{
			m_file_path.clear();
		}
		else
		{
			m_file_path = file_path;
		}
	}

	void GraphicFillSymbolImpl::SetSize(float size)
	{
		m_size = size;
	}

	float GraphicFillSymbolImpl::GetSize()
	{
		return m_size;
	}

	void GraphicFillSymbolImpl::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float GraphicFillSymbolImpl::GetRotation()
	{
		return m_rotation;
	}

	const char*	GraphicFillSymbolImpl::GetIcon()
	{
		return m_icon_path.c_str();
	}

	augeSymbolizerType GraphicFillSymbolImpl::GetType()
	{
		return augeSymbolPolygon;
	}

	RESULTCODE GraphicFillSymbolImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
	{
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}
		if(pGeometry->GeometryType()!=augeGTPoint)
		{
			return AG_FAILURE;
		}
		if(m_size==0)
		{
			return AG_FAILURE;
		}

		RendererCairo* pRendererCairo = static_cast<RendererCairo*>(pRenderer);
		cairo_t			*canvas_cairo = pRendererCairo->GetCairo();
		cairo_surface_t	*canvas_surface=pRendererCairo->GetCairoSurface();

		int sx=0, sy=0;
		WKBPoint* pWKBPoint = (WKBPoint*)pGeometry->AsBinary();
		pTransform->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);

		//cairo_surface_t *image = cairo_image_surface_create_from_png(resource);
		if(m_cairo==NULL)
		{
			LoadSymbol();
		}

		if(m_cairo!=NULL)
		{
			cairo_save(canvas_cairo);
			cairo_translate(canvas_cairo, sx-m_size, sy-m_size);
			
			cairo_set_source_surface(canvas_cairo, m_icon, 0,0);
			cairo_paint(canvas_cairo);
			cairo_surface_flush(canvas_surface);
			cairo_restore(canvas_cairo);
		}

		return AG_SUCCESS;
	}

	void GraphicFillSymbolImpl::LoadSymbol()
	{
		m_icon = cairo_image_surface_create_from_png(m_file_path.c_str());
		if(m_icon)
		{
			m_cairo = cairo_create(m_icon);
		}
	}
}