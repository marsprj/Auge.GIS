#ifndef __AUGE_FEATURE_INSERT_COMMAND_SHP_H__
#define __AUGE_FEATURE_INSERT_COMMAND_SHP_H__

#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "shapefil.h"

namespace auge
{
	class FeatureClassShp;
	class ConnectionShp;

	class FeatureInsertCommandShp : public FeatureInsertCommand
	{
	public:
		FeatureInsertCommandShp(FeatureClassShp* pFeatureClass);
		virtual ~FeatureInsertCommandShp();
	public:
		virtual	RESULTCODE	Prepare(FeatureClass* pFeatureClass);
		virtual RESULTCODE	Insert(Feature* pFeature);
		virtual RESULTCODE	Commit();

	private:
		SHPObject* CreateSHPObjectFromWKB(const g_uchar* pWKB, const int iWKBLen);
		SHPObject* CreateSHPPointFromWKB(WKBPoint* pWKBPoint);
		SHPObject* CreateSHPLineStringFromWKB(WKBLineString* pWKBLineString);
		SHPObject* CreateSHPLineStringFromWKB(WKBMultiLineString* pWKBMultiLineString);
		SHPObject* CreateSHPPolygonFromWKB(WKBPolygon* pWKBPolygon);
		SHPObject* CreateSHPMultiPointFromWKB(const g_uchar* pWKB, const int iWKBLen);
		SHPObject* CreateSHPMultiLineStringFromWKB(const g_uchar* pWKB, const int iWKBLen);
		SHPObject* CreateSHPMultiPolygonFromWKB(const g_uchar* pWKB, const int iWKBLen);
		SHPObject* CreateSHPGeometryCollectionFromWKB(const g_uchar* pWKB, const int iWKBLen);

		void GetMultiLineStringMeta(WKBMultiLineString* pWKBMultiLineString, int* pnParts, int* pnVertices);
		void GetMultiPolygonMeta(WKBMultiPolygon* pWKBMultiPolygon, int* pnParts, int* pnVertices);

		DBFFieldType GetDbfType(augeFieldType type);
		void WriteFieldValue(int shp_id, int dbf_index, augeFieldType type, GValue* pValue, const char* szEncoding=NULL);

		int			  GetFieldInnerIndex(int index, int fid, int gindex) const;
		void		  Swap(int& val1, int& val2) const;

	private:
		FeatureClassShp	*m_pFeatureClass;
	};
}

#endif //__AUGE_FEATURE_INSERT_COMMAND_SHP_H__
