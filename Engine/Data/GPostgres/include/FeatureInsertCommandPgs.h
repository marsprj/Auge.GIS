#ifndef __AUGE_FEATURE_INSERT_COMMAND_PGS_H__
#define __AUGE_FEATURE_INSERT_COMMAND_PGS_H__

#include "AugeFeature.h"

namespace auge
{
	class FeatureClassPgs;
	class ConnectionPgs;

	class FeatureInsertCommandPgs : public FeatureInsertCommand
	{
	public:
		FeatureInsertCommandPgs(FeatureClassPgs* pFeatureClass);
		virtual ~FeatureInsertCommandPgs();
	public:
		virtual	RESULTCODE	Prepare(FeatureClass* pFeatureClass);
		virtual RESULTCODE	Insert(Feature* pFeature);
	private:
		FeatureClassPgs	*m_pFeatureClass;
		ConnectionPgs	*m_pgConnection;
	};
}

#endif //__AUGE_FEATURE_INSERT_COMMAND_PGS_H__
