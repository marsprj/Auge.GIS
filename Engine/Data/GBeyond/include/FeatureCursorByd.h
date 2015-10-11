#ifndef __AUGE_FEATURE_CURSOR_BYD_H__
#define __AUGE_FEATURE_CURSOR_BYD_H__

#include <AugeFeature.h>
#include <cppi.h>

namespace auge
{
	class GFilter;
	class FeatureClassByd;

	class FeatureCursorByd : public FeatureCursor
	{
	public:
		FeatureCursorByd();
		virtual ~FeatureCursorByd();
	public:
		virtual Feature*		NextFeature();
		virtual FeatureClass*	GetFeatureClass();

	public:
		bool	Create(FeatureClassByd* pFeatureClass);
		bool	Create(FeatureClassByd* pFeatureClass, GFilter* pFilter);
		bool	Create(FeatureClassByd* pFeatureClass, GQuery* pQuery);
		bool	Create(FeatureClassByd* pFeatureClass, GEnvelope& extent);

	private:
		FeatureClassByd* CreateFeatureClass(FeatureClassByd* pFeatureClass, CPPIResultSet* pResultSet);
		FeatureClassByd* CreateFeatureClass(FeatureClassByd* pFeatureClass, GQuery* pQuery);

	private:
		FeatureClassByd*	m_pFeatureClass;
		CPPIConnection*		m_pConnection;
		CPPIResultSet*		m_pResultSet;
		CPPIStatement*		m_pStatment;
	};
}


#endif //__AUGE_FEATURE_CURSOR_BYD_H__
