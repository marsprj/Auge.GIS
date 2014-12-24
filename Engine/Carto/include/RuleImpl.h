#ifndef __AUGE_RULE_IMPL_H__
#define __AUGE_RULE_IMPL_H__

#include "AugeStyle.h"
#include <string>

namespace auge
{

	class RuleImpl : public Rule
	{
	public:
		RuleImpl();
		virtual ~RuleImpl();

	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* szName);

		virtual const char*		GetTitle();
		virtual void			SetTitle(const char* szTitle);

		virtual const char*		GetAbstract();
		virtual void			SetAbstract(const char* szAbstract);

		virtual Graphic*		GetLegendGraphic();
		virtual void			SetLegendGraphic(Graphic* pagGraphic);

		virtual	Symbolizer*		GetSymbolizer();
		virtual void			SetSymbolizer(Symbolizer *pagSymbolizer);

		virtual	TextSymbolizer*	GetTextSymbolizer();
		virtual void			SetTextSymbolizer(TextSymbolizer *pSymbolizer);

		virtual double			GetMinScale();
		virtual void			SetMinScale(double scale);

		virtual double			GetMaxScale();
		virtual void			SetMaxScale(double scale);

		virtual GFilter*		GetFilter();
		virtual void			SetFilter(GFilter* pagFilter);

		virtual void			Release();
	private:
		std::string		m_strName;
		std::string		m_strTitle;
		std::string		m_strAbstract;

		Graphic			*m_pLegendGraphic;
		Symbolizer		*m_pSymbolizer;
		TextSymbolizer	*m_pTextSymbolizer;
		double			m_min_scale;
		double			m_max_scale;

		GFilter			*m_pFilter;

	};

}

#endif //__AUGE_RULE_IMPL_H__
