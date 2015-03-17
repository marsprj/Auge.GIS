#include "GMLWriterImpl.h"
#include "AugeXML.h"
#include <sstream>

namespace auge
{
	#define AUGE_GML_NAMESPACE	"http://www.opengis.net/gml"
	#define	AUGE_GML_SPACENAME		"gml"

	GMLWriterImpl::GMLWriterImpl()
	{

	}

	GMLWriterImpl::~GMLWriterImpl()
	{

	}

	RESULTCODE GMLWriterImpl::Write(XElement* pxParent, const g_uchar* wkb, g_int srid, const char* version)
	{
		if(wkb==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			rc = AsGML(pxParent, (WKBPoint*)wkb, srid);
			break;
		case wkbLineString:
			rc = AsGML(pxParent, (WKBLineString*)wkb, srid);
			break;
		case wkbPolygon:
			rc = AsGML(pxParent, (WKBPolygon*)wkb, srid);
			break;
		case wkbMultiPoint:
			rc = AsGML(pxParent, (WKBMultiPoint*)wkb, srid);
			break;
		case wkbMultiLineString:
			rc = AsGML(pxParent, (WKBMultiLineString*)wkb, srid);
			break;
		case wkbMultiPolygon:
			rc = AsGML(pxParent, (WKBMultiPolygon*)wkb, srid);
			break;
		}

		return AG_SUCCESS;
	}

	RESULTCODE GMLWriterImpl::Write(XElement* pxParent, Geometry* pGeometry, g_int srid, const char* version)
	{
		return Write(pxParent, pGeometry->AsBinary(), srid, version);
	}

	void GMLWriterImpl::Release()
	{
		delete this;
	}

	/*
	<gml:Point>
		<gml:coordinates>1,2</gml:coordinates>
	</gml:Point>
	*/
	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent,WKBPoint* pWKBPoint, g_int srid)
	{
		char str[AUGE_MSG_MAX] = {0};		

		XElement* pxPoint = pxParent->AddChild("Point",NULL);
		pxPoint->SetNamespaceDeclaration(AUGE_GML_NAMESPACE,AUGE_GML_SPACENAME);
		pxPoint->SetNamespace("gml");
		
		g_snprintf(str, AUGE_MSG_MAX, "%s/srs/epsg.xml#%d",AUGE_GML_NAMESPACE, srid);
		pxPoint->SetAttribute("srsName", str, NULL);

		g_snprintf(str, AUGE_MSG_MAX, "%f,%f", pWKBPoint->point.x, pWKBPoint->point.y);
		XElement* pxCoordinates = pxPoint->AddChild("coordinates", AUGE_GML_SPACENAME);
		pxCoordinates->AddChildText(str);

		return AG_SUCCESS;
	}

	/*
	<gml:LineString>
		<gml:coordinates>1,2 3,4</gml:coordinates>
	</gml:LineString>
	*/
	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent,WKBLineString* pWKBLineString, g_int srid)
	{
		char str[AUGE_MSG_MAX] = {0};		

		XElement* pxLineString = pxParent->AddChild("LineString",NULL);
		pxLineString->SetNamespaceDeclaration(AUGE_GML_NAMESPACE,AUGE_GML_SPACENAME);
		pxLineString->SetNamespace("gml");

		g_snprintf(str, AUGE_MSG_MAX, "%s/srs/epsg.xml#%d",AUGE_GML_NAMESPACE, srid);
		pxLineString->SetAttribute("srsName", str, NULL);

		std::ostringstream ss;
		Point* pt = &(pWKBLineString->points[0]);
		for(int i=0; i<pWKBLineString->numPoints; i++, pt++)
		{
			if(i)
			{
				ss<<" ";
			}
			ss<<pt->x<<","<<pt->y;
		}

		std::string coordinates = ss.str();

		XElement* pxCoordinates = pxLineString->AddChild("coordinates", AUGE_GML_SPACENAME);
		pxLineString->AddChildText(ss.str().c_str());

		return AG_SUCCESS;
	}

	/*
	<gml:Polygon>
		<gml:outerBoundaryIs>
			<gml:LinearRing>
				<gml:coordinates>0,0 0,4 4,4 4,0 0,0</gml:coordinates>
			</gml:LinearRing>
		</gml:outerBoundaryIs>
		<gml:innerBoundaryIs>
			<gml:LinearRing>
				<gml:coordinates>1,1 1,3 3,4 3,1 1,1</gml:coordinates>
			</gml:LinearRing>
		</gml:innerBoundaryIs>
	</gml:Polygon>
	*/
	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent,WKBPolygon* pWKBPolygon, g_int srid)
	{	
		XElement* pxPolygon = pxParent->AddChild("Polygon",NULL);
		pxPolygon->SetNamespaceDeclaration(AUGE_GML_NAMESPACE,AUGE_GML_SPACENAME);
		pxPolygon->SetNamespace("gml");

		char str[AUGE_MSG_MAX] = {0};		
		g_snprintf(str, AUGE_MSG_MAX, "%s/srs/epsg.xml#%d",AUGE_GML_NAMESPACE, srid);
		pxPolygon->SetAttribute("srsName", str, NULL);

		int numRings = 0;
		int numPoints = 0;
		int i=0, j=0;
		int outBoundIndex = FindOutBoundIndex(pWKBPolygon);

		char* ptr = NULL;
		LinearRing* pLinearRing = NULL;

		numRings = pWKBPolygon->numRings;
		pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			if(outBoundIndex==i)
			{
				// <gml:xxxrBoundaryIs>
				XElement* pxBoundary = NULL;
				pxBoundary = pxPolygon->AddChild("outerBoundaryIs",AUGE_GML_SPACENAME);
				AsGML(pxBoundary, pLinearRing);

				break;
			}
			ptr = (char*)pLinearRing;
			pLinearRing = (LinearRing*)(ptr + CalWKBLinearRingLen(pLinearRing));
		}

		pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
		for(i=0; i<numRings; i++)
		{
			if(outBoundIndex!=i)
			{
				// <gml:innerBoundaryIs>
				XElement* pxBoundary = NULL;
				pxBoundary = pxPolygon->AddChild("outerBoundaryIs",AUGE_GML_SPACENAME);
				AsGML(pxBoundary, pLinearRing);
			}

			ptr = (char*)pLinearRing;
			pLinearRing = (LinearRing*)(ptr + CalWKBLinearRingLen(pLinearRing));
		}

		return AG_SUCCESS;
	}

	/*
	<gml:MultiPoint>
		<gml:pointMember>
			<gml:Point>
				<gml:coordinates>1,2</gml:coordinates>
			</gml:Point>
		</gml:pointMember>
		<gml:pointMember>
			<gml:Point>
				<gml:coordinates>3,4</gml:coordinates>
			</gml:Point>
		</gml:pointMember>
	</gml:MultiPoint>
	*/
	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent,WKBMultiPoint* pWKBMultiPoint, g_int srid)
	{
		XElement* pxPoint  = NULL;
		XElement* pxMember = NULL;
		XElement* pxCoordinates = NULL;
		XElement* pxMultiPoint = pxParent->AddChild("MultiPoint",NULL);
		pxMultiPoint->SetNamespaceDeclaration(AUGE_GML_NAMESPACE,AUGE_GML_SPACENAME);
		pxMultiPoint->SetNamespace("gml");

		char str[AUGE_MSG_MAX] = {0};	
		g_snprintf(str, AUGE_MSG_MAX, "%s/srs/epsg.xml#%d",AUGE_GML_NAMESPACE, srid);
		pxMultiPoint->SetAttribute("srsName", str, NULL);

		int numPoints = pWKBMultiPoint->numPoints;
		WKBPoint* pWKBPoint = &(pWKBMultiPoint->points[0]);
		for(int i=0; i<numPoints; i++, pWKBPoint++)
		{
			g_snprintf(str, AUGE_MSG_MAX, "%f,%f",pWKBPoint->point.x,pWKBPoint->point.y);
			pxMember = pxMultiPoint->AddChild("pointMember", AUGE_GML_SPACENAME);
			pxPoint  = pxMember->AddChild("Point", AUGE_GML_SPACENAME);
			pxCoordinates = pxPoint->AddChild("coordinates", AUGE_GML_SPACENAME);
			pxCoordinates->AddChildText(str);
		}

		return AG_SUCCESS;
	}

	/*
	<gml:MultiLineString>
		<gml:lineStringMember>
			<gml:LineString>
				<gml:coordinates>1,2 3,4</gml:coordinates>
			</gml:LineString>
		</gml:lineStringMember>
		<gml:lineStringMember>
			<gml:LineString>
				<gml:coordinates>5,6 7,8</gml:coordinates>
			</gml:LineString>
		</gml:lineStringMember>
	</gml:MultiLineString>
	*/
	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent,WKBMultiLineString* pWKBMultiLineString, g_int srid)
	{
		XElement* pxMultiLineString = pxParent->AddChild("MultiLineString",NULL);
		pxMultiLineString->SetNamespaceDeclaration(AUGE_GML_NAMESPACE,AUGE_GML_SPACENAME);
		pxMultiLineString->SetNamespace("gml");

		char str[AUGE_MSG_MAX] = {0};
		g_snprintf(str, AUGE_MSG_MAX, "%s/srs/epsg.xml#%d",AUGE_GML_NAMESPACE, srid);
		pxMultiLineString->SetAttribute("srsName", str, NULL);

		int numLineStrings = 0;
		char* ptr = NULL;
		WKBLineString* pWKBLineString = NULL;

		numLineStrings = pWKBMultiLineString->numLineStrings;
		pWKBLineString = (WKBLineString*)(&(pWKBMultiLineString->lineStrings[0]));

		XElement* pxMember = NULL;
		XElement* pxLineString = NULL;
		for(int i=0; i<numLineStrings; i++)
		{
			pxMember = pxMultiLineString->AddChild("lineStringMember", AUGE_GML_SPACENAME);

			AsGML(pxMember, pWKBLineString);

			ptr = (char*)(pWKBLineString);
			pWKBLineString = (WKBLineString*)(ptr + CalWKBLineStringLen(pWKBLineString));
		}

		return AG_SUCCESS;
	}

	/*
	<gml:MultiPolygon>
		<gml:polygonMember>
			<gml:Polygon>
				<gml:outerBoundaryIs>
					<gml:LinearRing>
						<gml:coordinates>0,0 0,1 1,1 1,0 0,0</gml:coordinates>
					</gml:LinearRing>
				</gml:outerBoundaryIs>
			</gml:Polygon>
		</gml:polygonMember>
		<gml:polygonMember>
			<gml:Polygon>
				<gml:outerBoundaryIs>
					<gml:LinearRing>
						<gml:coordinates>2,2 2,3 3,3 3,2 2,2</gml:coordinates>
					</gml:LinearRing>
				</gml:outerBoundaryIs>
			</gml:Polygon>
		</gml:polygonMember>
	</gml:MultiPolygon>
	*/
	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent,WKBMultiPolygon* pWKBMultiPolygon, g_int srid)
	{
		char str[AUGE_MSG_MAX] = {0};		

		XElement* pxWKBMultiPolygon = pxParent->AddChild("MultiPolygon",NULL);
		pxWKBMultiPolygon->SetNamespaceDeclaration(AUGE_GML_NAMESPACE,AUGE_GML_SPACENAME);
		pxWKBMultiPolygon->SetNamespace("gml");

		g_snprintf(str, AUGE_MSG_MAX, "%s/srs/epsg.xml#%d",AUGE_GML_NAMESPACE, srid);
		pxWKBMultiPolygon->SetAttribute("srsName", str, NULL);

		int numPolygons = 0;
		char* ptr = NULL;
		WKBPolygon* pWKBPolygon = NULL;
		XElement* pxMember = NULL;
		XElement* pxPolygon = NULL;

		numPolygons = pWKBMultiPolygon->numPolygons;
		pWKBPolygon = (WKBPolygon*)(&(pWKBMultiPolygon->polygons[0]));
		for(int i=0; i<numPolygons; i++)
		{
			pxMember = pxWKBMultiPolygon->AddChild("polygonMember", AUGE_GML_SPACENAME);
			AsGML(pxMember, pWKBPolygon);

			ptr = (char*)pWKBPolygon;
			pWKBPolygon = (WKBPolygon*)(ptr + CalWKBPolygonLen(pWKBPolygon));
		}

		return AG_SUCCESS;
	}

	/*
	<gml:LinearRing>
		<gml:coordinates>0,0 0,1 1,1 1,0 0,0</gml:coordinates>
	</gml:LinearRing>
	*/
	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent, LinearRing* pLinearRing)
	{
		std::ostringstream os;
		int numPoints = pLinearRing->numPoints;
		auge::Point* pt = &(pLinearRing->points[0]);
		
		for(int i=0; i<numPoints; i++,pt++)
		{
			if(i)	os<<" ";
			os<<pt->x<<","<<pt->y;
		}

		XElement* pxLinearRing = pxParent->AddChild("LinearRing",AUGE_GML_SPACENAME);
		pxLinearRing->AddChildText(os.str().c_str());

		return AG_SUCCESS;
	}

	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent, WKBLineString* pLineString)
	{
		std::ostringstream os;
		int numPoints = pLineString->numPoints;
		auge::Point* pt = &(pLineString->points[0]);

		for(int i=0; i<numPoints; i++,pt++)
		{
			if(i)	os<<" ";
			os<<pt->x<<","<<pt->y;
		}

		XElement* pxLineString = pxParent->AddChild("LineString",AUGE_GML_SPACENAME);
		pxLineString->AddChildText(os.str().c_str());

		return AG_SUCCESS;
	}

	RESULTCODE GMLWriterImpl::AsGML(XElement* pxParent,WKBPolygon* pWKBPolygon)
	{	
		XElement* pxPolygon = pxParent->AddChild("Polygon",NULL);

		int numRings = 0;
		int numPoints = 0;
		int i=0, j=0;
		int outBoundIndex = FindOutBoundIndex(pWKBPolygon);

		char* ptr = NULL;
		LinearRing* pLinearRing = NULL;

		numRings = pWKBPolygon->numRings;
		pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			if(outBoundIndex==i)
			{
				// <gml:xxxrBoundaryIs>
				XElement* pxBoundary = NULL;
				pxBoundary = pxPolygon->AddChild("outerBoundaryIs",AUGE_GML_SPACENAME);
				AsGML(pxBoundary, pLinearRing);

				break;
			}
			ptr = (char*)pLinearRing;
			pLinearRing = (LinearRing*)(ptr + CalWKBLinearRingLen(pLinearRing));
		}

		pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
		for(i=0; i<numRings; i++)
		{
			if(outBoundIndex!=i)
			{
				// <gml:innerBoundaryIs>
				XElement* pxBoundary = NULL;
				pxBoundary = pxPolygon->AddChild("outerBoundaryIs",AUGE_GML_SPACENAME);
				AsGML(pxBoundary, pLinearRing);
			}

			ptr = (char*)pLinearRing;
			pLinearRing = (LinearRing*)(ptr + CalWKBLinearRingLen(pLinearRing));
		}

		return AG_SUCCESS;
	}

	int GMLWriterImpl::CalWKBLinearRingLen(LinearRing* pLinearRing)
	{
		return (sizeof(g_int32)+sizeof(Point)*pLinearRing->numPoints);
	}

	int GMLWriterImpl::CalWKBLineStringLen(WKBLineString* pWKBLineString)
	{
		int len = 0;
		len += sizeof(unsigned char);
		len += sizeof(g_int32) << 1;
		len += sizeof(Point) * pWKBLineString->numPoints;
		return len;
	}

	int GMLWriterImpl::CalWKBPolygonLen(WKBPolygon* pWKBPolygon)
	{
		int len = 0;

		len += sizeof(unsigned char);
		len += sizeof(g_int32) << 1;

		int ringLen = 0;
		int numRings = pWKBPolygon->numRings;
		unsigned char* pLinearRing = NULL;
		pLinearRing = (unsigned char*)(&(pWKBPolygon->rings[0]));
		for(int i=0; i<numRings; i++)
		{
			ringLen = CalWKBLinearRingLen((LinearRing*)pLinearRing);
			len += ringLen;
			pLinearRing += ringLen;
		}

		return len;
	}

	int	GMLWriterImpl::FindOutBoundIndex(WKBPolygon* pWKBPolygon)
	{
		if(pWKBPolygon==NULL)
			return -1;

		if(pWKBPolygon->numRings==1)
			return 0;

		int out_bound_index=0;
		int numRings = 0;
		double dmin=0.0f, dmax=0.0f, stretch=0, d=0;
		LinearRing* pLinearRing = NULL;

		pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
		numRings = pWKBPolygon->numRings;
		for(int i=0; i<numRings; i++)
		{
			GetBound(pLinearRing, 0, dmin, dmax);
			d = dmax-dmin;
			if(d>stretch)
				out_bound_index = i;
		}

		return out_bound_index;
	}

	void GMLWriterImpl::GetBound(LinearRing* pLinearRing, int dimension, double& dmin, double& dmax)
	{
		double v=0.0f;
		double v1 = 1e308, v2 =-1e308;
		int numPoints = pLinearRing->numPoints;
		Point* pt = (Point*)(&(pLinearRing->points[0]));
		for(int i=0; i<numPoints; ++i)
		{
			v = (dimension==0) ? pt->x : pt->y;
			if(v<v1)
				v1 = v;
			else if(v>v2)
				v2 = v;
		}
		dmin = v1, dmax = v2;

	}

}