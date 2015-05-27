#ifndef __AUGE_GML_READER_IMPL_H__
#define __AUGE_GML_READER_IMPL_H__

#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class GMLReaderImpl : public GMLReader
	{
	public:
		GMLReaderImpl();
		virtual ~GMLReaderImpl();
	public:
		virtual	Geometry*	Read(const char* gml);
		virtual	Geometry*	Read(XElement *pxGeometry);
		virtual void		Release();

	private:
		//char*				AsGML(WKBGeometry* pWKBGeometry);
		//char*				AsGML(WKBPoint* pWKBPoint);
		//char*				AsGML(WKBLineString* pWKBLineString);
		//char*				AsGML(WKBPolygon* pWKBPolygon);
		//char*				AsGML(WKBMultiPoint* pWKBMultiPoint);
		//char*				AsGML(WKBMultiLineString* pWKBMultiLineString);
		//char*				AsGML(WKBMultiPolygon* pWKBMultiPolygon);

		//bool				AsGML(WKBPoint* pWKBPoint, std::string& gml);
		//bool				AsGML(WKBLineString* pWKBLineString, std::string& gml);
		//bool				AsGML(WKBPolygon* pWKBPolygon, std::string& gml);
		//bool				AsGML(WKBMultiPoint* pWKBMultiPoint, std::string& gml);
		//bool				AsGML(WKBMultiLineString* pWKBMultiLineString, std::string& gml);
		//bool				AsGML(WKBMultiPolygon* pWKBMultiPolygon, std::string& gml);
		//bool				AsGML(LinearRing* pLinearRing, std::string& gml);

		int					CalWKBLengthFromGML(const char* szgml);
		int					CalWKBLengthFromGML(XElement* pxGeometry);
		int					CalWKBLengthFromGMLPoint(XNode* pxGeometry);
		int					CalWKBLengthFromGMLLineString(XElement* pxGeometry);
		int					CalWKBLengthFromGMLPolygon(XElement* pxGeometry);
		int					CalWKBLengthFromGMLMultiPoint(XElement* pxGeometry);
		int					CalWKBLengthFromGMLMultiLineString(XElement* pxGeometry);
		int					CalWKBLengthFromGMLMultiPolygon(XElement* pxGeometry);
		int					CalWKBLengthFromGMLLinearRing(XNode* pxGeometry);
		int					CalWKBLengthFromGMLCoordinates(const char* coordinates);
		int					CalWKBLengthFromGMLPointCount(const char* coordinates);

		bool				CreateWKBFromGML(unsigned char* wkb, int wkbsize, const char* szgml);
		bool				CreateWKBFromGML(unsigned char* wkb, int wkbsize, XElement* pxGeometry);
		bool				CreateWKBFromGMLPoint(WKBPoint* pWKBPoint, XElement* pxGeometry);
		bool				CreateWKBFromGMLLineString(WKBLineString* pWKBLineString, XElement* pxGeometry);
		bool				CreateWKBFromGMLPolygon(WKBPolygon* pWKBPolygon, XElement* pxGeometry);
		bool				CreateWKBFromGMLMultiPoint(WKBMultiPoint* pWKBMultiPoint, XElement* pxGeometry);
		bool				CreateWKBFromGMLMultiLineString(WKBMultiLineString* pWKBMultiLineString, XElement* pxGeometry);
		bool				CreateWKBFromGMLMultiPolygon(WKBMultiPolygon* pWKBMultiPolygon, XElement* pxGeometry);
		bool				CreateWKBFromGMLLinearRing(LinearRing* pLinearRing, XNode* pxLinearRing);
		int					CreateWKBPointsFromGMLCoordinates(Point* pts, const char* coordinates);
		void				CreateWKBPointFromGMLCoordinates(Point* pt, char* coordinates);

		int					FindOutBoundIndex(WKBPolygon* pWKBGeometry);
		void				GetBound(LinearRing* pLinearRing, int dimension, double& dmin, double& dmax);

		int					CalWKBLen(unsigned char* wkb);
		int					CalWKBPointLen(WKBPoint* pWKBPoint);
		int					CalWKBLineStringLen(WKBLineString* pWKBLineString);
		int					CalWKBPolygonLen(WKBPolygon* pWKBPolygon);
		int					CalWKBLinearRingLen(LinearRing* pLinearRing);
		int					CalWKBMultiPointLen(WKBMultiPoint* pWKBMultiPoint);
		int					CalWKBMultiLineStringLen(WKBMultiLineString* pWKBMultiLineString);
		int					CalWKBMultiPolygonLen(WKBMultiPolygon* WKBMultiPolygon);
	};

}

#endif //__AUGE_GML_READER_IMPL_H__