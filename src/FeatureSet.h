#ifndef __AUGE_FEATURE_SET_H__
#define __AUGE_FEATURE_SET_H__

#include "Base.h"
#include "Feature.h"
#include <libpq-fe.h>

namespace auge
{
	class FeatureType;

	class AUGE_API FeatureSet : public Base
	{
	public:
		FeatureSet();
		virtual ~FeatureSet();
	public:
		Feature&		Next();
		bool			IsEOF();
		void			Reset();

		FeatureType*	GetFeatureType();

	public:
		bool		Create(FeatureType* pFeatureType, PGresult* pgResult, GeometryFormat format);
	private:
		FeatureType*	m_pFeatureType;
		PGresult*		m_pgResult;
		int				m_total;
		int				m_index;

		Feature			m_feature;
		GeometryFormat	m_format;
	};
}

#endif //__AUGE_FEATURE_SET_H__
