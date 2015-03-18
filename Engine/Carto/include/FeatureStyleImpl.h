#ifndef __AUGE_FEATURE_TYPE_STYLE_IMPL_H__
#define __AUGE_FEATURE_TYPE_STYLE_IMPL_H__

#include "AugeStyle.h"
#include <vector>
#include <string>

namespace auge
{
	class FeatureStyleImpl : public FeatureStyle
	{
	public:
		FeatureStyleImpl();
		virtual ~FeatureStyleImpl();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* szName);
		virtual augeStyleType	GetType();

		virtual const char*		GetTitle();
		virtual void			SetTitle(const char* szTitle);

		virtual const char*		GetAbstract();
		virtual void			SetAbstract(const char* szAbstract);

		virtual const char*		GetFeatureTypeName();
		virtual bool			SetFeatureTypeName(const char* szName);

		virtual int				GetRuleCount();
		virtual Rule*			GetRule(int i);
		virtual bool			AddRule(Rule* pagRule);

		virtual augeGeometryType GetGeometryType();
		virtual void			SetGeometryType(augeGeometryType type);

		virtual void			Release();

	private:
		void	Clear();
	private:
		std::string	m_name;
		typedef std::vector<Rule*> RULE_VECTOR;
		RULE_VECTOR m_rules;
		augeGeometryType		m_geom_type;
	};


}

#endif //__AUGE_FEATURE_TYPE_STYLE_IMPL_H__
