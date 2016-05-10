#ifndef __AUGE_FEATURE_CLASS_BYD_H__
#define __AUGE_FEATURE_CLASS_BYD_H__

#include "AugeFeature.h"
#include <string>

#include <cppi.h>

namespace auge
{
	class GField;
	class GFields;
	class WorkspaceByd;
	
	class FeatureClassByd : public FeatureClass
	{
		friend class FeatureByd;
		friend class FeatureCursorByd;
		friend class FeatureInsertCommandByd;
		friend class SQLBuilder;

	public:
		FeatureClassByd();
		virtual ~FeatureClassByd();
	public:
		virtual const char*			GetName();
		virtual const char*			GetAlias();
		virtual augeDataSetType		GetType();
		virtual g_uint				GetSRID();
		virtual GEnvelope&			GetExtent();
		virtual const char*			GetUUID();

		virtual GFields*			GetFields();
		virtual GField*				GetField(const char* name);
		virtual RESULTCODE			AddField(const char* name, augeFieldType type, g_uint width=32);
		virtual RESULTCODE			RemoveField(const char* name);

		virtual g_uint				GetCount();
		virtual g_uint				GetCount(GEnvelope& extent);
		virtual g_uint				GetCount(GFilter* pFilter);

		virtual const char*			GetSize();

		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GEnvelope& extent, augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GFilter* pFilter, augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GQuery* pQuery, augeCursorType type=augeStaticCursor);

		virtual EnumValue*			GetUniqueValue(const char* field, augeOrderType order=augeOrderAsc);
		virtual EnumValue*			GetMinMaxValue(const char* field);

		virtual RESULTCODE			UpdateFeature(EnumString* pFieldNames, EnumValue* pValues, GFilter* pFilter);
		virtual RESULTCODE			RemoveFeature(GFilter* pFilter);

		virtual RESULTCODE			BuildSpatialIndex();

		virtual FeatureInsertCommand* CreateInsertCommand();

		virtual Feature*			NewFeature();
		virtual RESULTCODE			Refresh();

	public:
		bool		Create(const char* name, WorkspaceByd* pWorkspace);
		bool		Create(const char* name, WorkspaceByd* pWorkspace, CPPIResultSet* pResultSet);
		GFields*	CreateFields();
		GFields*	CreateFields(CPPIResultSet* pResultSet);
		GField*		CreateField(CPPIField* pbydField);
		bool		GetGeometryMeta();

		augeGeometryType	ParseGeometryType(int type);

	private:
		std::string m_name;
		std::string m_geom_field;
		std::string m_srs_name;		
		std::string m_uuid;
		g_uint		m_srid;
		g_uint		m_dimension;
		GEnvelope	m_extent;
		augeGeometryType m_geom_type;

		GFields			*m_pFields;	
		WorkspaceByd	*m_pWorkspace;
	};
}

#endif //__AUGE_FEATURE_CLASS_BYD_H__
