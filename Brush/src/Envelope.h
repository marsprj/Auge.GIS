#ifndef __AUGE_ENVELOPE_H__
#define __AUGE_ENVELOPE_H__

#include "Base.h"

namespace auge
{
	class AUGE_API Envelope
	{
	public:
		Envelope();
		Envelope(double	xmin, double ymin, double xmax, double ymax);
		virtual ~Envelope();

	public:
		void	Union(Envelope& o);

		double	GetWidth();
		double	GetHeight();

		bool	IsValid();
	public:
		double	m_xmin;
		double	m_ymin;
		double	m_xmax;
		double	m_ymax;
	};
};

#endif //__AUGE_ENVELOPE_H__