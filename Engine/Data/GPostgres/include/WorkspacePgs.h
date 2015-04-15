#ifndef __AUGE_WORKSPACE_SHP_H__
#define __AUGE_WORKSPACE_SHP_H__

#include "AugeFeature.h"
#include <string>
#include <map>

#include "ConnectionPgs.h"

namespace auge
{
	class WorkspacePgs : public FeatureWorksapce
	{
		friend class FeaturePgs;
		friend class FeatureClassPgs;
		friend class FeatureInsertCommandPgs;

	public:
		WorkspacePgs();
		virtual ~WorkspacePgs();
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

		virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);

		// FeatureClass
		virtual	FeatureClass*	OpenFeatureClass(const char* name);
		virtual RESULTCODE		CreateFeatureClass(const char* name, GFields* pFields);
		virtual RESULTCODE		RemoveFeatureClass(const char* name);

	private:
		RESULTCODE				CreateTable(const char* name, GFields* pFields);
		RESULTCODE				RemoveTable(const char* name);

		bool					UnRegisterLayer(long lid);
		//RESULTCODE				RegiseterGeometryColumns(const char* szName, AgField* pGeomField);

		RESULTCODE				AddGeometryColumn(const char* name, GField* pField);
		RESULTCODE				RegiseterGeometryColumn(const char* name, GField* pField);
		RESULTCODE				UnRegiseterGeometryColumn(const char* name);
		
	private:
		ConnectionPgs	m_pgConnection;
		std::string		m_name;
		std::string		m_schema;
	};
}

#endif //__AUGE_WORKSPACE_SHP_H__
