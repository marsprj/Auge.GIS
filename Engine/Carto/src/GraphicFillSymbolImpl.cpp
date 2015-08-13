#include "GraphicFillSymbolImpl.h"

namespace auge
{
	GraphicFillSymbolImpl::GraphicFillSymbolImpl()
	{

	}

	GraphicFillSymbolImpl::~GraphicFillSymbolImpl()
	{

	}

	const char*	GraphicFillSymbolImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	GraphicFillSymbolImpl::GetPath()
	{
		return m_path.c_str();
	}

	void GraphicFillSymbolImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float GraphicFillSymbolImpl::GetOpacity()
	{
		return m_opacity;
	}
}