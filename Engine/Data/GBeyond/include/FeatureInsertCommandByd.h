#ifndef __AUGE_FEATURE_INSERT_COMMAND_BYD_H__
#define __AUGE_FEATURE_INSERT_COMMAND_BYD_H__

#include "AugeFeature.h"

namespace auge
{
	class FeatureClassByd;
	class WorkspaceByd;

	class FeatureInsertCommandByd : public FeatureInsertCommand
	{
	public:
		FeatureInsertCommandByd(FeatureClassByd* pFeatureClass);
		virtual ~FeatureInsertCommandByd();
	public:
		virtual	RESULTCODE	Prepare(FeatureClass* pFeatureClass);
		virtual RESULTCODE	Insert(Feature* pFeature);
		virtual RESULTCODE	Commit();
	private:
		FeatureClassByd	*m_pFeatureClass;
		WorkspaceByd	*m_pWorkspace;
	};
}

#endif //__AUGE_FEATURE_INSERT_COMMAND_BYD_H__
