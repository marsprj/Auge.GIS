#ifndef __AUGE_COLOR_H__
#define __AUGE_COLOR_H__

#include "Base.h"
#include <stdlib.h>

namespace auge
{
	enum
	{
		AlphaShift		= 24,
		RedShift		= 16,
		GreenShift		= 8,
		BlueShift		= 0
	};	

	class Color
	{
	public:
		Color()
		{
			m_color = MakeARGB((unsigned char)rands(255),(unsigned char)rands(255),(unsigned char)rands(255),255);
		}

		explicit Color(ulong color)
		{
			m_color=color;
		}

		Color(uchar r, uchar g, uchar b, uchar a)
		{
			m_color = MakeARGB(r, g, b, a);
		}

		~Color()
		{

		}
	public:
		uchar		GetAlpha()	{return (uchar) (m_color >> AlphaShift);}
		uchar		GetRed()	{return (uchar) (m_color >> RedShift);}
		uchar		GetGreen()	{return (uchar) (m_color >> GreenShift);}
		uchar		GetBlue()	{return (uchar) (m_color >> BlueShift);}

		void		SetAlpha(uchar a)	{m_color = MakeARGB(GetRed(), GetGreen(), GetBlue(), a);}
		void		SetRed(uchar r)		{m_color = MakeARGB(r, GetGreen(), GetBlue(), GetAlpha());}
		void		SetGreen(uchar g)	{m_color = MakeARGB(GetRed(), g, GetBlue(), GetAlpha());}
		void		SetBlue(uchar b)	{m_color = MakeARGB(GetRed(), GetGreen(), b, GetAlpha());}

		void		Set(ulong color)	{m_color = color;}
		void		Set(uchar r, uchar g, uchar b, uchar a)	{m_color = MakeARGB(r, g, b, a);}
		void		Set(Color &color)	{m_color = MakeARGB(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());}

	public:
		static ulong MakeARGB(unsigned char r,unsigned char g, unsigned char b, unsigned char a)
		{
			return (((ulong)(b)<<BlueShift) | ((ulong)(g)<<GreenShift) | ((ulong)(r)<<RedShift) | ((ulong)(a)<<AlphaShift));
		}

		static int rands(int s)
		{
			if(s<0)
				return 0;
			int span = s;
			int val = rand() % span;
			return val;
		}
		
	private:
		ulong m_color;
	};

}

#endif //__AUGE_COLOR_H__