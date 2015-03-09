#include "WKTWriter.h"

namespace auge
{

	bool WKTWriter::Write(std::string& wkt, g_uchar* wkb,bool multi/*=false*/)
	{
		if(wkb==NULL)
			return false;

		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			{
				if(multi)
					return WriteMultiPoint((WKBPoint*)wkb, wkt);
				else
					return Write((WKBPoint*)wkb, wkt);
			}
			break;
		case wkbLineString:
			{
				if(multi)
					return WriteMultiLineString((WKBLineString*)wkb, wkt);
				else
					return Write((WKBLineString*)wkb, wkt);
			}
			break;
		case wkbPolygon:
			{
				if(multi)
					return WriteMultiPolygon((WKBPolygon*)wkb, wkt);
				else
					return Write((WKBPolygon*)wkb, wkt);
			}
			break;
		case wkbMultiPoint:
			{
				return Write((WKBMultiPoint*)wkb, wkt);
			}
			break;
		case wkbMultiLineString:
			{
				return Write((WKBMultiLineString*)wkb, wkt);
			}
			break;
		case wkbMultiPolygon:
			{
				return Write((WKBMultiPolygon*)wkb, wkt);
			}
			break;
		}

		return false;
	}

	bool WKTWriter::Write(WKBPoint* pwkbPoint, std::string& wkt)
	{
		char coordinate[AUGE_BUFFER_MAX];
		sprintf(coordinate, "%f %f", pwkbPoint->point.x, pwkbPoint->point.y);

		wkt  = "POINT(";
		wkt += coordinate;
		wkt += ")";
		return true;
	}

	bool WKTWriter::WriteMultiPoint(WKBPoint* pwkbPoint, std::string& wkt)
	{
		char coordinate[AUGE_BUFFER_MAX];
		sprintf(coordinate, "%f %f", pwkbPoint->point.x, pwkbPoint->point.y);

		wkt  = "MULTIPOINT((";
		wkt += coordinate;
		wkt += "))";
		return true;
	}

	bool WKTWriter::Write(WKBLineString* pwkbLineString, std::string& wkt)
	{
		int i=0, numPoints = 0;
		auge::Point* ptr = &(pwkbLineString->points[0]);
		numPoints = pwkbLineString->numPoints;

		char coordinate[AUGE_BUFFER_MAX];
		bool started = false;

		wkt = "LINESTRING(";
		for(i=0; i<numPoints; ++i, ++ptr)
		{
			if(started)
				wkt += ",";
			else
				started=true;

			sprintf(coordinate, "%f %f", ptr->x, ptr->y);
			wkt += coordinate;
		}
		wkt +=")";

		return true;
	}

	bool WKTWriter::WriteMultiLineString(WKBLineString* pwkbLineString, std::string& wkt)
	{
		int i=0, numPoints = 0;
		auge::Point* ptr = &(pwkbLineString->points[0]);
		numPoints = pwkbLineString->numPoints;

		char coordinate[AUGE_BUFFER_MAX];
		bool started = false;

		wkt = "MULTILINESTRING((";
		for(i=0; i<numPoints; ++i, ++ptr)
		{
			if(started)
				wkt += ",";
			else
				started=true;

			sprintf(coordinate, "%f %f", ptr->x, ptr->y);
			wkt += coordinate;
		}
		wkt +="))";

		return true;
	}

	bool WKTWriter::Write(WKBPolygon* pwkbPolygon, std::string& wkt)
	{
		int i=0, j=0;
		int numPoints = 0;
		int numLinearRing = 0;
		int offset = 0;
		auge::Point *ptr = NULL;
		LinearRing *pLineRing = NULL;

		bool comma_started = false;

		numLinearRing = pwkbPolygon->numRings;
		pLineRing = (LinearRing*)(&(pwkbPolygon->rings[0]));

		wkt = "POLYGON(";
		for(i=0; i<numLinearRing; i++)
		{
			if(comma_started)
				wkt += ",";
			else
				comma_started = true;

			numPoints = pLineRing->numPoints;
			ptr = (auge::Point*)(&(pLineRing->points[0]));

			if(numPoints<=0)
			{
				return false;
			}

			write_coordinates(ptr, numPoints, wkt);

			offset = numPoints << 4;	//numPoints*sizeof(Point);
			pLineRing = (LinearRing*)(((char*)ptr)+offset);
		}
		wkt += ")";

		return true;
	}

	bool WKTWriter::WriteMultiPolygon(WKBPolygon* pwkbPolygon, std::string& wkt)
	{
		int i=0, j=0;
		int numPoints = 0;
		int numLinearRing = 0;
		int offset = 0;
		auge::Point *ptr = NULL;
		LinearRing *pLineRing = NULL;

		bool comma_started = false;

		numLinearRing = pwkbPolygon->numRings;
		pLineRing = (LinearRing*)(&(pwkbPolygon->rings[0]));

		wkt = "MULTIPOLYGON((";
		for(i=0; i<numLinearRing; i++)
		{
			if(comma_started)
				wkt += ",";
			else
				comma_started = true;

			numPoints = pLineRing->numPoints;
			ptr = (auge::Point*)(&(pLineRing->points[0]));

			if(numPoints<=0)
			{
				return false;
			}

			write_coordinates(ptr, numPoints, wkt);

			offset = numPoints << 4;	//numPoints*sizeof(Point);
			pLineRing = (LinearRing*)(((char*)ptr)+offset);
		}
		wkt += "))";

		return true;

	}

	bool WKTWriter::Write(WKBMultiPoint* pwkbMultiPoint, std::string& wkt)
	{
		int i=0, numPoints = 0;
		WKBPoint* ptr = &(pwkbMultiPoint->points[0]);
		numPoints = pwkbMultiPoint->numPoints;

		char coordinate[AUGE_BUFFER_MAX];
		bool started = false;

		wkt = "MULTIPOINT(";
		for(i=0; i<numPoints; ++i, ++ptr)
		{
			if(started)
				wkt += ",";
			else
				started=true;

			sprintf(coordinate, "%f %f", ptr->point.x, ptr->point.y);
			wkt += coordinate;
		}
		wkt +=")";

		return true;
	}

	bool WKTWriter::Write(WKBMultiLineString* pwkbMultiLineString, std::string& wkt)
	{
		int i=0, j=0;
		int numPoints = 0;
		int numLineString = 0;
		int offset = 0;

		auge::Point   *pt = NULL;
		WKBLineString *pwkbLineString = NULL;

		numLineString = pwkbMultiLineString->numLineStrings;
		pwkbLineString = (WKBLineString*)(&(pwkbMultiLineString->lineStrings[0]));

		bool comma_started = false;

		wkt = "MULTILINESTRING(";
		for(i=0; i<numLineString; i++)
		{
			if(comma_started)
				wkt += ",";
			else
				comma_started = true;

			write_line_string_body(pwkbLineString,wkt);

			offset = CalWKBLineStringLen(pwkbLineString);
			pwkbLineString = (WKBLineString*)(((char*)pwkbLineString)+offset);
		}
		wkt += ")";
		return true;
	}

	bool WKTWriter::Write(WKBMultiPolygon* pwkbMultiPolgon, std::string& wkt)
	{
		int i=0, j=0;	
		int offset = 0;

		int numPolygons = 0;
		WKBPolygon *pwkbPolygon = NULL;

		numPolygons = pwkbMultiPolgon->numPolygons;
		pwkbPolygon = (WKBPolygon*)(&(pwkbMultiPolgon->polygons[0]));

		bool comma_started = false;

		wkt = "MULTIPOLYGON(";
		for(i=0; i<numPolygons; i++)
		{
			if(comma_started)
				wkt += ",";
			else
				comma_started = true;

			write_polygon_body(pwkbPolygon,wkt);

			offset = CalWKBPolygonLen(pwkbPolygon);
			pwkbPolygon = (WKBPolygon*)(((char*)pwkbPolygon)+offset);
		}
		wkt += ")";
		return true;
	}

	bool WKTWriter::write_coordinates(auge::Point* points, int numPoints, std::string& wkt)
	{
		if(numPoints<=0)
			return false;

		auge::Point *pt = points;
		char coords[AUGE_BUFFER_MAX];
		bool comma_started = false;

		wkt += "(";
		for(int i=0; i<numPoints; i++, pt++)
		{	
			sprintf(coords,"%f %f", pt->x, pt->y);
			if(comma_started)
				wkt += ",";
			else
				comma_started = true;
			wkt += coords;
		}
		wkt += ")";

		return true;
	}

	bool WKTWriter::write_line_string_body(WKBLineString *pwkbLineString, std::string& wkt)
	{
		int i=0, numPoints = 0;
		auge::Point* ptr = &(pwkbLineString->points[0]);
		numPoints = pwkbLineString->numPoints;

		char coordinate[AUGE_BUFFER_MAX];
		bool started = false;

		wkt += "(";
		for(i=0; i<numPoints; ++i, ++ptr)
		{
			if(started)
				wkt += ",";
			else
				started=true;

			sprintf(coordinate, "%f %f", ptr->x, ptr->y);
			wkt += coordinate;
		}
		wkt +=")";

		return true;
	}

	bool WKTWriter::write_polygon_body(WKBPolygon *pwkbPolygon, std::string& wkt)
	{
		int i=0, j=0;
		int numPoints = 0;
		int numLinearRing = 0;
		int offset = 0;
		auge::Point *ptr = NULL;
		LinearRing *pLineRing = NULL;

		bool comma_started = false;

		numLinearRing = pwkbPolygon->numRings;
		pLineRing = (LinearRing*)(&(pwkbPolygon->rings[0]));

		wkt += "(";
		for(i=0; i<numLinearRing; i++)
		{
			if(comma_started)
				wkt += ",";
			else
				comma_started = true;

			numPoints = pLineRing->numPoints;
			ptr = (auge::Point*)(&(pLineRing->points[0]));

			if(numPoints<=0)
			{
				return false;
			}

			write_coordinates(ptr, numPoints, wkt);

			offset = numPoints << 4;	//numPoints*sizeof(Point);
			pLineRing = (LinearRing*)(((char*)ptr)+offset);
		}
		wkt += ")";

		return true;
	}

	//--------------------------------------------------------------------------//
	//                      WKB LEGNTH FROM GML  END                            //
	//--------------------------------------------------------------------------//
	//////////////////////////////////////////////////////////////////////////
	int	WKTWriter::CalWKBLen(unsigned char* wkb)
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


	int WKTWriter::CalWKBPointLen(WKBPoint* pWKBPoint)
	{
		return (sizeof(unsigned char) + sizeof(g_int32) + sizeof(Point));
	}

	int WKTWriter::CalWKBLineStringLen(WKBLineString* pWKBLineString)
	{
		int len = 0;
		len += sizeof(unsigned char);
		len += sizeof(g_int32) << 1;
		len += sizeof(Point) * pWKBLineString->numPoints;
		return len;
	}

	int WKTWriter::CalWKBPolygonLen(WKBPolygon* pWKBPolygon)
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

	int WKTWriter::CalWKBMultiPointLen(WKBMultiPoint* pWKBMultiPoint)
	{
		int len = 0;
		len += sizeof(unsigned char);
		len += sizeof(g_int32) << 1;
		len += sizeof(WKBPoint) * pWKBMultiPoint->numPoints;
		return len;
	}

	int WKTWriter::CalWKBMultiLineStringLen(WKBMultiLineString* pWKBMultiLineString)
	{
		int len = sizeof(unsigned char);
		len += sizeof(g_int32) << 1;

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

	int WKTWriter::CalWKBMultiPolygonLen(WKBMultiPolygon* WKBMultiPolygon)
	{
		int len = sizeof(unsigned char);
		len += sizeof(g_int32) << 1;

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

	int WKTWriter::CalWKBLinearRingLen(LinearRing* pLinearRing)
	{
		return (sizeof(g_int32)+sizeof(Point)*pLinearRing->numPoints);
	}
}