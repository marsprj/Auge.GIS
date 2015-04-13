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
		friend class SQLBuilder;

	public:
		FeatureClassPgs();
		virtual ~FeatureClassPgs();
	public:
		virtual const char*			GetName();
		virtual g_uint				GetSRID();
		virtual GEnvelope&			GetExtent();
		virtual g_int				GetCount();

		virtual GFields*			GetFields();
		virtual GField*				GetField(const char* name);

		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GEnvelope& extent, augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GFilter* pFilter, augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GQuery* pQuery, augeCursorType type=augeStaticCursor);

		virtual EnumValue*			GetUniqueValue(const char* field, augeOrderType order=augeOrderAsc);

		virtual RESULTCODE			RemoveFeature(GFilter* pFilter);

		virtual FeatureInsertCommand* CreateInsertCommand();

		virtual Feature*			NewFeature();

		virtual void				Release();

	public:
		bool	Create(const char* name, WorkspacePgs* pWorkspace);
		bool	Create(const char* name, WorkspacePgs* pWorkspace, PGresult* pgResult);
		GField*	CreateField(int col, PGresult* pgResult);
		bool	CreateFields();
		bool	CreateFields(PGresult* pgResult);
		bool	GetMetaData();

	private:

	private:
		Oid					m_oid;
		g_uint				m_srid;
		g_uint				m_dimension;
		GEnvelope			m_extent;
		std::string			m_name;
		std::string			m_schema;
		std::string			m_geom_filed_name;
		augeGeometryType	m_geom_type;
		WorkspacePgs*		m_pWorkspace;
		GFields*			m_pFields;
	};
}

#endif //__AUGE_FEATURECLASS_PGS_H__
