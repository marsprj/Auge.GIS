#include "AugeStyle.h"
#include "ColorMapImpl.h"

namespace auge
{

	ColorMapImpl::ColorMapImpl():
m_pColors(NULL),
	m_count(0)
{

}

ColorMapImpl::~ColorMapImpl()
{
	if(m_pColors!=NULL)
	{
		delete[] m_pColors;
		m_pColors = NULL;
	}
}

g_uint ColorMapImpl::GetCount()
{
	return m_count;
}

GColor* ColorMapImpl::GetColor(g_uint i)
{
	if(i>=m_count)
	{
		return NULL;
	}
	return m_pColors + i;
}

void ColorMapImpl::Release()
{
	delete this;
}

bool ColorMapImpl::Create(g_uint id, GColor start, GColor end)
{
	SetID(id);
	m_start_color = start;
	m_end_color = end;
	return true;
}

bool ColorMapImpl::Create(g_uint id, GColor start, GColor end, const char* img_path, const char* abs_img_path)
{
	SetID(id);
	m_start_color = start;
	m_end_color = end;
	SetImagePath(img_path);
	SetAbsoluteImagePath(abs_img_path);

	return true;
}

GColor&	ColorMapImpl::GetStartColor()
{
	return m_start_color;
}

GColor&	ColorMapImpl::GetEndColor()
{
	return m_end_color;
}

bool ColorMapImpl::SetCount(g_uint count)
{
	if(count==0)
	{
		return false;
	}

	if(m_pColors!=NULL)
	{
		delete[] m_pColors;
		m_pColors = NULL;
	}
	if(count==0)
	{
		return false;
	}
	m_count = count;
	m_pColors = new GColor[count];
	GColor& start = m_start_color;
	GColor& end = m_end_color;
	
	g_byte r=0, g=0, b=0;
	float h=0.0f, s=0.0f, v=0.0f;
	float h_s=0.0f, s_s=0.0f, v_s=0.0f;
	float h_e=0.0f, s_e=0.0f, v_e=0.0f;
	
	auge_rgb_2_hsv(start.GetRed(), start.GetGreen(), start.GetBlue(), h_s, s_s, v_s);
	auge_rgb_2_hsv(end.GetRed(), end.GetGreen(), end.GetBlue(), h_e, s_e, v_e);
	
	float h_span = (h_e - h_s) / (m_count-1);
	float s_span = (s_e - s_s) / (m_count-1);
	float v_span = (v_e - v_s) / (m_count-1);
	
	m_pColors[0].Set(start);
	for(int i=1; i<(m_count); i++)
	{
		auge_hsv_2_rgb(h_s+h_span*i, s_s+s_span*i, v_s+v_span*i, r, g, b);
		m_pColors[i].Set(r, g, b, 255);
	}
	//m_pColors[count-1] = end

	return true;
}

//bool ColorMapImpl::Create(g_uint count, GColor start, GColor end)
//{
//	if(m_pColors!=NULL)
//	{
//		delete[] m_pColors;
//		m_pColors = NULL;
//	}
//	if(count==0)
//	{
//		return false;
//	}
//	m_count = count;
//	m_pColors = new GColor[count];
//	//memset(m_pColors, 0, sizeof(GColor)*count);
//
//	g_byte r=0, g=0, b=0;
//	float h=0.0f, s=0.0f, v=0.0f;
//	float h_s=0.0f, s_s=0.0f, v_s=0.0f;
//	float h_e=0.0f, s_e=0.0f, v_e=0.0f;
//
//	auge_rgb_2_hsv(start.GetRed(), start.GetGreen(), start.GetBlue(), h_s, s_s, v_s);
//	auge_rgb_2_hsv(end.GetRed(), end.GetGreen(), end.GetBlue(), h_e, s_e, v_e);
//
//	float h_span = (h_e - h_s) / (m_count-1);
//	float s_span = (s_e - s_s) / (m_count-1);
//	float v_span = (v_e - v_s) / (m_count-1);
//
//	m_pColors[0].Set(start);
//	for(int i=1; i<(m_count); i++)
//	{
//		auge_hsv_2_rgb(h_s+h_span*i, s_s+s_span*i, v_s+v_span*i, r, g, b);
//		m_pColors[i].Set(r, g, b, 255);
//	}
//	//m_pColors[count-1] = end;
//
//	return true;
//}

	const char* ColorMapImpl::GetImagePath()
	{
		return m_image_path.c_str();
	}

	const char*	ColorMapImpl::GetAbsoluteImagePath()
	{
		return m_absolute_image_path.c_str();
	}

	void ColorMapImpl::SetImagePath(const char* path)
	{
		if(path==NULL)
		{
			m_image_path.clear();
		}
		else
		{
			m_image_path = path;
		}
	}
	void ColorMapImpl::SetAbsoluteImagePath(const char* path)
	{
		if(path==NULL)
		{
			m_absolute_image_path.clear();
		}
		else
		{
			m_absolute_image_path = path;
		}
	}

}
