#ifndef __AUGE_FEATURE_CURSOR_SHP_H__
#define __AUGE_FEATURE_CURSOR_SHP_H__

#include "AugeFeature.h"
#include "shapefil.h"

namespace auge
{
	class FeatureClassShp;

	class FeatureCursorShp : public FeatureCursor
	{
	public:
		FeatureCursorShp();
		virtual ~FeatureCursorShp();
	public:
		virtual Feature*		NextFeature();
		virtual FeatureClass*	GetFeatureClass();
		virtual void			Release();

	public:
		bool	Create(FeatureClassShp* pFeatureClass, SHPHandle pSHPHandle, DBFHandle pDBFHandle);

	public:
		FeatureClassShp *m_pFeatureClass;
		SHPHandle		m_pSHPHandle;
		DBFHandle		m_pDBFHandle;

		int				m_Cursor;
		int				m_Count;
		bool			m_bIsAll;
	};
}

#endif //__AUGE_FEATURE_CURSOR_SHP_H__
