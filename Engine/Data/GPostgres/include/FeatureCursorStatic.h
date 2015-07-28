#ifndef __AUGE_FEATURE_CURSOR_STATIC_PGS_H__
#define __AUGE_FEATURE_CURSOR_STATIC_PGS_H__

#include "AugeFeature.h"
#include <libpq-fe.h>

namespace auge
{
	class WorkspacePgs;
	class FeatureClassPgs;

	class FeatureCursorStatic : public FeatureCursor
	{
	public:
		FeatureCursorStatic();
		virtual ~FeatureCursorStatic();
	public:
		virtual Feature*		NextFeature();
		virtual FeatureClass*	GetFeatureClass();
		virtual void			Release();

	public:
		bool					Create(FeatureClassPgs* pFeatureClass);
		//bool					Create(FeatureClassPgs* pFeatureClass, GFilter* pFilter);
		//bool					Create(FeatureClassPgs* pFeatureClass, GQuery*  pQuery);

	private:
		RESULTCODE				OpenCursor();

	public:
		WorkspacePgs	*m_pWorkspace;
		FeatureClassPgs *m_pFeatureClass;
		PGresult		*m_pgResult;

		g_int			m_cursor;
		g_int			m_geom_findex;

		g_int			m_fetch_count;
		g_int			m_fetched_count;
		g_int			m_last_fetched_count;
		bool			m_has_more_data;
		char			m_cursor_name[AUGE_NAME_MAX];
		
	};
}

#endif //__AUGE_FEATURE_CURSOR_STATIC_PGS_H__
