#ifndef __AUGE_WKT_WRITER_H__
#define __AUGE_WKT_WRITER_H__

#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class WKTWriter
	{
	public:
		WKTWriter(){}
		virtual ~WKTWriter(){}
	public:
		bool	Write(std::string& wkt, g_uchar* wkb, bool multi=false);
	protected:
		bool	Write(WKBPoint* pwkbPoint, std::string& wkt);
		bool	Write(WKBLineString* pwkbLineString, std::string& wkt);
		bool	Write(WKBPolygon* pwkbPolygon, std::string& wkt);
		bool	Write(WKBMultiPoint* pwkbMultiPoint, std::string& wkt);
		bool	Write(WKBMultiLineString* pwkbMultiLineString, std::string& wkt);
		bool	Write(WKBMultiPolygon* pwkbMultiPolgon, std::string& wkt);
		bool	WriteMultiPoint(WKBPoint* pwkbPoint, std::string& wkt);
		bool	WriteMultiLineString(WKBLineString* pwkbLineString, std::string& wkt);
		bool	WriteMultiPolygon(WKBPolygon* pwkbPolygon, std::string& wkt);

		bool	write_coordinates(auge::Point* points, int numPoints, std::string& wkt);
		bool	write_line_string_body(WKBLineString *pwkbLineString, std::string& wkt);
		bool	write_polygon_body(WKBPolygon *pwkbPolygon, std::string& wkt);

	private:
		int		CalWKBLen(unsigned char* wkb);
		int		CalWKBPointLen(WKBPoint* pWKBPoint);
		int		CalWKBLineStringLen(WKBLineString* pWKBLineString);
		int		CalWKBPolygonLen(WKBPolygon* pWKBPolygon);
		int		CalWKBLinearRingLen(LinearRing* pLinearRing);
		int		CalWKBMultiPointLen(WKBMultiPoint* pWKBMultiPoint);
		int		CalWKBMultiLineStringLen(WKBMultiLineString* pWKBMultiLineString);
		int		CalWKBMultiPolygonLen(WKBMultiPolygon* WKBMultiPolygon);
	};
}

#endif //__AUGE_WKT_WRITER_H__
