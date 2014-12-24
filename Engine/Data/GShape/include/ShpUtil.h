#ifndef __AUGE_UTIL_SHP_H__
#define __AUGE_UTIL_SHP_H__

#include "shapefil.h"
#include "AugeCore.h"
#include "AugeGeometry.h"

namespace auge
{

	class ShpUtil
	{
	public:
		ShpUtil();
		virtual ~ShpUtil();
	public:
		//augeFieldType		type_shp_2_gbsde(int iType);
		//DBFFieldType		type_gbsde_2_shp(augeFieldType irsaTye);
		//augeGeometryType	type_geom_shp_2_gbsde(int iType);
		int					type_geom_gbsde_2_shp(augeGeometryType irsaType);


		static int	GetWKBLength(SHPObject* pSHPObject);
		static int	GetWKBLenofPoint(SHPObject *pSHPObject);
		static int	GetWKBLenofLine(SHPObject *pSHPObject);
		static int	GetWKBLenofPolygon(SHPObject *pSHPObject);
		static int	GetWKBLenofMPoint(SHPObject *pSHPObject);
		static int	GetWKBLenofMultiLine(SHPObject *pSHPObject);
		static int GetWKBLenofMultiPolygon(SHPObject *pSHPObject);

		static g_uchar* SHPObject_2_WKB(SHPObject *pSHPObject);
		static int SHPObject_2_WKB(SHPObject *pSHPObject, g_uchar* pWKB, int iWKBLen);
		static int WritePoint(SHPObject *pSHPObject, char* pWKB, int iWKBLen);
		static int WriteLine(SHPObject *pSHPObject, char* pWKB, int iWKBLen);
		static int WritePolygon(SHPObject *pSHPObject, char* pWKB, int iWKBLen);

		static int WriteMultiLine(SHPObject *pSHPObject, char* pWKB, int iWKBLen);
		static int WriteMultiPolygon(SHPObject *pSHPObject, char* pWKB, int iWKBLen);

		static int WritePoint(SHPObject *pSHPObject, WKBPoint* pWKBPoint);
		static int WriteLine(SHPObject *pSHPObject, WKBLineString* pWKBLineString);
		static int WritePolygon(SHPObject *pSHPObject, WKBPolygon* pWKBPolygon);
		static int WriteMultiPoint(SHPObject *pSHPObject, WKBMultiPoint* pWKBMultiPoint);
		static int WriteMultiLine(SHPObject *pSHPObject, WKBMultiLineString* pWKBMultiLineString);
		static int WriteMultiPolygon(SHPObject *pSHPObject, WKBMultiPolygon* pWKBMultiPolygon);

		static SHPObject* CreateSHPObjectFromWKB(const g_uchar* pWKB, const int iWKBLen);
		static SHPObject* CreateSHPPointFromWKB(WKBPoint* pWKBPoint);
		static SHPObject* CreateSHPLineStringFromWKB(WKBLineString* pWKBLineString);
		static SHPObject* CreateSHPLineStringFromWKB(WKBMultiLineString* pWKBMultiLineString);
		static SHPObject* CreateSHPPolygonFromWKB(WKBPolygon* pWKBPolygon);
		static SHPObject* CreateSHPMultiPointFromWKB(const g_uchar* pWKB, const int iWKBLen);
		static SHPObject* CreateSHPMultiLineStringFromWKB(const g_uchar* pWKB, const int iWKBLen);
		static SHPObject* CreateSHPMultiPolygonFromWKB(const g_uchar* pWKB, const int iWKBLen);
		static SHPObject* CreateSHPGeometryCollectionFromWKB(const char* pWKB, const int iWKBLen);

		static void GetMultiLineStringMeta(WKBMultiLineString* pWKBMultiLineString, int* pnParts, int* pnVertices);
		static void GetMultiPolygonMeta(WKBMultiPolygon* pWKBMultiPolygon, int* pnParts, int* pnVertices);

		//	int FindDefaultGeometryField(AgFeatureClassInfo* pFeatureClassInfo);

		static bool IsSimplePolygon(SHPObject* pSHPObject);
		static bool IsClockwise(float x0, float y0,float x1, float y1,float x2, float y2);
		static bool HasAntiClockRing(SHPObject* pSHPObject);
		static int  FindNextClockwiseRing(SHPObject* pSHPObject, int cur_ring_id);

		//////////////////////////////////////////////////////////////////////////
		//	AgField* CreateField(const char* szName, const char* szAlias, augeFieldType type, int nLength = 4, int nPrecision = 0);
		//	AgField* CreateField(const char* szName, const char* szAlias, augeGeometryType iGeomType, AgEnvelope &extent, long lSrid=0, bool isDefault=false);
	};

}

#endif //__AUGE_UTIL_SHP_H__
