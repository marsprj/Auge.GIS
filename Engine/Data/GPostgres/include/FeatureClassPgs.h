#ifndef __AUGE_FEATURECLASS_PGS_H__
#define __AUGE_FEATURECLASS_PGS_H__

#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeCore.h"
#include <libpq-fe.h>

namespace auge
{
	class WorkspacePgs;

	class FeatureClassPgs : public FeatureClass
	{
		friend class FeaturePgs;
		friend class FeatureInsertCommandPgs;

	public:
		FeatureClassPgs();
		virtual ~FeatureClassPgs();
	public:
		virtual const char*			GetName();
		virtual GFields*			GetFields();
		virtual GField*				GetField(const char* name);

		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor);

		virtual FeatureInsertCommand* CreateInsertCommand();

		virtual void				Release();

	public:
		bool	Create(const char* name, WorkspacePgs* pWorkspace);
		GField*	CreateField(int col, PGresult* pgResult);
		bool	CreateFields();
		bool	GetMetaData();

	private:

	private:
		Oid					m_oid;
		g_uint				m_srid;
		g_uint				m_dimension;
		std::string			m_name;
		std::string			m_schema;
		std::string			m_geom_filed_name;
		augeGeometryType	m_geom_type;
		WorkspacePgs*		m_pWorkspace;
		GFields*			m_pFields;
	};
}

#endif //__AUGE_FEATURECLASS_PGS_H__
