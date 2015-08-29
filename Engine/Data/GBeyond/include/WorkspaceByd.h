#ifndef __AUGE_WORKSPACE_BYD_H__
#define __AUGE_WORKSPACE_BYD_H__

#include <cppi.h>
#include <string.h>
#include <string>

#include "AugeFeature.h"

namespace auge
{
	class WorkspaceByd : public FeatureWorkspace
	{
		friend class FeatureClassByd;
		friend class FeatureCursorByd;

	public:
		WorkspaceByd();
		virtual ~WorkspaceByd();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* name);
		virtual	DataEngine*		GetEngine();

	public:
		/* DATABASE=d:\\data\\world */
		virtual	RESULTCODE		SetConnectionString(const char* conn_string);
		virtual const char*		GetConnectionString();

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();
		virtual GConnection*	GetConnection();

		virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);
		virtual RESULTCODE		RemoveDataSet(const char* name);

		virtual EnumDataSet*	GetFeatureClasses();
		virtual	FeatureClass*	OpenFeatureClass(const char* name);
		virtual FeatureClass*	CreateFeatureClass(const char* name, GFields* pFields);
		virtual RESULTCODE		RemoveFeatureClass(const char* name);


	private:
		RESULTCODE	ExecuteSQL(const char* sql);
		bool		CreateSequence(const char* name);
		bool		CreateTable(const char* name, GFields* pFields);

	private:
		CPPIEnvironment*		m_pbydEnvironment;
		CPPIConnection*         m_pbydConnction;

		std::string		m_name;
		std::string		m_connstring;
		GPropertySet	m_props;
	};
}

#endif //__AUGE_WORKSPACE_BYD_H__
