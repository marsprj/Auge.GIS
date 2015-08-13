#include "GraphicMarkerSymbolImpl.h"

namespace auge
{
	GraphicMarkerSymbolImpl::GraphicMarkerSymbolImpl()
	{

	}

	GraphicMarkerSymbolImpl::~GraphicMarkerSymbolImpl()
	{

	}

	const char*	GraphicMarkerSymbolImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	GraphicMarkerSymbolImpl::GetPath()
	{
		return m_path.c_str();
	}

	void GraphicMarkerSymbolImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float GraphicMarkerSymbolImpl::GetOpacity()
	{
		return m_opacity;
	}
}