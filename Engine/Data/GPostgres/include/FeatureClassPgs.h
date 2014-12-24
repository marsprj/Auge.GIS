#ifndef __AUGE_FEATURECLASS_PGS_H__
#define __AUGE_FEATURECLASS_PGS_H__

#include "AugeFeature.h"
#include <libpq-fe.h>

namespace auge
{
	class WorkspacePgs;

	class FeatureClassPgs : public FeatureClass
	{
		friend class FeaturePgs;

	public:
		FeatureClassPgs();
		virtual ~FeatureClassPgs();
	public:
		virtual const char*			GetName();
		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor);

		virtual void				Release();

	public:
		bool	Create(const char* szName, WorkspacePgs* pWorkspace);

	private:

	private:
		std::string		m_name;
		WorkspacePgs	*m_pWorkspace;
	};
}

#endif //__AUGE_FEATURECLASS_PGS_H__
