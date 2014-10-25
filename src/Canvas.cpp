#include "Canvas.h"
#include "Renderer.h"
#include "Transformation.h"
#include <stdio.h>

namespace auge
{
	Canvas::Canvas(Map*pMap, uint width, uint height):
	m_width(width),
	m_height(height),
	m_pMap(pMap)
	{
		m_pRenderer = new Renderer();
		m_pRenderer->Create(m_width, m_height);
	}

	Canvas::~Canvas()
	{
		if(m_pRenderer!=NULL)
		{
			m_pRenderer->Release();
			m_pRenderer = NULL;
		}
	}

	int	Canvas::Draw(Envelope& viewer)
	{
		Transformation trans;
		trans.SetCanvas(m_width, m_height);
		trans.SetViewer(viewer);
		
		m_pRenderer->DrawMap(m_pMap, &trans);

		return AG_SUCCESS;
	}

	int	Canvas::Save(const char* path)
	{
		m_pRenderer->Save(path);
		return AG_SUCCESS;
	}
}