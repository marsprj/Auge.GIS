#include "GeometryUtilImpl.h"

namespace auge
{
	GeometryUtil* augeGetGeometryUtilInstance()
	{
		static GeometryUtilImpl g_geometryUtil;
		return &g_geometryUtil;
	}

	GeometryUtilImpl::GeometryUtilImpl()
	{

	}

	GeometryUtilImpl::~GeometryUtilImpl()
	{

	}

	g_uint GeometryUtilImpl::GetWKBSize(const g_uchar* wkb)
	{
		if(wkb==NULL)
		{
			return 0;
		}

		g_uint wkbSize = 0;

		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			wkbSize = GetWKBSize((WKBPoint*)wkb);
			break;
		case wkbLineString:
			wkbSize = GetWKBSize((WKBLineString*)wkb);
			break;
		case wkbPolygon:
			wkbSize = GetWKBSize((WKBPolygon*)wkb);
			break;
		case wkbMultiPoint:
			wkbSize = GetWKBSize((WKBMultiPoint*)wkb);
			break;
		case wkbMultiLineString:
			wkbSize = GetWKBSize((WKBMultiLineString*)wkb);
			break;
		case wkbMultiPolygon:
			wkbSize = GetWKBSize((WKBMultiPolygon*)wkb);
			break;
		}

		return wkbSize;
	}

	g_uint GeometryUtilImpl::GetWKBSize(WKBPoint* pPoint)
	{
		int wkbSize = sizeof(WKBPoint);
		return wkbSize;
	}

	g_uint GeometryUtilImpl::GetWKBSize(WKBLineString* pLineString)
	{
		int wkbSize   = sizeof(WKBLineString)-sizeof(Point);
		int numPoints = pLineString->numPoints;
		wkbSize  += sizeof(Point)*numPoints;
		return wkbSize;
	}

	g_uint GeometryUtilImpl::GetWKBSize(WKBPolygon* pPolygon)
	{
		int   i,itemSize;
		int   wkbSize  = sizeof(WKBPolygon) - sizeof(LinearRing);
		int   numRings = pPolygon->numRings;
		char *ring     = (char*)pPolygon->rings;
		for(i=0;i<numRings;i++)
		{
			itemSize = GetWKBSize((LinearRing*)ring);
			ring    += itemSize;
			wkbSize += itemSize;
		}
		return wkbSize;
	}

	g_uint GeometryUtilImpl::GetWKBSize(WKBMultiPoint* pMultiPoint)
	{
		int wkbSize = sizeof(WKBMultiPoint) - sizeof(WKBPoint);
		int numPoints = pMultiPoint->numPoints;
		wkbSize  += sizeof(Point)*numPoints;
		return wkbSize;
	}

	g_uint GeometryUtilImpl::GetWKBSize(WKBMultiLineString* pMultiLineString)
	{
		int i,itemSize;
		int wkbSize       = sizeof(WKBMultiLineString) - sizeof(WKBLineString);
		int numLineString = pMultiLineString->numLineStrings;
		char *lineStrings = (char*)pMultiLineString->lineStrings;
		for(i=0;i<numLineString;i++)
		{
			itemSize    = GetWKBSize((WKBLineString*)lineStrings);
			lineStrings+= itemSize;
			wkbSize    += itemSize;
		}
		return wkbSize;
	}

	g_uint GeometryUtilImpl::GetWKBSize(WKBMultiPolygon* pMultiPolygon)
	{
		int   i,itemSize;
		int   wkbSize     = sizeof(WKBMultiPolygon) - sizeof(WKBPolygon);
		int   numPolygons = pMultiPolygon->numPolygons;
		char *polygon     = (char*)pMultiPolygon->polygons;
		for(i=0;i<numPolygons;i++)
		{
			itemSize  = GetWKBSize((WKBPolygon*)polygon);
			polygon  += itemSize;
			wkbSize  += itemSize;
		}
		return wkbSize;
	}

	g_uint GeometryUtilImpl::GetWKBSize(LinearRing* pLinearRing)
	{
		int wkbSize   = sizeof(LinearRing)-sizeof(Point);
		int numPoints = pLinearRing->numPoints;
		wkbSize  += sizeof(Point)*numPoints;
		return wkbSize;
	}
}
