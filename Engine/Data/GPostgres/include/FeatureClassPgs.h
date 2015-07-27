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
		virtual augeDataSetType		GetType();
		virtual const char*			GetName();
		virtual g_uint				GetSRID();
		virtual GEnvelope&			GetExtent();
		virtual const char*			GetUUID();

		virtual g_uint				GetCount();
		virtual g_uint				GetCount(GEnvelope& extent);
		virtual g_uint				GetCount(GFilter* pFilter);

		virtual GFields*			GetFields();
		virtual GField*				GetField(const char* name);

		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GEnvelope& extent, augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GFilter* pFilter, augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GQuery* pQuery, augeCursorType type=augeStaticCursor);

		virtual Cursor*				GetRows();

		virtual EnumValue*			GetUniqueValue(const char* field, augeOrderType order=augeOrderAsc);
		virtual EnumValue*			GetMinMaxValue(const char* field);

		virtual RESULTCODE			RemoveFeature(GFilter* pFilter);

		virtual FeatureInsertCommand* CreateInsertCommand();

		virtual Feature*			NewFeature();
		virtual RESULTCODE			Refresh();

		virtual void				Release();

	public:
		bool		Create(const char* name, WorkspacePgs* pWorkspace);
		bool		Create(const char* name, WorkspacePgs* pWorkspace, PGresult* pgResult);
		GField*		CreateField(int col, PGresult* pgResult);
		bool		CreateFields();
		bool		CreateFields(PGresult* pgResult);
		bool		GetGeometryInfo();

	private:
		g_uint		ComputeCount();
		RESULTCODE	ComputeExtent(GEnvelope& extent);

		bool		HasMetaInfo();
		RESULTCODE	GetMetaInfo();
		RESULTCODE	AddMetaInfo();
		RESULTCODE	UpdateMetaInfo();

	private:
		Oid					m_oid;
		g_uint				m_srid;
		g_uint				m_dimension;
		g_uint				m_feature_count;
		GEnvelope			m_extent;
		std::string			m_name;
		std::string			m_alias;
		std::string			m_schema;
		std::string			m_geom_filed_name;
		std::string			m_uuid;
		augeGeometryType	m_geom_type;
		WorkspacePgs*		m_pWorkspace;
		GFields*			m_pFields;
	};
}

#endif //__AUGE_FEATURECLASS_PGS_H__
