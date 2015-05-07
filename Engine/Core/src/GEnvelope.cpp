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
	m_xmin = xmin < xmax ? xmin : xmax;
	m_xmax = xmin > xmax ? xmin : xmax;
	m_ymin = ymin < ymax ? ymin : ymax;
	m_ymax = ymin > ymax ? ymin : ymax;
}

void GEnvelope::Union(GEnvelope& o)
{
	m_xmin = m_xmin < o.m_xmin ? m_xmin : o.m_xmin;
	m_xmax = m_xmax > o.m_xmax ? m_xmax : o.m_xmax;
	m_ymin = m_ymin < o.m_ymin ? m_ymin : o.m_ymin;
	m_ymax = m_ymax > o.m_ymax ? m_ymax : o.m_ymax;
}

void GEnvelope::Offset(double x, double y)
{
	m_xmin += x;
	m_xmax += x;
	m_ymin += y;
	m_ymax += y;
}

void GEnvelope::MoveTo(double x, double y)
{
	double w = m_xmax - m_xmin;
	double h = m_ymax - m_ymin;
	m_xmin = x;
	m_xmax = x + w;
	m_ymin = y;
	m_ymax = y + h;
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

bool GEnvelope::Contain(double x, double y)
{
	bool b1 = (x>m_xmin) && (x<m_xmax);  
	bool b2 = (y>m_ymin) && (y<m_ymax);  

	return b1 && b2;
}

bool GEnvelope::Intersects(GEnvelope& other)
{
	double xmin = m_xmin > other.m_xmin ? m_xmin : other.m_xmin;
	double xmax = m_xmax < other.m_xmax ? m_xmax : other.m_xmax;

	double ymin = m_ymin > other.m_ymin ? m_ymin : other.m_ymin;
	double ymax = m_ymax < other.m_ymax ? m_ymax : other.m_ymax;

	return (xmin<xmax)&&(ymin<ymax);  
}

void GEnvelope::Inflate(int o)
{
	m_xmin -= o;
	m_xmax += o;
	m_ymin -= o;
	m_ymax += o;
}

};