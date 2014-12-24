#ifndef __AUGE_WKT_READER_H__
#define __AUGE_WKT_READER_H__

#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class WKTReader
	{
	public:
		WKTReader();
		virtual ~WKTReader();
	public:
		g_uchar*  Read(const char* wkt);
	private:
		WKBGeometryType ParseGeometryType(const char* ptr);
	private:
		int		cal_wkb_size(const char* wkt);
		int		cal_wkb_size(const char* wkt, WKBGeometryType type);
		int		cal_wkb_point_size(const char* text);
		int		cal_wkb_line_string_size(const char* text);
		int		cal_wkb_polygon_size(const char* text);
		int		cal_wkb_multi_point_size(const char* text);
		int		cal_wkb_multi_line_string_size(const char* text);
		int		cal_wkb_multi_polygon_size(const char* text);
		int		cal_wkb_linear_ring_size(const char* text);

		int		cal_comma_count(const char* text, int length);

		bool	wkt_2_wkb(g_uchar* wkb, const char* wkt);
		bool	wkt_2_wkb(g_uchar* wkb, const char* wkt, WKBGeometryType type);
		bool	wkt_2_wkb_point(WKBPoint* pwkbPoint, const char* wkt);
		bool	wkt_2_wkb_line_string(WKBLineString* pwkbLineString, const char* wkt);
		bool	wkt_2_wkb_polygon(WKBPolygon* pwkbPolygon, const char* wkt);
		bool	wkt_2_wkb_multi_point(WKBMultiPoint* wkb, const char* wkt);
		bool	wkt_2_wkb_multi_line_string(WKBMultiLineString* wkb, const char* wkt);
		bool	wkt_2_wkb_multi_polygon(WKBMultiPolygon* wkb, const char* wkt);
		bool	wkt_2_wkb_linear_ring(LinearRing* pwkbLinearRing, const char* wkt);

		bool	parse_coordinate(char* coordinate, double& x, double &y);
		bool	parse_coordinate(std::string& coordinate, double& x, double &y);
		bool	parse_coordinates(char* coordinates, auge::Point* pt, int& numPoints);
		bool	parse_coordinates(std::string& coordinate, auge::Point* pt, int& numPoints);
		bool	parse_coordinates(std::string& coordinate, WKBPoint* pt, int& numPoints);
		bool	parse_coordinates(char* coordinates, WKBPoint* pt, int& numPoints);

	};

}

#endif //__AUGE_WKT_READER_H__

