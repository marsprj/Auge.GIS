#ifndef __AUGE_FEATURE_SHP_H__
#define __AUGE_FEATURE_SHP_H__

#include "AugeFeature.h"
#include "shapefil.h"

namespace auge
{
	class Geometry;
	class FeatureClassShp;

	class FeatureShp : public Feature
	{
	public:
		FeatureShp();
		virtual ~FeatureShp();

	public:
		virtual g_int		GetFID();
		virtual Geometry*	GetGeometry();
		virtual void		Release();

	public:
		bool		Create(g_int lFID, FeatureClassShp* pFeatureClassShp);
		Geometry*	CreateGeometry(long index, SHPHandle pSHPHandle);

	private:
		FeatureClassShp	*m_pFeatureClass;
		Geometry		*m_pGeometry;
		g_int			m_fid;
	};
}

#endif //__AUGE_FEATURE_SHP_H__
