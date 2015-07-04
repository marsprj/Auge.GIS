#ifndef __AUGE_WKB_H__
#define __AUGE_WKB_H__

namespace auge
{
#pragma pack(1)

	// Basic Type definitions
	// byte : 1 byte                              : unsigned char
	// uint32 : 32 bit unsigned integer (4 bytes) : __int32
	// double : double precision number (8 bytes)
	// Building Blocks : Coordinate, LinearRing
	struct Point{
		double x;
		double y;
	};
	struct PointZ{
		double x;
		double y;
		double z;
	};
	struct PointM {
		double x;
		double y;
		double m;
	};
	struct PointZM {
		double x;
		double y;
		double z;
		double m;
	};
	struct LinearRing {
		int numPoints;
		Point   points[1];
	};
	struct LinearRingZ {
		int numPoints;
		PointZ  points[1];
	};
	struct LinearRingM {
		int numPoints;
		PointM  points[1];
	};
	struct LinearRingZM {
		int numPoints;
		PointZM points[1];
	};

	enum WKBByteOrder {
		wkbXDR, // Big Endian    : MacOS
		wkbNDR = 1  // Little Endian : Windows
	};
	//定义系统可处理的字节序
#ifdef WIN32
#define coDefaultByteOrder wkbNDR
#else 
	//#define coDefaultByteOrder wkbXDR
#define coDefaultByteOrder wkbNDR
#endif

	enum WKBGeometryType {
		wkbNull                ,
		wkbPoint                = 1,
		wkbLineString           = 2,
		wkbPolygon              = 3,
		wkbTriangle             = 17,
		wkbMultiPoint           = 4,
		wkbMultiLineString      = 5,
		wkbMultiPolygon         = 6,
		wkbGeometryCollection   = 7,
		wkbPolyhedralSurface    = 15,
		wkbTIN                  = 16,
		wkbPointZ               = 1001,
		wkbLineStringZ          = 1002,
		wkbPolygonZ             = 1003,
		wkbTrianglez            = 1017,
		wkbMultiPointZ          = 1004,
		wkbMultiLineStringZ     = 1005,
		wkbMultiPolygonZ        = 1006,
		wkbGeometryCollectionZ  = 1007,
		wkbPolyhedralSurfaceZ   = 1015,
		wkbTINZ                 = 1016,
		wkbPointM               = 2001,
		wkbLineStringM          = 2002,
		wkbPolygonM             = 2003,
		wkbTriangleM            = 2017,
		wkbMultiPointM          = 2004,
		wkbMultiLineStringM     = 2005,
		wkbMultiPolygonM        = 2006,
		wkbGeometryCollectionM  = 2007,
		wkbPolyhedralSurfaceM   = 2015,
		wkbTINM                 = 2016,
		wkbPointZM              = 3001,
		wkbLineStringZM         = 3002,
		wkbPolygonZM            = 3003,
		wkbTriangleZM           = 3017,
		wkbMultiPointZM         = 3004,
		wkbMultiLineStringZM    = 3005,
		wkbMultiPolygonZM       = 3006,
		wkbGeometryCollectionZM = 3007,
		wkbPolyhedralSurfaceZM  = 3015,
		wkbTinZM                = 3016,
		wkbRaster               = 21    //Raster Data
	};

	struct WKBGeometryInfo{
		unsigned char	byteOrder;
		int				wkbType; 
	};

	struct WKBPoint{
		unsigned char	byteOrder;
		int				wkbType;// = 1;
		Point			point;
	};
	struct WKBPointZ {
		unsigned char	byteOrder;
		int				wkbType;// = 1001;
		PointZ			point;
	};
	struct WKBPointM {
		unsigned char	byteOrder;
		int				wkbType;// = 2001;
		PointM			point;
	};
	struct WKBPointZM {
		unsigned char	byteOrder;
		int				wkbType;// = 3001;
		PointZM			point;
	};
	struct WKBLineString {
		unsigned char	byteOrder;
		int				wkbType;// = 2;
		int				numPoints;
		Point			points[1];
	};
	struct WKBLineStringZ {
		unsigned char	byteOrder;
		int				wkbType;// = 1002;
		int				numPoints;
		PointZ			points[1];
	};
	struct WKBLineStringM {
		unsigned char	byteOrder;
		int				wkbType;// = 2002;
		int				numPoints;
		PointM			points[1];
	};
	struct WKBLineStringZM {
		unsigned char	byteOrder;
		int				wkbType;// = 3002;
		int				numPoints;
		PointZM			points[1];
	};
	struct WKBPolygon {
		unsigned char	byteOrder;
		int				wkbType;// = 3;
		int				numRings;
		LinearRing		rings[1];
	};
	struct WKBPolygonZ {
		unsigned char	byteOrder;
		int				wkbType;// = 1003;
		int				numRings;
		LinearRingZ		rings[1];
	};
	struct WKBPolygonM {
		unsigned char	byteOrder;
		int				wkbType;// = 2003;
		int				numRings;
		LinearRingM		rings[1];
	};
	struct WKBPolygonZM {
		unsigned char	byteOrder;
		int				wkbType;// = 3003;
		int				numRings;
		LinearRingZM	rings[1];
	};
	struct WKBTriangle {
		unsigned char	byteOrder;
		int				wkbType;// = 17;
		int				numRings;
		LinearRing		rings[1];
	};
	struct WKBTriangleZ {
		unsigned char	byteOrder;
		int				wkbType;// = 1017;
		int				numRings;
		LinearRingZ		rings[1];
	};
	struct WKBTriangleM {
		unsigned char	byteOrder;
		int				wkbType;// = 2017;
		int				numRings;
		LinearRingM		rings[1];
	};
	struct WKBTriangleZM {
		unsigned char	byteOrder;
		int				wkbType;// = 3017;
		int				numRings;
		LinearRingZM	rings[1];
	};
	struct WKBPolyhedralSurface {
		unsigned char	byteOrder;
		int				wkbType;// = 15;
		int				numPolygons;
		WKBPolygon		polygons[1];
	};
	struct WKBPolyhedralSurfaceZ {
		unsigned char	byteOrder;
		int				wkbType;//=1015;
		int				numPolygons;
		WKBPolygonZ		polygons[1];
	};
	struct WKBPolyhedralSurfaceM {
		unsigned char	byteOrder;
		int				wkbType;//=2015;
		int				numPolygons;
		WKBPolygonM		polygons[1];
	};

	struct WKBPolyhedralSurfaceZM {
		unsigned char	byteOrder;
		int				wkbType;//=3015;
		int				numPolygons;
		WKBPolygonZM	polygons[1];
	};
	struct WKBTIN {
		unsigned char	byteOrder;
		int				wkbType;// = 16;
		int				numPolygons;
		WKBPolygon		polygons[1];
	};
	struct WKBTINZ {
		unsigned char	byteOrder;
		int				wkbType;//=1016;
		int				numPolygons;
		WKBPolygonZ		polygons[1];
	};
	struct WKBTINM {
		unsigned char	byteOrder;
		int				wkbType;//=2016;
		int				numPolygons;
		WKBPolygonM		polygons[1];
	};
	struct WKBTINZM {
		unsigned char	byteOrder;
		int				wkbType;//=3016;
		int				numPolygons;
		WKBPolygonZM	polygons[1];
	};
	struct WKBMultiPoint {
		unsigned char	byteOrder;
		int				wkbType;//=4;
		int				numPoints;
		WKBPoint		points[1];
	};
	struct WKBMultiPointZ {
		unsigned char	byteOrder;
		int				wkbType;//=1004;
		int				numPoints;
		WKBPointZ		points[1];
	};
	struct WKBMultiPointM {
		unsigned char	byteOrder;
		int				wkbType;//=2004;
		int				numPoints;
		WKBPointM		points[1];
	};
	struct WKBMultiPointZM {
		unsigned char	byteOrder;
		int				wkbType;//=3004;
		int				numPoints;
		WKBPointZM		points[1];
	};
	struct WKBMultiLineString {
		unsigned char	byteOrder;
		int				wkbType;// = 5;
		int				numLineStrings;
		WKBLineString	lineStrings[1];
	};
	struct WKBMultiLineStringZ {
		unsigned char	byteOrder;
		int				wkbType;// = 1005;
		int				numLineStrings;
		WKBLineStringZ	lineStrings[1];
	};
	struct WKBMultiLineStringM {
		unsigned char	byteOrder;
		int				wkbType;// = 2005;
		int				numLineStrings;
		WKBLineStringM	lineStrings[1];
	};
	struct WKBMultiLineStringZM {
		unsigned char   byteOrder;
		int				wkbType;// = 3005;
		int				numLineStrings;
		WKBLineStringZM lineStrings[1];
	};
	struct WKBMultiPolygon {
		unsigned char	byteOrder;
		int				wkbType;// = 6;
		int				numPolygons;
		WKBPolygon		polygons[1];
	};
	struct WKBMultiPolygonZ {
		unsigned char	byteOrder;
		int				wkbType;// = 1006;
		int				numPolygons;
		WKBPolygonZ		polygons[1];
	};
	struct WKBMultiPolygonM {
		unsigned char	byteOrder;
		int				wkbType;// = 2006;
		int				numPolygons;
		WKBPolygonM		polygons[1];
	};
	struct WKBMultiPolygonZM {
		unsigned char	byteOrder;
		int				wkbType;// = 3006;
		int				numPolygons;
		WKBPolygonZM	polygons[1];
	};

	struct WKBGeometry {
		union {
			WKBPoint              point;
			WKBLineString         linestring;
			WKBPolygon            polygon;
			WKBTriangle           triangle;
			WKBPolyhedralSurface  polyhedralsurface;
			WKBTIN                tin;
			WKBMultiPoint         mpoint;
			WKBMultiLineString    mlinestring;
			WKBMultiPolygon       mpolygon;
			//WKBGeometryCollection collection;
		};
	};

	struct WKBGeometryZ {
		union {
			WKBPointZ              pointz;
			WKBLineStringZ         linestringz;
			WKBPolygonZ            polygonz;
			WKBTriangleZ           trianglez;
			WKBPolyhedralSurfaceZ  polyhedralsurfacez;
			WKBTINZ                tinz;
			WKBMultiPointZ         mpointz;
			WKBMultiLineStringZ    mlinestringz;
			WKBMultiPolygonZ       mpolygonz;
			//WKBGeometryCollectionZ collectionz;
		};
	};
	struct WKBGeometryM {
		union {
			WKBPointM              pointm;
			WKBLineStringM         linestringm;
			WKBPolygonM            polygonm;
			WKBTriangleM           trianglem;
			WKBPolyhedralSurfaceM  polyhedralsurfacem;
			WKBTINM                tinm;
			WKBMultiPointM         mpointm;
			WKBMultiLineStringM    mlinestringm;
			WKBMultiPolygonM       mpolygonm;
			//WKBGeometryCollectionM collectionz;
		};
	};
	struct WKBGeometryZM {
		union {
			WKBPointZM              pointzm;
			WKBLineStringZM         linestringzm;
			WKBPolygonZM            polygonzm;
			WKBTriangleZM           trianglezm;
			WKBPolyhedralSurfaceZM  polyhedralsurfacezm;
			WKBTINZM                tinzm;
			WKBMultiPointZM         mpointzm;
			WKBMultiLineStringZM    mlinestringzm;
			WKBMultiPolygonZM       mpolygonzm;
			//WKBGeometryCollectionZM collectionzm;
		};
	};
	struct WKBGeometryCollection {
		unsigned char byteOrder;
		int       wkbType;// = 7;
		int       numGeometries;
		WKBGeometry   geometries[1];
	};
	struct WKBGeometryCollectionZ {
		unsigned char byteOrder;
		int       wkbType;// = 1007;
		int       numGeometries;
		WKBGeometryZ  geometries[1];
	};
	struct WKBGeometryCollectionM {
		unsigned char byteOrder;
		int       wkbType;// = 2007;
		int       numGeometries;
		WKBGeometryM  geometries[1];
	};
	struct WKBGeometryCollectionZM {
		unsigned char byteOrder;
		int       wkbType;// = 3007;
		int       numGeometries;
		WKBGeometryZM geometries[1];
	};

	// typedef 
	typedef struct Point	auge_point_t;

	// struct WKBRaster{
	// 	unsigned char byteOrder;
	// 	int       wkbType;// = 21;
	// 	int       datatype;
	// 	int       datasize;
	// 	unsigned char data[1]; // 布局：xmin(double),ymin(double),xmax(double),yamx(double),imagedata(dib)
	// };

#define	SizeOfWKBLinearRing(ptr) (((ptr)->numPoints - 1) * sizeof(auge::Point) + sizeof(auge::LinearRing))
#define SizeOfWKBLineString(ptr) (((ptr)->numPoints - 1) * sizeof(auge::Point) + sizeof(auge::WKBLineString))
#define	WKBBYTEORDER(ptr)        (((auge::WKBGeometryInfo *)(ptr))->byteOrder)
#define	WKBTYPE(ptr)             (((auge::WKBGeometryInfo *)(ptr))->wkbType)

	// Geometry pack tag macro defines
#define TAG_SETBYTEODR(t,o)		((t)=(((t)&0xF0)|(o))) // o == 1 == Big Endian, o == 2 == Little Endian 
#define TAG_SETZM(t,z,m)		((t)=(((t)&0xCF)|((z)<<5)|((m)<<4)))
#define TAG_SETHASBBOX(t,b)		((t)=(((t)&0x7F)|((b)<<7)))
#define TAG_SETHASSRID(t,s)		((t)=(((t)&0xBF)|((s)<<6)))

#define TAG_HASZ(t)				(((t)&0x20)>>5)
#define TAG_HASM(t)				(((t)&0x10)>>4)
#define TAG_HASBBOX(t)			(((t)&0x80)>>7)
#define TAG_HASSRID(t)			((((t)&0x40))>>6)
#define TAG_NDIMS(t)			((((t)&0x20)>>5)+(((t)&0x10)>>4)+2)
#define TAG_GETZM(t)			(((t)&0x30)>>4)	// 0x02==Z 0x01==M
#define TAG_GETBYTEODR(t)		((t)&0x0F)		// 1 == Big Endian, 2 == Little Endian

#pragma pack()
}

#endif //__AUGE_WKB_H__