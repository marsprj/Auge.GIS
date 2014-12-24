#include "AugeCore.h"
#include "AugeGeometry.h"

namespace auge
{
	GEnvelope::GEnvelope():
m_xmin(DOUBLE_MAX), 
	m_ymin(DOUBLE_MAX), 
	m_xmax(DOUBLE_MIN), 
	m_ymax(DOUBLE_MIN)
{

}
GEnvelope::GEnvelope(double xmin, double ymin, double xmax, double ymax):
m_xmin(xmin), 
	m_ymin(ymin), 
	m_xmax(xmax), 
	m_ymax(ymax)
{

}

void GEnvelope::Set(double xmin, double ymin, double xmax, double ymax)
{
	m_xmin = xmin;
	m_xmax = xmax;
	m_ymin = ymin;
	m_ymax = ymax;
}

void GEnvelope::Union(GEnvelope& o)
{
	m_xmin = m_xmin < o.m_xmin ? m_xmin : o.m_xmin;
	m_xmax = m_xmax > o.m_xmax ? m_xmax : o.m_xmax;
	m_ymin = m_ymin < o.m_ymin ? m_ymin : o.m_ymin;
	m_ymax = m_ymax > o.m_ymax ? m_ymax : o.m_ymax;
}

bool GEnvelope::IsValid()
{
	return ((m_xmin<m_xmax)||(m_ymin<m_ymax));
}

GEnvelope::~GEnvelope()
{

}

double GEnvelope::GetWidth()
{
	return (m_xmax-m_xmin);
}

double GEnvelope::GetHeight()
{
	return (m_ymax-m_ymin);
}
};