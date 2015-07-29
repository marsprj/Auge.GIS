#include "GMLReaderImpl.h"

namespace auge
{
	GMLReaderImpl::GMLReaderImpl()
	{

	}

	GMLReaderImpl::~GMLReaderImpl()
	{

	}

	Geometry* GMLReaderImpl::Read(const char* gml)
	{
		if(gml==NULL)
		{
			return NULL;
		}
		XParser parser;
		XDocument* pxDoc = parser.ParseMemory(gml);
		if(pxDoc==NULL)
		{
			return NULL;
		}

		XElement* pxGeometry = pxDoc->GetRootNode();
		if(pxGeometry==NULL)
		{
			pxDoc->Close();
			pxDoc->Release();
			return NULL;
		}

		Geometry* pGeometry = NULL;
		pGeometry = Read(pxGeometry);

		pxDoc->Close();
		pxDoc->Release();
		return pGeometry;
	}

	Geometry* GMLReaderImpl::Read(XElement *pxGeometry)
	{
		if(pxGeometry==NULL)
		{
			return NULL;
		}

		int wkblen = CalWKBLengthFromGML(pxGeometry);
		if(wkblen==0)
		{
			return NULL;
		}

		g_uchar* wkb = (g_uchar*)calloc(wkblen, sizeof(g_uchar));
		if(wkb==NULL)
		{
			return NULL;
		}
		
		if(!CreateWKBFromGML(wkb, wkblen, pxGeometry))
		{
			free(wkb);
			return NULL;
		}

		Geometry* pGeometry = NULL;
		GeometryFactory* pGeometryFactory = NULL;
		pGeometryFactory = augeGetGeometryFactoryInstance();
		pGeometry = pGeometryFactory->CreateGeometryFromWKB(wkb);
		return pGeometry;
	}

	void GMLReaderImpl::Release()
	{
		delete this;
	}

	//////////////////////////////////////////////////////////////////////////

	inline
	int GMLReaderImpl::CalWKBPointLen(WKBPoint* pWKBPoint)
	{
		return (sizeof(unsigned char) + sizeof(g_int) + sizeof(Point));
	}

	inline
	int GMLReaderImpl::CalWKBLineStringLen(WKBLineString* pWKBLineString)
	{
		int len = 0;
		len += sizeof(unsigned char);
		len += sizeof(g_int) << 1;
		len += sizeof(Point) * pWKBLineString->numPoints;
		return len;
	}

	inline
	int GMLReaderImpl::CalWKBPolygonLen(WKBPolygon* pWKBPolygon)
	{
		int len = 0;
	
		len += sizeof(unsigned char);
		len += sizeof(g_int) << 1;
	
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

	//--------------------------------------------------------------------------//
	//                      WKB LEGNTH FROM GML  BEGIN                          //
	//--------------------------------------------------------------------------//
	int	GMLReaderImpl::CalWKBLengthFromGML(const char* szgml)
	{
		int wkblen = 0;

		if(szgml==NULL)
			return 0;
	
	//	
	//	xmlDocPtr doc = NULL;
	//	XNode* pRootNode = NULL;

	//	doc = xmlParseMemory(szgml, strlen(szgml));
	//	if(doc==NULL)
	//		return 0;

	//	pRootNode = xmlDocGetRootElement(doc);
	//#ifdef __DEBUG__
	//	cout<<pRootNode->name<<endl;
	//#endif

	//	wkblen = CalWKBLengthFromGML(pRootNode);

	//	xmlFreeDoc(doc);

		return wkblen;
	}

	int	GMLReaderImpl::CalWKBLengthFromGML(XElement* pxGeometry)
	{
		int wkblen = 0;

		if(pxGeometry==NULL)
			return 0;
		
		if(0==g_stricmp(pxGeometry->GetName(), "Point"))
		{
			wkblen = CalWKBLengthFromGMLPoint(pxGeometry);
		}
		else if(0==g_stricmp(pxGeometry->GetName(), "LineString"))
		{
			wkblen = CalWKBLengthFromGMLLineString(pxGeometry);
		}
		else if(0==g_stricmp(pxGeometry->GetName(), "Polygon"))
		{
			wkblen = CalWKBLengthFromGMLPolygon(pxGeometry);
		}
		else if(0==g_stricmp(pxGeometry->GetName(), "MultiPoint"))
		{
			wkblen = CalWKBLengthFromGMLMultiPoint(pxGeometry);
		}
		else if(0==g_stricmp(pxGeometry->GetName(), "MultiLineString"))
		{
			wkblen = CalWKBLengthFromGMLMultiLineString(pxGeometry);
		}
		else if(0==g_stricmp(pxGeometry->GetName(), "MultiPolygon"))
		{
			wkblen = CalWKBLengthFromGMLMultiPolygon(pxGeometry);
		}
		else
		{
			wkblen = 0;
		}
	
		return wkblen;
	}

	int GMLReaderImpl::CalWKBLengthFromGMLPoint(XNode* pxGeometry)
	{
		return 21;
	}

	/*
	<gml:LineString>
		<gml:coordinates>1,2 3,4</gml:coordinates>
	</gml:LineString>
	*/
	int GMLReaderImpl::CalWKBLengthFromGMLLineString(XElement* pxGeometry)
	{
		int wkblen = 0;

		if(pxGeometry==NULL)
			return 0;

		XNode* pxCoordinate = pxGeometry->GetFirstChild();
		if(pxCoordinate==NULL)
			return 0;

		const char* coords = pxCoordinate->GetContent();

		wkblen  = sizeof(unsigned char) + (sizeof(int) << 1);
		wkblen += CalWKBLengthFromGMLCoordinates(coords);

		return wkblen;
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
	// struct WKBPolygon {
	// 	unsigned char byteOrder;
	// 	int       wkbType;// = 3;
	// 	int       numRings;
	// 	LinearRing    rings[1];
	// };
	int GMLReaderImpl::CalWKBLengthFromGMLPolygon(XElement* pxGeometry)
	{
		int wkblen = 0;

		if(pxGeometry==NULL)
			return 0;

		XNodeSet *pxNodeSet = pxGeometry->GetChildren();
		if(pxNodeSet==NULL)
		{
			return 0;
		}

		XNode* pxBoundNode = NULL;
		XNode* pxLinearRingNode = NULL;

		pxNodeSet->Reset();

		while((pxBoundNode=pxNodeSet->Next())!=NULL)
		{
			pxLinearRingNode = pxBoundNode->GetFirstChild();
			if(pxLinearRingNode!=NULL)
			{
				wkblen += CalWKBLengthFromGMLLinearRing(pxLinearRingNode);
			}
		}

		pxNodeSet->Release();

		wkblen += sizeof(unsigned char) + (sizeof(int) << 1);
		return wkblen;
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
	// struct WKBMultiPoint {
	// 	unsigned char byteOrder;
	// 	int       wkbType;//=4;
	// 	int       numPoints;
	// 	WKBPoint      points[1];
	// };
	int GMLReaderImpl::CalWKBLengthFromGMLMultiPoint(XElement* pxGeometry)
	{
		int wkblen = 0;

		if(pxGeometry==NULL)
			return 0;

		XNodeSet *pxNodeSet = pxGeometry->GetChildren();
		if(pxNodeSet==NULL)
		{
			return 0;
		}

		XNode* pxPointMemberNode = NULL;
		XNode* pxPointNode = NULL;

		pxNodeSet->Reset();
		while((pxPointMemberNode=pxNodeSet->Next())!=NULL)
		{
			pxPointNode = pxPointMemberNode->GetFirstChild();
			if(pxPointNode!=NULL)
			{
				wkblen += CalWKBLengthFromGMLPoint(pxPointNode);
			}
		}

		pxNodeSet->Release();

		wkblen += sizeof(unsigned char) + (sizeof(int)<<1);
		
		return wkblen;
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
	// struct WKBMultiLineString {
	// 	unsigned char byteOrder;
	// 	int       wkbType;// = 5;
	// 	int       numLineStrings;
	// 	WKBLineString lineStrings[1];
	// };
	int GMLReaderImpl::CalWKBLengthFromGMLMultiLineString(XElement* pxGeometry)
	{
		int wkblen = 0;

		if(pxGeometry==NULL)
		{
			return 0;
		}

		XNode* pxLineStringNode = NULL;
		XNode* pxLineStringMemberNode = NULL;
		XNodeSet* pxNodeSet = pxGeometry->GetChildren();

		pxNodeSet->Reset();
		while((pxLineStringMemberNode=pxNodeSet->Next())!=NULL)
		{
			pxLineStringNode = pxLineStringMemberNode->GetFirstChild();
			if(pxLineStringNode!=NULL)
			{
				wkblen += CalWKBLengthFromGMLLineString(static_cast<XElement*>(pxLineStringNode));
			}
		}
		pxNodeSet->Release();	
		return wkblen;
	}

	int GMLReaderImpl::CalWKBLengthFromGMLMultiPolygon(XElement* pxGeometry)
	{
		int wkblen = 0;

		if(pxGeometry==NULL)
		{
			return 0;
		}

		XNode* pxPolygonNode = NULL;
		XNode* pxPolygonMemberNode = NULL;
		XNodeSet* pxNodeSet = NULL;

		pxNodeSet = pxGeometry->GetChildren();
		if(pxNodeSet==NULL)
		{
			return 0;
		}

		pxNodeSet->Reset();
		while((pxPolygonMemberNode=pxNodeSet->Next())!=NULL)
		{
			pxPolygonNode = pxPolygonMemberNode->GetFirstChild();
			if(pxPolygonNode!=NULL)
			{
				wkblen += CalWKBLengthFromGMLPolygon(static_cast<XElement*>(pxPolygonNode));
			}
		}

		pxNodeSet->Release();
	
		return wkblen;
	}

	// struct LinearRing {
	// 	int numPoints;
	// 	Point   points[1];
	// };
	/*
	<gml:LinearRing>
		<gml:coordinates>0,0 0,1 1,1 1,0 0,0</gml:coordinates>
	</gml:LinearRing>
	*/
	int	GMLReaderImpl::CalWKBLengthFromGMLLinearRing(XNode* pxLinearRing)
	{
		int wkblen = 0;

		if(pxLinearRing==NULL)
		{
			return 0;
		}
	
		XNode* pxCoordsNode = pxLinearRing->GetFirstChild();
		if(pxCoordsNode==NULL)
		{
			return 0;
		}

		const char* coords = pxCoordsNode->GetContent();
		if(coords==NULL)
		{
			return 0;
		}

		wkblen += sizeof(int);
		wkblen += CalWKBLengthFromGMLCoordinates(coords);

		return wkblen;
	}

	int	GMLReaderImpl::CalWKBLengthFromGMLCoordinates(const char* coordinates)
	{
		if(coordinates==NULL)
			return 0;
	
		int wkblen = 0;
		if(strlen(coordinates)==0)
			return 0;
	
		int ptcount = 1;
		for(char* pt = (char*)coordinates; *pt!='\0'; pt++)
		{
			if(*pt == ' ')
				++ptcount;
		}
		wkblen = sizeof(Point) * ptcount;
		return wkblen;
	}

	int	GMLReaderImpl::CalWKBLengthFromGMLPointCount(const char* coordinates)
	{
		if(coordinates==NULL)
			return 0;
	
		if(strlen(coordinates)==0)
			return 0;
	
		int ptcount = 1;
		for(char* pt = (char*)coordinates; pt!='\0'; pt++)
		{
			if(*pt = ' ')
				++ptcount;
		}
		return ptcount;
	}


	bool GMLReaderImpl::CreateWKBFromGML(unsigned char* wkb, int wkbsize, const char* szgml)
	{
		if(szgml==NULL)
			return 0;
	
		bool ret = false;
		//xmlDocPtr doc = NULL;
		//XNode* pRootNode = NULL;
	
		//doc = xmlParseMemory(szgml, strlen(szgml));
		//if(doc==NULL)
		//	return 0;

		//pRootNode = xmlDocGetRootElement(doc);
		//ret = CreateWKBFromGML(wkb, wkbsize, pRootNode);
		//xmlFreeDoc(doc);
	
		return ret;
	}

	bool GMLReaderImpl::CreateWKBFromGML(unsigned char* wkb, int wkbsize, XElement* pxGeometry)
	{
		if(pxGeometry==NULL)
			return false;

		bool ret = false;
		const char* type = pxGeometry->GetName();
		if(0==g_stricmp( type, "Point"))
		{
			ret = CreateWKBFromGMLPoint((WKBPoint*)wkb, pxGeometry);
		}
		else if(0==g_stricmp( type, "LineString"))
		{
			ret = CreateWKBFromGMLLineString((WKBLineString*)wkb, pxGeometry);
		}
		else if(0==g_stricmp( type, "Polygon"))
		{
			ret = CreateWKBFromGMLPolygon((WKBPolygon*)wkb, pxGeometry);
		}
		else if(0==g_stricmp( type, "MultiPoint"))
		{
			ret = CreateWKBFromGMLMultiPoint((WKBMultiPoint*)wkb, pxGeometry);
		}
		else if(0==g_stricmp( type, "MultiLineString"))
		{
			ret = CreateWKBFromGMLMultiLineString((WKBMultiLineString*)wkb, pxGeometry);
		}
		else if(0==g_stricmp( type, "MultiPolygon"))
		{
			ret = CreateWKBFromGMLMultiPolygon((WKBMultiPolygon*)wkb, pxGeometry);
		}
	
		return true;
	}

	bool GMLReaderImpl::CreateWKBFromGMLPoint(WKBPoint* pWKBPoint, XElement* pxGeometry)
	{
		if(pxGeometry==NULL)
			return false;

		XNode* pxNode = pxGeometry->GetFirstChild();
		if(pxNode==NULL)
		{
			return false;
		}

		const char* coords = pxNode->GetContent();
		if(coords==NULL)
		{
			return false;
		}

		pWKBPoint->byteOrder = coDefaultByteOrder;
		pWKBPoint->wkbType = wkbPoint;
		sscanf(coords, "%lf,%lf", &(pWKBPoint->point.x),&(pWKBPoint->point.y));

		return true;
	}

	bool GMLReaderImpl::CreateWKBFromGMLLineString(WKBLineString* pWKBLineString, XElement* pxGeometry)
	{
		if(pxGeometry==NULL)
		{
			return false;
		}

		XNode* pxNode = pxGeometry->GetFirstChild();
		if(pxNode==NULL)
		{
			return false;
		}
		
		const char* coords = pxNode->GetContent();
		if(coords==NULL)
		{
			return false;
		}

		pWKBLineString->byteOrder = coDefaultByteOrder;
		pWKBLineString->wkbType = wkbLineString;
		pWKBLineString->numPoints = CreateWKBPointsFromGMLCoordinates(&(pWKBLineString->points[0]), coords);

		return true;
	}

	bool GMLReaderImpl::CreateWKBFromGMLPolygon(WKBPolygon* pWKBPolygon, XElement* pxGeometry)
	{
		if(pxGeometry==NULL)
		{
			return false;
		}
	
		int numRings = 0;
		char* ptr = NULL;
		LinearRing* pLinearRing = NULL;
		pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));

		XNode* pxBoundNode = NULL;
		XNode* pxLinearRingNode = NULL;
		XNodeSet* pxNodeSet = NULL;

		pxNodeSet = pxGeometry->GetChildren();
		if(pxNodeSet==NULL)
		{
			return false;
		}

		pxNodeSet->Reset();
		while((pxBoundNode=pxNodeSet->Next())!=NULL)
		{
			pxLinearRingNode = pxBoundNode->GetFirstChild();
			if(pxLinearRingNode!=NULL)
			{
				CreateWKBFromGMLLinearRing(pLinearRing, pxLinearRingNode);

				ptr = (char*)pLinearRing;
				pLinearRing = (LinearRing*)(ptr + CalWKBLinearRingLen(pLinearRing));
				numRings++;
			}
		}

		pWKBPolygon->byteOrder = coDefaultByteOrder;
		pWKBPolygon->wkbType = wkbPolygon;
		pWKBPolygon->numRings = numRings;

		return true;
	}

	bool GMLReaderImpl::CreateWKBFromGMLMultiPoint(WKBMultiPoint* pWKBMultiPoint, XElement* pxGeometry)
	{
		if(pxGeometry==NULL)
		{
			return false;
		}

		int numPoints = 0;
		XNode* pxPointMemberNode = NULL;
		XNode* pxPointNode = NULL;
		XNodeSet *pxNodeSet = NULL;
		
		pxNodeSet = pxGeometry->GetChildren();
		if(pxNodeSet==NULL)
		{
			return false;
		}

		WKBPoint* pWKBPoint = NULL;
		pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));

		pxNodeSet->Reset();
		while((pxPointMemberNode=pxNodeSet->Next())!=NULL)
		{
			pxPointNode = pxPointMemberNode->GetFirstChild();
			if(pxPointNode!=NULL)
			{
				CreateWKBFromGMLPoint(pWKBPoint, static_cast<XElement*>(pxPointNode));
				pWKBPoint++;
				numPoints++;
			}
		}

		pWKBMultiPoint->byteOrder = coDefaultByteOrder;
		pWKBMultiPoint->wkbType = wkbMultiPoint;
		pWKBMultiPoint->numPoints = numPoints;

		return true;
	}

	bool GMLReaderImpl::CreateWKBFromGMLMultiLineString(WKBMultiLineString* pWKBMultiLineString, XElement* pxGeometry)
	{
		if(pxGeometry==NULL)
		{
			return false;
		}
	
		XNode* pxlineStringMemberNode = NULL;
		XNode* pxLineStringNode = NULL;
		XNodeSet* pxNodeSet = NULL;

		int numLineStrings = 0;
		char* ptr = NULL;
		WKBLineString* pWKBLineString = NULL;
		pWKBLineString = (WKBLineString*)(&(pWKBMultiLineString->lineStrings[0]));
		
		pxNodeSet = pxGeometry->GetChildren();
		if(pxNodeSet==NULL)
		{
			return false;
		}

		while((pxlineStringMemberNode=pxNodeSet->Next())!=NULL)
		{
			pxLineStringNode = pxlineStringMemberNode->GetFirstChild();
			if(pxLineStringNode!=NULL)
			{
				CreateWKBFromGMLLineString(pWKBLineString, static_cast<XElement*>(pxLineStringNode));

				ptr = (char*)pWKBLineString;
				pWKBLineString = (WKBLineString*)(ptr + CalWKBLineStringLen(pWKBLineString));
				numLineStrings++;
			}
		}

		pWKBMultiLineString->byteOrder = coDefaultByteOrder;
		pWKBMultiLineString->wkbType = wkbMultiLineString;
		pWKBMultiLineString->numLineStrings = numLineStrings;

		return true;
	}

	bool GMLReaderImpl::CreateWKBFromGMLMultiPolygon(WKBMultiPolygon* pWKBMultiPolygon, XElement* pxGeometry)
	{
		if(pxGeometry==NULL)
		{
			return false;
		}
	
		XNode* pxPolygonMemberNode = NULL;
		XNode* pxPolygonNode = NULL;
		XNodeSet* pxNodeSet = NULL;

		char* ptr = NULL;
		int numPolygons = 0;
		WKBPolygon* pWKBPolygon = NULL;
		pWKBPolygon = (WKBPolygon*)(&(pWKBMultiPolygon->polygons[0]));

		pxNodeSet = pxGeometry->GetChildren();
		if(pxNodeSet==NULL)
		{
			return false;
		}

		pxNodeSet->Reset();
		while((pxPolygonMemberNode=pxNodeSet->Next())!=NULL)
		{
			pxPolygonNode = pxPolygonMemberNode->GetFirstChild();
			if(pxPolygonNode!=NULL)
			{
				CreateWKBFromGMLPolygon(pWKBPolygon, static_cast<XElement*>(pxPolygonNode));

				ptr = (char*)pWKBPolygon;
				pWKBPolygon = (WKBPolygon*)(ptr + CalWKBPolygonLen(pWKBPolygon));
				numPolygons++;
			}
		}

		pWKBMultiPolygon->byteOrder = coDefaultByteOrder;
		pWKBMultiPolygon->wkbType = wkbMultiPolygon;
		pWKBMultiPolygon->numPolygons = numPolygons;
	
		return true;
	}

	bool GMLReaderImpl::CreateWKBFromGMLLinearRing(LinearRing* pLinearRing, XNode* pxLinearRing)
	{
		int numPoints = 0;
		const char* coords = pxLinearRing->GetContent();
		Point* pts = (Point*)(&(pLinearRing->points[0]));
		numPoints = CreateWKBPointsFromGMLCoordinates(pts, coords);
		pLinearRing->numPoints = numPoints;

		return true;
	}

	int GMLReaderImpl::CreateWKBPointsFromGMLCoordinates(Point* pts, const char* coordinates)
	{
		int ptcount = 0;
		char* pt = NULL;
		char* temp = strdup(coordinates);
		pt = strtok(temp, " ");
		while(pt!=NULL)
		{
			char* sx = pt;
			char* sy = pt;
			for(; *sy!='\0'; sy++)
			{
				if(*sy==',')
				{	
					*sy = '\0';
					sy++;
					break;
				}
			}

			pts->x = atof(sx);
			pts->y = atof(sy);

			pt = strtok(NULL, " ");
			pts++;
			ptcount++;
		}
		free(temp);
		return ptcount;
	}

	void GMLReaderImpl::CreateWKBPointFromGMLCoordinates(Point* pt, char* coordinates)
	{
		char* ptr = NULL;
		ptr = strtok(coordinates, ",");
		while(ptr!=NULL)
		{
			char* sx = ptr;
			char* sy = ptr;
			for(; *sy!='\0'; sy++)
			{
				if(*sy==',')
				{	
					*sy = '\0';
					sy++;
					break;
				}
			}

			pt->x = atof(sx);
			pt->y = atof(sy);
			ptr = strtok(NULL, " ");
		}
	}


	int	GMLReaderImpl::FindOutBoundIndex(WKBPolygon* pWKBPolygon)
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


	void GMLReaderImpl::GetBound(LinearRing* pLinearRing, int dimension, double& dmin, double& dmax)
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

	//--------------------------------------------------------------------------//
	//                      WKB LEGNTH FROM GML  END                            //
	//--------------------------------------------------------------------------//
	//////////////////////////////////////////////////////////////////////////
	int	GMLReaderImpl::CalWKBLen(unsigned char* wkb)
	{
		if(wkb==NULL)
			return 0;
		int numBytes = 0;
		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			numBytes = CalWKBPointLen((WKBPoint*)wkb);
			break;
		case wkbLineString:
			numBytes = CalWKBLineStringLen((WKBLineString*)wkb);
			break;
		case wkbPolygon:
			numBytes = CalWKBPolygonLen((WKBPolygon*)wkb);
			break;
		case wkbMultiPoint:
			numBytes = CalWKBMultiPointLen((WKBMultiPoint*)wkb);
			break;
		case wkbMultiLineString:
			numBytes = CalWKBMultiLineStringLen((WKBMultiLineString*)wkb);
			break;
		case wkbMultiPolygon:
			numBytes = CalWKBMultiPolygonLen((WKBMultiPolygon*)wkb);
			break;
		}
		return numBytes;
	}

	int GMLReaderImpl::CalWKBMultiPointLen(WKBMultiPoint* pWKBMultiPoint)
	{
		int len = 0;
		len += sizeof(unsigned char);
		len += sizeof(g_int) << 1;
		len += sizeof(Point) * pWKBMultiPoint->numPoints;
		return len;
	}

	int GMLReaderImpl::CalWKBMultiLineStringLen(WKBMultiLineString* pWKBMultiLineString)
	{
		int len = sizeof(unsigned char);
		len += sizeof(g_int) << 1;

		int lenLintString = 0;
		int numLineStrings = pWKBMultiLineString->numLineStrings;
		unsigned char* pWKBLineString = (unsigned char*)&(pWKBMultiLineString->lineStrings[0]);
		for(int i=0; i<numLineStrings; i++)
		{
			lenLintString = CalWKBLineStringLen((WKBLineString*)pWKBLineString);
			len+=lenLintString;
			pWKBLineString += lenLintString;
		}

		return len;
	}

	int GMLReaderImpl::CalWKBMultiPolygonLen(WKBMultiPolygon* WKBMultiPolygon)
	{
		int len = sizeof(unsigned char);
		len += sizeof(g_int) << 1;

		int lenPolygon = 0;
		int numPolygons = WKBMultiPolygon->numPolygons;
		unsigned char* pWKBPolygon = (unsigned char*)&(WKBMultiPolygon->polygons[0]);
		for(int i=0; i<numPolygons; i++)
		{
			lenPolygon = CalWKBPolygonLen((WKBPolygon*)pWKBPolygon);
			len+=lenPolygon;
			pWKBPolygon += lenPolygon;
		}
		return len;
	}

	int GMLReaderImpl::CalWKBLinearRingLen(LinearRing* pLinearRing)
	{
		return (sizeof(g_int)+sizeof(Point)*pLinearRing->numPoints);
	}

}
