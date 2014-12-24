#ifndef __AUGE_GEOMETRY_H__
#define __AUGE_GEOMETRY_H__

#ifdef WIN32
#	ifdef AUGE_GEOMETRY_EXPORTS
#		define AUGE_GEOMETRY_API __declspec(dllexport)
#	else
#		define AUGE_GEOMETRY_API __declspec(dllimport)
#	endif
#else
#	define AUGE_GEOMETRY_API
#endif

#include "AugeType.h"
#include "AugeObject.h"
#include "wkb.h"

namespace auge
{
	typedef enum augeGeometryType 
	{
		augeGTNull              ,
		augeGTPoint              = 1,
		augeGTLineString         = 2,
		augeGTPolygon            = 3,
		augeGTMultiPoint         = 4,
		augeGTMultiLineString    = 5,
		augeGTMultiPolygon       = 6,
		augeGTGeometryCollection = 7,
		augeGTTriangle			 = 8
	}augeGeometryType;

	class GEnvelope;
	class GMultiCurve;
	
	class Geometry : public GObject 
	{
	protected:
		Geometry(){}
		virtual ~Geometry(){}
	public:
		//--------------------------------------------------------------------------
		//base
		//virtual       int			Dimension()           const =0;		// : Integer
		virtual augeGeometryType	GeometryType()			    =0;		// : String
		//virtual       int			GetSRID()             const =0;		// : Integer
		//virtual       void			SetSRID(int srid)           =0;		// : Integer
		virtual const GEnvelope*	Envelope()            const =0;		// : Gometry
		//virtual       GEnvelope*	Envelope(bool refresh=false)=0;		// : Gometry
		//virtual		  bool			Envelope(GEnvelope& envelope) =0;	// : Gometry
		virtual       const char*	AsText()					=0;		// : String.Release
		virtual       g_uchar*		AsBinary()            const =0;		// : Binary.
	};

	class GPoint : public Geometry
	{
	protected:
		GPoint(){}
		virtual ~GPoint(){}
	//public:
	//	//--------------------------------------------------------------------------
	//	virtual double X() const;// : Double
	//	virtual double Y() const;// : Double
	//	virtual double Z() const;// : Double
	//	virtual double M() const;// : Double

	//public:
	//	//--------------------------------------------------------------------------
	//	virtual void   SetX(double x);// : Double
	//	virtual void   SetY(double y);// : Double
	//	virtual void   SetZ(double z);// : Double
	//	virtual void   SetM(double m);// : Double
	};
	
	class GCurve : public Geometry
	{
	protected:
		GCurve(){}
		virtual ~GCurve(){}
	public:
		////--------------------------------------------------------------------------
		//virtual       double	Length()     const =0; // : Double
		//virtual const GPoint*	StartPoint() const =0; // : Point
		//virtual       GPoint*	StartPoint()       =0; // : Point
		//virtual const GPoint*	EndPoint()   const =0; // : Point
		//virtual       GPoint*	EndPoint()         =0; // : Point
		//virtual       bool		IsClosed()   const =0; // : Boolean
		//virtual       bool		IsRing()     const =0; // : Boolean
	};

	class GLineString : public GCurve
	{
	protected:
		GLineString(){}
		virtual ~GLineString(){}

	public:
		//--------------------------------------------------------------------------
		//virtual       int		NumPoints()			const =0; // : Integer
		//virtual const GPoint*	PointN(g_uint n)	const =0; // : Point
		//virtual       GPoint*	PointN(g_uint n)          =0; // : Point
		//virtual       bool		PointN(g_uint n, double& x, double &y) =0; // : Point
		//virtual		  bool		SetPointN(g_uint n, double x, double y) = 0;
		//virtual       bool		CloseRings() =0;
		//virtual       double	PointX(g_uint n)	const =0;
		//virtual       double	PointY(g_uint n)	const =0;
	};

	class GLinearRing : public GLineString
	{
	protected:
		GLinearRing(){}
		virtual ~GLinearRing(){}
	};

	class GSurface : public Geometry
	{
	protected:
		GSurface(){}
		virtual ~GSurface(){}
	public:
		//--------------------------------------------------------------------------
		//virtual double			Area()           const =0; // : Area
		//virtual GPoint*			Centroid()       const =0; // : Point
		//virtual GPoint*			PointOnSurface() const =0; // : Point
		//virtual GMultiCurve*	Boundary()       const =0; // : MultiCurve
	};

	class GPolygon : public GSurface
	{
	protected:
		GPolygon(){}
		virtual ~GPolygon(){}
	public:
	//	//--------------------------------------------------------------------------
	//	virtual const GLineString*	ExterorRing()        const =0; // : LineString
	//	virtual       GLineString*	ExterorRing()              =0; // : LineString
	//	virtual       int			NumInteriorRing()    const =0; // : Integer
	//	virtual const GLineString*	InteriorRingN(int n) const =0; // : LineString
	//	virtual       GLineString*	InteriorRingN(int n)       =0; // : LineString
	//public:
	//	//--------------------------------------------------------------------------
	//	virtual       int			NumRing()                         const =0;
	//	virtual const GLineString*	RingN(int n)                      const =0;
	//	virtual       GLineString*	RingN(int n)                            =0;
	//	virtual       bool			AddRing(const GLineString* ring)        =0;
	//	virtual       bool			AddRing(GLineString* ring)              =0;
	//	virtual       bool			AddRing(GPoint* pPoints,int iPointCount) =0;
	};

	class GeometryCollection : public Geometry
	{
	protected:
		GeometryCollection(){}
		virtual ~GeometryCollection(){}
	public:
		//virtual int			NumGeometries() =0;
		//virtual Geometry*	GeometryN(int n) =0;
		//virtual bool		AddGeometry(Geometry* geometry) =0;
	};

	/**
	* Auge Geometry MultiPoint
	* 
	* A MultiPoint is a 0-dimensional GeometryCollection. 
	*/
	class GMultiPoint : public GeometryCollection
	{
	protected:
		GMultiPoint(){}
		virtual ~GMultiPoint(){}
	public:

	};

	class GMultiLineString : public GeometryCollection
	{
	public:
		GMultiLineString(){}
		virtual ~GMultiLineString(){}
	public:
		//--------------------------------------------------------------------------
		//virtual       bool    AddLineString(GLineString *pLingString) = 0;
	};

	class GMultiPolygon  :public GeometryCollection
	{
	public:
		GMultiPolygon(){}
		virtual ~GMultiPolygon(){}
	public:
		//--------------------------------------------------------------------------
		//virtual bool      AddPolygon(GPolygon* pPolygon) =0;
	};

	class AUGE_GEOMETRY_API GEnvelope
	{
	public:
		GEnvelope();
		GEnvelope(double xmin, double ymin, double xmax, double ymax);
		virtual ~GEnvelope();

	public:
		void	Set(double xmin, double ymin, double xmax, double ymax);
		void	Union(GEnvelope& o);

		double	GetWidth();
		double	GetHeight();

		bool	IsValid();
	public:
		double	m_xmin, m_ymin,m_xmax, m_ymax;
	};

	class GeometryFactory
	{
	protected:
		GeometryFactory(){}
		virtual ~GeometryFactory(){}
	public:
		virtual	Geometry*	CreateGeometryFromWKB(g_uchar* wkb, bool attach=false) = 0;
		virtual Geometry*	CreateGeometryFromWKT(const char* wkt) = 0;
	};

	class GeometryUtil
	{
	protected:
		GeometryUtil(){}
		virtual ~GeometryUtil(){}
	public:
		virtual g_uint	GetWKBSize(const g_uchar* wkb) = 0;
	};

	extern "C"
	{
		AUGE_GEOMETRY_API GeometryFactory*	augeGetGeometryFactoryInstance();
		AUGE_GEOMETRY_API GeometryUtil*		augeGetGeometryUtilInstance();
	}
}


#endif __AUGE_GEOMETRY_H__
