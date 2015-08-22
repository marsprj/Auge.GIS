#ifndef __AUGE_FEATURE_LAYER_IMPL_H__
#define __AUGE_FEATURE_LAYER_IMPL_H__

#include "AugeCarto.h"
#include <string>

namespace auge
{
	class Style;

	class FeatureLayerImpl : public FeatureLayer
	{
	public:
		FeatureLayerImpl();
		virtual ~FeatureLayerImpl();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* szName);
		virtual augeLayerType	GetType();

		virtual g_int			GetSRID();
		virtual GEnvelope&		GetExtent();

		virtual RESULTCODE		SetFeatureClass(FeatureClass* pFeatureClass);
		virtual FeatureClass*	GetFeatureClass();

		virtual bool			IsQueryable();
		virtual void			SetQueryable(bool flag);

		virtual bool			IsVisiable();
		virtual bool			IsVisiable(double scale);
		virtual void			SetVisiable(bool flag);

		virtual	double			GetMinScale();
		virtual	void			SetMinScale(double scale);

		virtual	double			GetMaxScale();
		virtual	void			SetMaxScale(double scale);

		virtual RESULTCODE		SetStyle(Style* pStyle);
		virtual Style*			GetStyle();

		virtual void			Release();

	private:
		std::string		m_name;
		GEnvelope		m_extent;
		g_int			m_srid;
		bool			m_queryable;
		bool			m_visiable;
		Style*			m_pStyle;

		double			m_min_scale;
		double			m_max_scale;

		FeatureClass*	m_pFeatureClass;
	};
}

#endif //__AUGE_FEATURE_LAYER_IMPL_H__
