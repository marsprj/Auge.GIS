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

		virtual void			SetStyleID(g_int style_id);

		virtual RESULTCODE		SetStyle(Style* pStyle);
		virtual Style*			GetStyle();

		virtual void			Release();

	public:
		void			SetFeatureSource(g_uint source_id);
		void			SetFeatureClassName(const char* className);

	private:
		std::string		m_name;
		GEnvelope		m_extent;
		g_int			m_srid;
		bool			m_queryable;
		bool			m_visiable;
		Style*			m_pStyle;
		g_uint			m_style_id;

		double			m_min_scale;
		double			m_max_scale;

		FeatureClass*	m_pFeatureClass;
		g_uint			m_source_id;
		std::string		m_class_name;
	};
}

#endif //__AUGE_FEATURE_LAYER_IMPL_H__
