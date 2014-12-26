#include "WKTReader.h"
#include "StringTokenizer.h"
#include "GeometryUtilImpl.h"

#include "string.h"

namespace auge
{

WKTReader::WKTReader()
{

}

WKTReader::~WKTReader()
{

}

g_uchar* WKTReader::Read(const char* wkt)
{
	if(wkt==NULL)
		return NULL;

	g_uchar* wkb = NULL;
	WKBGeometryType type = ParseGeometryType(wkt);
	
	int wkb_size = 0;
	wkb_size =  cal_wkb_size(wkt, type);
	if(wkb_size<=0)
	{
		return NULL;
	}

	wkb = (g_uchar*)malloc(sizeof(g_uchar)*wkb_size);
	memset(wkb, 0, wkb_size);

	bool ret = wkt_2_wkb(wkb, wkt, type);
	if(!ret)
	{
		free(wkb);
		wkb = NULL;
	}

	return wkb;
}

int	WKTReader::cal_wkb_size(const char* wkt)
{
	return 0;
}

int	WKTReader::cal_wkb_size(const char* wkt, WKBGeometryType type)
{
	int wkb_size = 0;
	switch(type)
	{
	case wkbPoint:
		wkb_size = cal_wkb_point_size(wkt);
		break;
	case wkbLineString:
		wkb_size = cal_wkb_line_string_size(wkt);
		break;
	case wkbPolygon:
		wkb_size = cal_wkb_polygon_size(wkt);
		break;
	case wkbMultiPoint:
		wkb_size = cal_wkb_multi_point_size(wkt);
		break;
	case wkbMultiLineString:
		wkb_size = cal_wkb_multi_line_string_size(wkt);
		break;
	case wkbMultiPolygon:
		wkb_size = cal_wkb_multi_polygon_size(wkt);
		break;
	}
	return wkb_size;
}

int	WKTReader::cal_wkb_point_size(const char* text)
{
	return sizeof(WKBPoint);
}

int	WKTReader::cal_wkb_line_string_size(const char* text)
{
	const char* head = strchr(text, '(');
	const char* tail = strrchr(text, ')');
	if(head>tail)
		return 0;

	int comma_count = cal_comma_count((char*)head, tail-head);
	if(comma_count<0)
		return -1;

	int wkb_size = sizeof(WKBLineString) + sizeof(auge::Point) * comma_count;
	return wkb_size;
}

// struct WKBPolygon {
// 	unsigned char	byteOrder;
// 	int				wkbType;// = 3;
// 	int				numRings;
// 	LinearRing		rings[1];
// };

int	WKTReader::cal_wkb_polygon_size(const char* text)
{
	std::string data = text;
	StringTokenizer::Remove(data,"(",")");

	StringTokenizer st(data.c_str(),',');
	int iCount = st.GetCount();
	if(iCount==0)
	{
		return 0;
	}

	int wkb_size = 0;
	int wkb_linear_ring_size = 0;
	const char* wkb_linear_ring = NULL;
	//for each LineRing, caculate the wkb size;
	for(int i=0;i<iCount;i++)
	{
		wkb_linear_ring = st.GetItem(i);
		wkb_linear_ring_size = cal_wkb_linear_ring_size(wkb_linear_ring);
		wkb_size += wkb_linear_ring_size;
	}

	wkb_size += sizeof(unsigned char);	//byteOrder
	wkb_size += sizeof(int) << 1;		//wkbType + numRings;

	return wkb_size;
}

int	WKTReader::cal_wkb_multi_point_size(const char* text)
{
	const char* head = strchr(text, '(');
	const char* tail = strrchr(text, ')');
	if(head>tail)
		return 0;

	int comma_count = cal_comma_count(head, tail-head);
	if(comma_count<0)
		return -1;

	int wkb_size = sizeof(WKBMultiPoint) + sizeof(WKBPoint) * comma_count;
	return wkb_size;
}

// struct WKBMultiLineString 
// {
// 	unsigned char	byteOrder;
// 	int				wkbType;// = 5;
// 	int				numLineStrings;
// 	WKBLineString	lineStrings[1];
// };
int	WKTReader::cal_wkb_multi_line_string_size(const char* text)
{
	std::string data = text;
	StringTokenizer::Remove(data,"(",")");
	
	StringTokenizer st(data.c_str(),',');
	int iCount = st.GetCount();
	if(iCount==0)
	{
		return 0;
	}

	int wkb_size = 0;
	int wkb_line_string_size;
	const char* wkb_line_string = NULL;
	
	for(int i=0; i<iCount; i++)
	{
		wkb_line_string = st.GetItem(i);
		wkb_line_string_size = cal_wkb_line_string_size(wkb_line_string);
		wkb_size += wkb_line_string_size;
	}

	wkb_size += sizeof(unsigned char);	//byteOrder
	wkb_size += sizeof(int) << 1;		//wkbType + numLineStrings;
	
	return wkb_size;
}

// struct WKBMultiPolygon 
// {
// 	unsigned char	byteOrder;
// 	int				wkbType;// = 6;
// 	int				numPolygons;
// 	WKBPolygon		polygons[1];
// };
int	WKTReader::cal_wkb_multi_polygon_size(const char* text)
{
	std::string data = text;
	StringTokenizer::Remove(data,"(",")");

	StringTokenizer st(data.c_str(),',');
	int iCount = st.GetCount();
	if(iCount==0)
	{
		return 0;
	}

	int wkb_size = 0;
	int wkb_polygon_size;
	const char* wkb_polygon = NULL;

	for(int i=0; i<iCount; i++)
	{
		wkb_polygon = st.GetItem(i);
		wkb_polygon_size = cal_wkb_polygon_size(wkb_polygon);
		wkb_size += wkb_polygon_size;
	}

	wkb_size += sizeof(unsigned char);	//byteOrder
	wkb_size += sizeof(int) << 1;		//wkbType + numPolygons;

	return wkb_size;
}

int	WKTReader::cal_wkb_linear_ring_size(const char* text)
{
	const char* head = strchr(text, '(');
	const char* tail = strrchr(text, ')');
	if(head>tail)
		return 0;
	
	int comma_count = cal_comma_count(head, tail-head);
	if(comma_count<0)
		return -1;
	
	int wkb_size = sizeof(LinearRing) + sizeof(auge::Point) * comma_count;
	return wkb_size;
}

/**
 * if text is empty, then return -1
 */
inline
int	WKTReader::cal_comma_count(const char* text, int length)
{
	const char *head = text;
	const char *tail = text+length;
	
	char chr;
	int  comma_count = 0;
	bool isempty = true;
	char* ptr = (char*)(head+1);
	for( ; ptr<tail; ptr++)
	{
		chr = *ptr;
		if(chr!=' ')
			isempty = false;
		if(chr==',')
			comma_count++;
	}
	
	if(isempty)
		return -1;
	return comma_count;
}

WKBGeometryType WKTReader::ParseGeometryType(const char* ptr)
{
	WKBGeometryType type = wkbNull;
	if(g_strnicmp(ptr,"POINT",5)==0)
		type = wkbPoint;
	else if(g_strnicmp(ptr,"LINESTRING",10)==0)
		type = wkbLineString;
	else if(g_strnicmp(ptr,"POLYGON",7)==0)
		type = wkbPolygon;
	else if(g_strnicmp(ptr,"MULTIPOINT",10)==0)
		type = wkbMultiPoint;
	else if(g_strnicmp(ptr,"MULTILINESTRING",15)==0)
		type = wkbMultiLineString;
	else if(g_strnicmp(ptr,"MULTIPOLYGON",12)==0)
		type = wkbMultiPolygon;
	return type;
}

bool WKTReader::wkt_2_wkb(g_uchar* wkb, const char* wkt, WKBGeometryType type)
{
	bool ret = false;
	switch(type)
	{
	case wkbPoint:
		ret = wkt_2_wkb_point((WKBPoint*)wkb, wkt);
		break;
	case wkbLineString:
		ret = wkt_2_wkb_line_string((WKBLineString*)wkb, wkt);
		break;
	case wkbPolygon:
		ret = wkt_2_wkb_polygon((WKBPolygon*)wkb, wkt);
		break;
	case wkbMultiPoint:
		ret = wkt_2_wkb_multi_point((WKBMultiPoint*)wkb, wkt);
		break;
	case wkbMultiLineString:
		ret = wkt_2_wkb_multi_line_string((WKBMultiLineString*)wkb, wkt);
		break;
	case wkbMultiPolygon:
		ret = wkt_2_wkb_multi_polygon((WKBMultiPolygon*)wkb, wkt);
		break;
	}
	return ret;
}

bool WKTReader::wkt_2_wkb_point(WKBPoint* pwkbPoint, const char* wkt)
{
	pwkbPoint->byteOrder = coDefaultByteOrder;
	pwkbPoint->wkbType = wkbPoint;

	std::string text = wkt;
	int head = text.find("(");
	int tail = text.find_last_of(")");
	if(head>=tail)
		return false;
	double x = 0.0f;
	double y = 0.0f;

	std::string coordinate = text.substr(head+1, tail-head-1);
	if(!parse_coordinate(coordinate, pwkbPoint->point.x, pwkbPoint->point.y))
	{
		return false;
	}

	return true;
}

bool WKTReader::wkt_2_wkb_line_string(WKBLineString* pwkbLineString, const char* wkt)
{
	pwkbLineString->byteOrder = coDefaultByteOrder;
	pwkbLineString->wkbType = wkbLineString;
	auge::Point *points = &(pwkbLineString->points[0]);
	
	std::string text = wkt;
	int head = text.find("(");
	int tail = text.find_last_of(")");
	if(head>=tail)
		return false;
	double x = 0.0f;
	double y = 0.0f;
	
	std::string coordinates = text.substr(head+1, tail-head-1);
	if(!parse_coordinates(coordinates, points, pwkbLineString->numPoints))
	{
		return false;
	}

	return true;
}

/*
"POLYGON((6.027164 49.524078,6.088064 49.680508,5.948726 49.772232,5.941587 49.91943))";
*/
bool WKTReader::wkt_2_wkb_polygon(WKBPolygon* pwkbPolygon, const char* wkt)
{
	std::string text = wkt;
	StringTokenizer::Remove(text,"(",")");
	
	StringTokenizer st(text.c_str(),',');
	int iCount = st.GetCount();
	if(iCount==0)
	{
		return 0;
	}

	pwkbPolygon->byteOrder = coDefaultByteOrder;
	pwkbPolygon->wkbType = wkbPolygon;
	pwkbPolygon->numRings = st.GetCount();

	LinearRing *pLinearRing = &(pwkbPolygon->rings[0]);
	const char *linear_ring;

	for(int i=0; i<iCount; i++)
	{
		linear_ring = st.GetItem(i);
		if(pLinearRing==NULL)
		{
			return false;
		}

		if(!wkt_2_wkb_linear_ring(pLinearRing, linear_ring))
		{
			return false;
		}
		GeometryUtilImpl util;
		
		int offset = util.GetWKBSize(pLinearRing);
		pLinearRing = (LinearRing*)(((g_uchar*)pLinearRing) + offset);
	}

	return true;
}

bool WKTReader::wkt_2_wkb_multi_point(WKBMultiPoint* pwkbMultiPoint, const char* wkt)
{
	pwkbMultiPoint->byteOrder = coDefaultByteOrder;
	pwkbMultiPoint->wkbType = wkbMultiPoint;
	WKBPoint *points = &(pwkbMultiPoint->points[0]);
	
	std::string text = wkt;
	int head = text.find("(");
	int tail = text.find_last_of(")");
	if(head>=tail)
		return false;
	double x = 0.0f;
	double y = 0.0f;
	
	std::string coordinates = text.substr(head+1, tail-head-1);
	if(!parse_coordinates(coordinates, points, pwkbMultiPoint->numPoints))
	{
		return false;
	}
	return true;
}

bool WKTReader::wkt_2_wkb_multi_line_string(WKBMultiLineString* pwkbMultiLineString, const char* wkt)
{
	std::string text = wkt;
	StringTokenizer::Remove(text,"(",")");
	
	StringTokenizer st(text.c_str(),',');
	int iCount = st.GetCount();
	if(iCount==0)
	{
		return 0;
	}

	pwkbMultiLineString->byteOrder = coDefaultByteOrder;
	pwkbMultiLineString->wkbType = wkbMultiLineString;
	pwkbMultiLineString->numLineStrings = st.GetCount();

	WKBLineString *pWKBLineString = &(pwkbMultiLineString->lineStrings[0]);	
	const char *wkt_line_string = NULL;
	
	for(int i=0; i<iCount; i++)
	{
		wkt_line_string = st.GetItem(i);
		if(wkt_line_string==NULL)
		{
			return false;
		}
		if(!wkt_2_wkb_line_string(pWKBLineString, wkt_line_string))
		{
			return false;
		}
		GeometryUtilImpl util;
		int offset = util.GetWKBSize(pWKBLineString);
		pWKBLineString = (WKBLineString*)(((g_uchar*)pWKBLineString) + offset);
	}

	return true;
}

bool WKTReader::wkt_2_wkb_multi_polygon(WKBMultiPolygon* pwkbMultiPolygon, const char* wkt)
{
	std::string text = wkt;
	StringTokenizer::Remove(text,"(",")");
	
	StringTokenizer st(text.c_str(),',');
	int iCount = st.GetCount();
	if(iCount==0)
	{
		return 0;
	}
	
	pwkbMultiPolygon->byteOrder = coDefaultByteOrder;
	pwkbMultiPolygon->wkbType = wkbMultiPolygon;
	pwkbMultiPolygon->numPolygons = st.GetCount();
	
	WKBPolygon *pWKBPolygon = &(pwkbMultiPolygon->polygons[0]);	
	const char *wkt_polygon = NULL;
	
	for(int i=0; i<iCount; i++)
	{
		wkt_polygon = st.GetItem(i);
		if(wkt_polygon==NULL)
		{
			return false;
		}
		if(!wkt_2_wkb_polygon(pWKBPolygon, wkt_polygon))
		{
			return false;
		}
		GeometryUtilImpl util;
		int offset = util.GetWKBSize(pWKBPolygon);
		pWKBPolygon = (WKBPolygon*)(((g_uchar*)pWKBPolygon) + offset);
	}

	return true;
}

bool WKTReader::wkt_2_wkb_linear_ring(LinearRing* pwkbLinearRing, const char* wkt)
{
	auge::Point *points = &(pwkbLinearRing->points[0]);
	
	std::string text = wkt;
	int head = text.find("(");
	int tail = text.find_last_of(")");
	if(head>=tail)
		return false;
	double x = 0.0f;
	double y = 0.0f;
	
	std::string coordinates = text.substr(head+1, tail-head-1);
	if(!parse_coordinates(coordinates, points, pwkbLinearRing->numPoints))
	{
		return false;
	}
	
	return true;
}

bool WKTReader::parse_coordinates(std::string& coordinate, auge::Point* pt, int& numPoints)
{
	char* coord_dup = strdup(coordinate.c_str());
	if(coord_dup==NULL)
		return false;

	bool ret = parse_coordinates(coord_dup, pt, numPoints);

	free(coord_dup);
	return ret;

	return true;
}

bool WKTReader::parse_coordinates(char* coordinates, auge::Point* pt, int& numPoints)
{
	char* head = coordinates;
	char* tail = head;
	for(; *tail!='\0'; tail++)
	{
		if(*tail==',')
		{
			*tail='\0';
			if(!parse_coordinate(head, pt->x, pt->y))
			{
				return false;
			}
			++numPoints;
			head = ++tail;
			pt++;
		}
	}

	// last coordinates
	if(head<tail)
	{
		if(!parse_coordinate(head, pt->x, pt->y))
		{
			return false;
		}
		++numPoints;
	}

	return true;
}

bool WKTReader::parse_coordinates(std::string& coordinate, WKBPoint* pt, int& numPoints)
{
	char* coord_dup = strdup(coordinate.c_str());
	if(coord_dup==NULL)
		return false;
	
	bool ret = parse_coordinates(coord_dup, pt, numPoints);
	
	free(coord_dup);
	return ret;
}

bool WKTReader::parse_coordinates(char* coordinates, WKBPoint* pt, int& numPoints)
{
	char* head = coordinates;
	char* tail = head;
	for(; *tail!='\0'; tail++)
	{
		if(*tail==',')
		{
			*tail='\0';
			if(!parse_coordinate(head, pt->point.x, pt->point.y))
			{
				return false;
			}
			
			head = ++tail;

			pt->byteOrder = coDefaultByteOrder;
			pt->wkbType = wkbPoint;
			++pt;
			++numPoints;
		}
	}
	
	// last coordinates
	if(head<tail)
	{
		if(!parse_coordinate(head, pt->point.x, pt->point.y))
		{
			return false;
		}
		pt->byteOrder = coDefaultByteOrder;
		pt->wkbType = wkbPoint;
		++numPoints;
	}
	
	return true;
}

bool WKTReader::parse_coordinate(std::string& coordinate, double& x, double &y)
{
	int npos = coordinate.find(" ");
	if(npos<0)
		return false;
	
	std::string sx = coordinate.substr(0,npos);
	std::string sy = coordinate.substr(npos+1, coordinate.length()-npos-1);
	x = atof(sx.c_str());
	y = atof(sy.c_str());

	return true;
}

bool WKTReader::parse_coordinate(char* coordinate, double& x, double &y)
{
	char* ptr = strstr(coordinate, " ");
	if(ptr==NULL)
		return false;	
	*ptr = '\0';

	char* sx = coordinate;
	char* sy = ptr+1;
	x = atof(sx);
	y = atof(sy);

	return true;
}

}
