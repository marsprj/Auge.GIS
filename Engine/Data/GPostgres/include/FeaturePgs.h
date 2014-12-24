#ifndef __AUGE_FEATURE_PGS_H__
#define __AUGE_FEATURE_PGS_H__

#include "AugeFeature.h"
#include <libpq-fe.h>

namespace auge
{
	class Geometry;
	class FeatureClassPgs;

	class FeaturePgs : public Feature
	{
	public:
		FeaturePgs();
		virtual ~FeaturePgs();

	public:
		virtual g_int		GetFID();
		virtual Geometry*	GetGeometry();
		virtual void		Release();

	public:
		bool		Create(g_uint index, PGresult* pgResult, FeatureClassPgs* pFeatureClass);
		Geometry*	CreateGeometry();

	private:
		FeatureClassPgs	*m_pFeatureClass;
		Geometry		*m_pGeometry;
		PGresult		*m_pgResult;
		g_uint			m_index;
		g_int			m_fid;
	};
}

#endif //__AUGE_FEATURE_PGS_H__
