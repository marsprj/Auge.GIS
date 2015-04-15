#ifndef __AUGE_WORKSPACE_SHP_H__
#define __AUGE_WORKSPACE_SHP_H__

#include "AugeFeature.h"
#include <string>

namespace auge
{
	class WorkspaceShp : public FeatureWorksapce
	{
		friend class FeatureClassShp;

	public:
		WorkspaceShp();
		virtual ~WorkspaceShp();
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

		virtual	FeatureClass*	OpenFeatureClass(const char* name);
		virtual RESULTCODE		CreateFeatureClass(const char* name, GFields* pFields);
		virtual RESULTCODE		RemoveFeatureClass(const char* name);

	private:
		std::string	m_connstring;
		std::string	m_path;
		std::string m_name;
	};
}

#endif //__AUGE_WORKSPACE_SHP_H__
