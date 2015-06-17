#ifndef __AUGE_BUFFER_PROCESSOR_IMPL_H__
#define __AUGE_BUFFER_PROCESSOR_IMPL_H__

#include "AugeProcessor.h"

namespace auge
{
	class Geometry;

	class BufferProcessorImpl : public BufferProcessor
	{
	protected:
		BufferProcessorImpl();
		virtual ~BufferProcessorImpl();
	public:
		virtual RESULTCODE	Execute();

		virtual	void		SetGeometry(Geometry* pGeometry);
		virtual Geometry*	GetGeometry();

		virtual void		SetRadius(double radius);
		virtual double		GetRadius();

		virtual Geometry*	GetResultGeometry();

	private:
		Geometry*			m_pinGeometry;
		Geometry*			m_pouGeometry;

		double				m_radius;
	};
}

#endif //__AUGE_BUFFER_PROCESSOR_IMPL_H__
