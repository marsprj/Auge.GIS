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
		virtual Feature*	NextFeature();
		virtual void		Release();

	public:
		bool	Create(FeatureClassPgs* pFeatureClass, PGresult* pgResult);

	public:
		FeatureClassPgs *m_pFeatureClass;
		PGresult		*m_pgResult;

		int				m_cursor;
		int				m_count;
	};
}

#endif //__AUGE_FEATURE_CURSOR_PGS_H__
