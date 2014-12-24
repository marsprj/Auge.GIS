#ifndef __AUGE_MARK_IMPL_H__
#define __AUGE_MARK_IMPL_H__

#include "AugeStyle.h"

namespace auge
{
	class MarkImpl : public Mark
	{
	public:
		MarkImpl();
		virtual ~MarkImpl();
	public:
		virtual augeMarkType	GetType();
		virtual void			SetType(augeMarkType type);

		virtual Fill*			GetFill();
		virtual bool			SetFill(Fill *pFill);

		virtual Stroke*			GetStoke();
		virtual bool			SetStroke(Stroke *pStroke);

		virtual void			Release();
	private:
		augeMarkType	m_type;
		Fill			*m_pFill;
		Stroke			*m_pStroke;
	};

}

#endif //__AUGE_MARK_IMPL_H__

