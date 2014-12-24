#include "GeometryFactoryImpl.h"
#include "wkb.h"
#include "GPointWKB.h"
#include "GLineStringWKB.h"
#include "GPolygonWKB.h"
#include "GMultiPointWKB.h"
#include "GMultiLineStringWKB.h"
#include "GMultiPolygonWKB.h"

#include "GeometryUtilImpl.h"
#include "WKTReader.h"

#include "AugeCore.h"

namespace auge
{
	GeometryFactory* augeGetGeometryFactoryInstance()
	{
		static GeometryFactoryImpl g_geometryFactory;
		return &g_geometryFactory;
	}

	GeometryFactoryImpl::GeometryFactoryImpl()
	{

	}

	GeometryFactoryImpl::~GeometryFactoryImpl()
	{

	}

	Geometry* GeometryFactoryImpl::CreateGeometryFromWKB(g_uchar* wkb, bool attach/*=false*/)
	{
		if(wkb==NULL)
		{
			return NULL;
		}

		Geometry* pGeometry = NULL;
		g_uchar* wkbd = NULL;

		if(attach)
		{
			wkbd = wkb;
		}
		else
		{
			GeometryUtilImpl gutil;
			g_uint wkbSize = gutil.GetWKBSize(wkb);
			if(wkbSize>0)
			{
				wkbd = (g_uchar*)auge_malloc(1, wkbSize);
				if(wkbd!=NULL)
				{
					memcpy(wkbd, wkb, wkbSize);
				}
			}
		}
		if(wkbd==NULL)
		{
			return NULL;
		}

		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			{
				GPointWKB* ptr = new GPointWKB();
				ptr->Create(wkb, attach);
				pGeometry = ptr;
			}
			break;
		case wkbLineString:
			{
				GLineStringWKB* ptr = new GLineStringWKB();
				ptr->Create(wkb, attach);
				pGeometry = ptr;
			}
			break;
		case wkbPolygon:
			{
				GPolygonWKB* ptr = new GPolygonWKB();
				ptr->Create(wkb, attach);
				pGeometry = ptr;
			}
			break;
		case wkbMultiPoint:
			{
				GMultiPointWKB* ptr = new GMultiPointWKB();
				ptr->Create(wkb, attach);
				pGeometry = ptr;
			}
			break;
		case wkbMultiLineString:
			{
				GMultiLineStringWKB* ptr = new GMultiLineStringWKB();
				ptr->Create(wkb, attach);
				pGeometry = ptr;
			}
			break;
		case wkbMultiPolygon:
			{
				GMultiPolygonWKB* ptr = new GMultiPolygonWKB();
				ptr->Create(wkb, attach);
				pGeometry = ptr;
			}
			break;
		}
		return pGeometry;
	}

	Geometry* GeometryFactoryImpl::CreateGeometryFromWKT(const char* wkt)
	{
		if(wkt==NULL)
		{
			return NULL;
		}

		WKTReader reader;
		g_uchar* wkb = reader.Read(wkt);
		if(wkb==NULL)
		{
			return NULL;
		}
		return CreateGeometryFromWKB(wkb,true);
	}
}