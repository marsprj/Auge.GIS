#ifndef __AUGE_FEATURE_CURSOR_PGS_H__
#define __AUGE_FEATURE_CURSOR_PGS_H__

#include "AugeFeature.h"
#include <libpq-fe.h>

namespace auge
{
	class FeatureClassPgs;

	class FeatureCursorPgs : public FeatureCursor
	{
	public:
		FeatureCursorPgs();
		virtual ~FeatureCursorPgs();
	public:
		virtual Feature*		NextFeature();
		virtual FeatureClass*	GetFeatureClass();
		virtual void			Release();

	public:
		bool	Create(FeatureClassPgs* pFeatureClass, PGresult* pgResult);

	public:
		FeatureClassPgs *m_pFeatureClass;
		PGresult		*m_pgResult;

		g_int			m_cursor;
		g_int			m_count;

		g_int			m_geom_findex;
	};
}

#endif //__AUGE_FEATURE_CURSOR_PGS_H__
