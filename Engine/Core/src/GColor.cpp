#include "AugeCore.h"
#include <stdlib.h>

namespace auge
{
	unsigned long MakeARGB(unsigned char r,unsigned char g, unsigned char b, unsigned char a)
	{
		return (((unsigned long) (b) <<  BlueShift) |
			((unsigned long) (g) << GreenShift) |
			((unsigned long) (r) <<   RedShift) |
			((unsigned long) (a) << AlphaShift));
	}

	int rands(int s)
	{
		if(s<0)
			return 0;
		int span = s;
		int val = rand() % span;
		return val;
	}

	GColor::GColor()
	{
		g_byte r = (unsigned char)rands(255);
		g_byte g = (unsigned char)rands(255);
		g_byte b = (unsigned char)rands(255);
		g_byte a = 255;

		m_color = MakeARGB(r, g, b, a);

		m_r = r / 255.0f;
		m_g = g / 255.0f;
		m_b = b / 255.0f;
		m_a = a / 255.0f;
	}

	GColor::GColor(g_ulong color)
	{

	}

	GColor::GColor(g_byte r, g_byte g, g_byte b, g_byte a)
	{
		m_color = MakeARGB(r, g, b, a);

		m_r = r / 255.0f;
		m_g = g / 255.0f;
		m_b = b / 255.0f;
		m_a = a / 255.0f;
	}

	GColor::~GColor()
	{

	}

	g_byte GColor::GetAlpha()
	{
		return (g_byte) (m_color >> AlphaShift);
	}

	g_byte GColor::GetRed()
	{
		return (unsigned char) (m_color >> RedShift);
	}

	g_byte GColor::GetGreen()
	{
		return (unsigned char) (m_color >> GreenShift);
	}

	g_byte GColor::GetBlue()
	{
		return (unsigned char) (m_color >> BlueShift);
	}

	void GColor::SetAlpha(g_byte a)
	{
		m_color = MakeARGB(GetRed(), GetGreen(), GetBlue(), a);
		m_a = a / 255.0f;
	}

	void GColor::SetRed(g_byte r)
	{
		m_color = MakeARGB(r, GetGreen(), GetBlue(), GetAlpha());
		m_r = r / 255.0f;
	}

	void GColor::SetGreen(g_byte g)
	{
		m_color = MakeARGB(GetRed(), g, GetBlue(), GetAlpha());
		m_g = g / 255.0f;
	}

	void GColor::SetBlue(g_byte b)
	{
		m_color = MakeARGB(GetRed(), GetGreen(), b, GetAlpha());
		m_b = b / 255.0f;
	}


	void GColor::Set(g_ulong color)
	{
		m_color = color;
	}

	void GColor::Set(g_byte r, g_byte g, g_byte b, g_byte a)
	{
		m_color = MakeARGB(r, g, b, a);
		m_r = r / 255.0f;
		m_g = g / 255.0f;
		m_b = b / 255.0f;
		m_a = a / 255.0f;
	}

	void GColor::Set(GColor &color)
	{
		m_color = MakeARGB(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
		m_r = color.GetRed()   / 255.0f;
		m_g = color.GetGreen() / 255.0f;
		m_b = color.GetBlue()  / 255.0f;
		m_a =color.GetAlpha()  / 255.0f;
	}

	float GColor::GetAlphaF()
	{
		return m_a;
	}

	float GColor::GetRedF()
	{
		return m_r;
	}

	float GColor::GetGreenF()
	{
		return m_g;
	}

	float GColor::GetBlueF()
	{
		return m_b;
	}

}