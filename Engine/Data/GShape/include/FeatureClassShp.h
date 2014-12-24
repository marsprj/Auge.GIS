#ifndef __AUGE_FEATURE_CLASS_SHP_H__
#define __AUGE_FEATURE_CLASS_SHP_H__

#include "AugeFeature.h"
#include <string>

#include "shapefil.h"

namespace auge
{
	class WorkspaceShp;

	class FeatureClassShp : public FeatureClass
	{
		friend class FeatureShp;

	public:
		FeatureClassShp();
		virtual ~FeatureClassShp();
	public:
		virtual const char*			GetName();
		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor);

		virtual void				Release();

	public:
		bool	Create(const char* szName, WorkspaceShp* pWorkspace);

	private:
		void	CloseSHPFile();

	private:
		std::string		m_name;
		WorkspaceShp	*m_pWorkspace;
		SHPHandle		m_pSHPHandle;
		DBFHandle		m_pDBFHandle;
	};
}

#endif //__AUGE_FEATURE_CLASS_SHP_H__
