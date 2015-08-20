#include "BufferProcessorImpl.h"

#include "AugeGeometry.h"

namespace auge
{
	BufferProcessorImpl::BufferProcessorImpl()
	{
		m_radius = -1.0f;

		m_pinGeometry = NULL;
		m_pouGeometry = NULL;
	}

	BufferProcessorImpl::~BufferProcessorImpl()
	{

	}

	void BufferProcessorImpl::SetGeometry(Geometry* pGeometry)
	{
		if(m_pinGeometry!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pinGeometry);
		}
		m_pinGeometry = pGeometry;
	}

	Geometry* BufferProcessorImpl::GetGeometry()
	{
		return m_pinGeometry;
	}

	void BufferProcessorImpl::SetRadius(double radius)
	{
		m_radius = radius;
	}

	double BufferProcessorImpl::GetRadius()
	{
		return m_radius;
	}

	Geometry* BufferProcessorImpl::GetResultGeometry()
	{
		return m_pouGeometry;
	}

	RESULTCODE BufferProcessorImpl::Execute()
	{
		return AG_SUCCESS;
	}
}
