#include "Envelope.h"

namespace auge
{
	Envelope::Envelope():
	m_xmin(DOUBLE_MAX), 
	m_ymin(DOUBLE_MAX), 
	m_xmax(DOUBLE_MIN), 
	m_ymax(DOUBLE_MIN)
	{

	}
	Envelope::Envelope(double xmin, double ymin, double xmax, double ymax):
	m_xmin(xmin), 
	m_ymin(ymin), 
	m_xmax(xmax), 
	m_ymax(ymax)
	{

	}

	void Envelope::Union(Envelope& o)
	{
		m_xmin = m_xmin < o.m_xmin ? m_xmin : o.m_xmin;
		m_xmax = m_xmax > o.m_xmax ? m_xmax : o.m_xmax;
		m_ymin = m_ymin < o.m_ymin ? m_ymin : o.m_ymin;
		m_ymax = m_ymax > o.m_ymax ? m_ymax : o.m_ymax;
	}

	bool Envelope::IsValid()
	{
		return ((m_xmin<m_xmax)||(m_ymin<m_ymax));
	}
	
	Envelope::~Envelope()
	{

	}

	double Envelope::GetWidth()
	{
		return (m_xmax-m_xmin);
	}

	double Envelope::GetHeight()
	{
		return (m_ymin-m_ymin);
	}
};