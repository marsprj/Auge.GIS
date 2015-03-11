#ifndef __AUGE_FEATURE_CLASS_SHP_H__
#define __AUGE_FEATURE_CLASS_SHP_H__

#include "AugeFeature.h"
#include "AugeField.h"
#include <string>

#include "shapefil.h"

namespace auge
{
	class WorkspaceShp;

	class FeatureClassShp : public FeatureClass
	{
		friend class FeatureShp;

	public:
		FeatureClassShp();
		virtual ~FeatureClassShp();
	public:
		virtual const char*			GetName();
		virtual g_uint				GetSRID();
		virtual GEnvelope&			GetExtent();

		virtual GFields*			GetFields();
		virtual GField*				GetField(const char* name);

		virtual FeatureCursor*		Query(augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GEnvelope& extent, augeCursorType type=augeStaticCursor);
		virtual FeatureCursor*		Query(GFilter* pFilter, augeCursorType type=augeStaticCursor);

		virtual RESULTCODE			RemoveFeature(GFilter* pFilter);

		virtual FeatureInsertCommand* CreateInsertCommand();

		virtual Feature*			NewFeature();

		virtual void				Release();

	public:
		bool	Create(const char* name, WorkspaceShp* pWorkspace);

	private:
		bool	OpenSHPFile(const char* name, WorkspaceShp* pWorkspace);
		void	CloseSHPFile();

		bool	GetMetaData();
		bool	CreateFields();

		augeFieldType		GetFieldType(DBFFieldType dbfType);
		augeGeometryType	GetGeometryType(int shpType);
		

	private:
		std::string			m_name;
		WorkspaceShp*		m_pWorkspace;
		SHPHandle			m_pSHPHandle;
		DBFHandle			m_pDBFHandle;
		augeGeometryType	m_geom_type;
		GFields*			m_pFields;
		GEnvelope			m_extent;
		g_uint				m_srid;
		g_int				m_feature_count;
	};
}

#endif //__AUGE_FEATURE_CLASS_SHP_H__
