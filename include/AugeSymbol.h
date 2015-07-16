#ifndef __AUGE_SYMBOL_H__
#define __AUGE_SYMBOL_H__

#include "AugeObject.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeGeometry.h"
#include "AugeFilter.h"
#include "AugeFeature.h"

#ifdef WIN32
#	ifdef AUGE_CARTO_EXPORTS
#		define AUGE_CARTO_API __declspec(dllexport)
#	else
#		define AUGE_CARTO_API __declspec(dllimport)
#	endif
#else
#	define AUGE_CARTO_API
#endif

namespace auge
{
	class Symbolizer;
	class PointSymbolizer;
	class LineSymbolizer;
	class PolygonSymbolizer;
	class TextSymbolizer;
	class ExternalGraphic;
	class Stroke;
	class Fill;
	class Graphic;
	class GraphicFill;
	class GraphicStroke;
	class Font;
	class Mark;
	class StyleFactory;
	class Transformation;
	class Renderer;

	class MarkerSymbol;
	class LineSymbol;
	class RegionSymbol;

	//========================================================================
	// augeSymbolType Enum
	//========================================================================
	typedef enum augeSymbolizerType
	{
		augeSymbolPoint		= 0,
		augeSymbolLine		= 1,
		augeSymbolPolygon	= 2,
		augeSymbolText		= 3,
		augeSymbolRaster	= 4
	}augeSymbolizerType;

	//========================================================================
	// LineCap Enum
	//========================================================================
	typedef enum
	{
		augeButtCap		= 0,
		augeSquareCap	= 1,
		augeRoundCap	= 2,
		augeLineCapMAX	= 3
	}augeLineCap; 

	//========================================================================
	// augeLineJoin Enum
	//========================================================================
	typedef enum
	{
		augeMiterJoin		= 0,
		augeMiterRevertJoin	= 1,
		augeRoundJoin		= 2,
		augeBevelJoin		= 3,
		augeLineJoinMax		= 4
	}augeLineJoin;

	//========================================================================
	// augeMarkerType Enum
	//========================================================================
	typedef enum
	{
		augeMarkerSquare		= 0,
		augeMarkerCircle		= 1,
		augeMarkerTriangle		= 2,
		augeMarkerStar			= 3,
		augeMarkerCross			= 4,
		augeMarkerX				= 5,
		augeMarkerPentagon		= 6,
		augeMarkerCapital		= 7
	}augeMarkerType;

	//========================================================================
	// augeLineType Enum
	//========================================================================
	typedef enum
	{
		augeLineSimple			= 0,
		augeLineRailway			= 1
	}augeLineType;

	//========================================================================
	// augeLineType Enum
	//========================================================================
	typedef enum
	{
		augeRegionSimple		= 0,
		augeRegionGrass			= 1
	}augeRegionType;

	//========================================================================
	// Auge Symbolizer
	//========================================================================
	class Symbolizer : public GObject
	{
	protected:
		Symbolizer(){}
		virtual ~Symbolizer(){}
	public:
		virtual augeSymbolizerType	GetType() = 0;
	};

	//========================================================================
	// Auge Fill
	//------------------------------------------------------------------------
	//<xsd:element name="Fill" type="se:FillType"/> 
	//<xsd:complexType name="FillType"> 
	//	<xsd:sequence> 
	//		<xsd:element ref="se:GraphicFill" minOccurs="0"/> 
	//		<xsd:element ref="se:SvgParameter" minOccurs="0" maxOccurs="unbounded"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//========================================================================
	class Fill : public GObject
	{
	public:
		Fill(){}
		virtual ~Fill(){}
	public:
		virtual GColor&		GetColor() = 0;
		virtual void		SetColor(GColor& color) = 0;

		virtual float		GetOpacity() = 0;
		virtual void		SetOpacity(float opacity) = 0;

		virtual void		Release() = 0;
	};

	//========================================================================
	// Auge Stroke
	//------------------------------------------------------------------------
	//<xsd:element name="Stroke" type="se:StrokeType"/> 
	//	<xsd:complexType name="StrokeType"> 
	//		<xsd:sequence> 
	//			<xsd:choice minOccurs="0"> 
	//			<xsd:element ref="se:GraphicFill"/> 
	//			<xsd:element ref="se:GraphicStroke"/> 
	//		</xsd:choice> 
	//		<xsd:element ref="se:SvgParameter" minOccurs="0" maxOccurs="unbounded"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//========================================================================
	class Stroke : public GObject
	{
	public:
		Stroke(){}
		virtual ~Stroke(){}
	public:
		virtual GColor&			GetColor() = 0;
		virtual void			SetColor(GColor& color) = 0;

		virtual augeLineCap		GetLinecap() = 0;
		virtual void			SetLinecap(augeLineCap linecap) = 0;

		virtual augeLineJoin	GetLinejoin() = 0;
		virtual void			SetLinejoin(augeLineJoin linejoin) = 0;

		virtual float			GetOpacity() = 0;
		virtual void			SetOpacity(float opacity) = 0;

		virtual float			GetWidth() = 0;
		virtual void			SetWidth(float width) = 0;

		virtual float			GetDashoffset() = 0;	
		virtual void			SetDashoffset(float dash_offset) = 0;

		virtual GraphicFill*	GetGraphicFill() = 0;
		virtual void			SetGraphicFill(GraphicFill* pGraphicFill) = 0;

		virtual GraphicStroke*	GetGraphicStroke() = 0;
		virtual void			SetGraphicStroke(GraphicStroke* pGraphicFill) = 0;

		virtual Stroke&			operator=(const Stroke& rhs) = 0;

		virtual void			 Release() = 0;
	};

	//========================================================================
	// Auge GraphicStroke
	//------------------------------------------------------------------------
	// A Graphic is a ¡°graphic symbol¡± with an inherent shape, color(s), and possibly size. A 
	//¡°graphic¡± can be very informally defined as ¡°a little picture¡± and can be of either a raster 
	// or vector-graphic source type. 
	//------------------------------------------------------------------------
	//<xsd:element name="Graphic" type="se:GraphicType"/> 
	//<xsd:complexType name="GraphicType"> 
	//   <xsd:sequence> 
	//    <xsd:choice minOccurs="0" maxOccurs="unbounded"> 
	//     <xsd:element ref="se:ExternalGraphic"/> 
	//     <xsd:element ref="se:Mark"/> 
	//    </xsd:choice> 
	//    <xsd:element ref="se:Opacity" minOccurs="0"/> 
	//    <xsd:element ref="se:Size" minOccurs="0"/> 
	//    <xsd:element ref="se:Rotation" minOccurs="0"/> 
	//    <xsd:element ref="se:AnchorPoint" minOccurs="0"/> 
	//    <xsd:element ref="se:Displacement" minOccurs="0"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//
	//<xsd:element name="Opacity" type="se:ParameterValueType"/> 
	//<xsd:element name="Size" type="se:ParameterValueType"/> 
	//<xsd:element name="Rotation" type="se:ParameterValueType"/> 
	//
	//<xsd:element name="AnchorPoint" type="se:AnchorPointType"/> 
	//<xsd:complexType name="AnchorPointType"> 
	//<xsd:sequence> 
	//<xsd:element ref="se:AnchorPointX"/> 
	//<xsd:element ref="se:AnchorPointY"/> 
	//</xsd:sequence> 
	//</xsd:complexType> 
	//
	//<xsd:element name="AnchorPointX" type="se:ParameterValueType"/> 
	//<xsd:element name="AnchorPointY" type="se:ParameterValueType"/> 
	//
	//<xsd:element name="Displacement" type="se:DisplacementType"/> 
	//	<xsd:complexType name="DisplacementType"> 
	//	<xsd:sequence> 
	//		<xsd:element ref="se:DisplacementX"/> 
	//		<xsd:element ref="se:DisplacementY"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//
	//<xsd:element name="DisplacementX" type="se:ParameterValueType"/> 
	//<xsd:element name="DisplacementY" type="se:ParameterValueType"/>
	//
	//<xsd:element name="ExternalGraphic" type="se:ExternalGraphicType"/> 
	//<xsd:complexType name="ExternalGraphicType"> 
	//	<xsd:sequence> 
	//		<xsd:choice> 
	//			<xsd:element ref="se:OnlineResource"/> 
	//			<xsd:element ref="se:InlineContent"/> 
	//		</xsd:choice> 
	//		<xsd:element ref="se:Format"/> 
	//		<xsd:element ref="se:ColorReplacement" minOccurs="0" maxOccurs="unbounded"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//
	//<xsd:element name="ColorReplacement" type="se:ColorReplacementType"/> 
	//<xsd:complexType name="ColorReplacementType"> 
	//	<xsd:sequence> 
	//		<xsd:element ref="se:Recode"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//
	//<xsd:element name="Format" type="xsd:string"/> 
	//========================================================================
	class Graphic : public GObject
	{
	protected:
		Graphic(){}
		virtual ~Graphic(){}
	public:
		virtual float		GetOpacity() = 0;
		virtual void		SetOpacity(float opacity) = 0;

		virtual int			GetSize() = 0;
		virtual void		SetSize(int size) = 0;

		virtual float		GetRotation() = 0;
		virtual void		SetRotation(float rotation) = 0;

		virtual float		GetAnchor_X() = 0;
		virtual float		GetAnchor_Y() = 0;
		virtual void		SetAnchor(float x, float y) = 0;

		virtual float		GetDisplacement_X() = 0;
		virtual float		GetDisplacement_Y() = 0;
		virtual void		SetDisplacement(float x, float y) = 0;

		virtual Mark*		GetMark() = 0;
		virtual bool		SetMark(Mark* pMark) = 0;

		virtual ExternalGraphic*	GetExternalGraphic() = 0;
		virtual bool		SetExternalGraphic(ExternalGraphic* pExternalGraphic) = 0;
	};

	class ExternalGraphic : public GObject
	{
	protected:
		ExternalGraphic(){}
		virtual ~ExternalGraphic(){}
	public:
		virtual const char* GetResource() = 0;
		virtual void		SetResource(const char* resource) = 0;

		virtual const char*	GetFormat() = 0;
		virtual void		SetFormat(const char* format) = 0;
	};

	//========================================================================
	// Auge PointSymbolizer
	//------------------------------------------------------------------------
	//<xsd:element name="PointSymbolizer" type="se:PointSymbolizerType" substitutionGroup="se:Symbolizer"/> 
	//	<xsd:complexType name="PointSymbolizerType"> 
	//		<xsd:complexContent> 
	//			<xsd:extension base="se:SymbolizerType"> 
	//			<xsd:sequence> 
	//				<xsd:element ref="se:Geometry" minOccurs="0"/> 
	//				<xsd:element ref="se:Graphic" minOccurs="0"/> 
	//			</xsd:sequence> 
	//		</xsd:extension> 
	//	</xsd:complexContent> 
	//</xsd:complexType> 
	//========================================================================
	class PointSymbolizer : public Symbolizer
	{
	protected:
		PointSymbolizer(){}
		virtual ~PointSymbolizer(){}
	public:
		virtual const char*		GetGeometry() = 0;
		virtual void			SetGeometry(const char* prop_name) = 0;

		virtual void			SetGraphic(Graphic* pGraphic) = 0;
		virtual Graphic*		GetGraphic() = 0;

		//virtual void			SetMarkType(augeMarkerType type) = 0;
		//virtual augeMarkerType	GetMarkType() = 0;

		//virtual void			SetStroke(Stroke* pStroke) = 0;
		//virtual Stroke*			GetStroke() = 0;

		//virtual void			SetFill(Fill* pFill) = 0;
		//virtual Fill*			GetFill() = 0;

		//virtual void			SetOpacity(float opacity) = 0;
		//virtual float			GetOpacity() = 0;

		//virtual void			SetSize(float size) = 0;
		//virtual float			GetSize() = 0;

		//virtual void			SetRotation(float rotation) = 0;
		//virtual float			GetRotation() = 0;

		virtual void			SetMarker(MarkerSymbol* pMarker) = 0;
		virtual MarkerSymbol*	GetMarker() = 0;
	};

	//========================================================================
	// Auge LineSymbolizer
	//------------------------------------------------------------------------
	//<xsd:element name="LineSymbolizer" type="se:LineSymbolizerType" substitutionGroup="se:Symbolizer"/> 
	//<xsd:complexType name="LineSymbolizerType"> 
	//	<xsd:complexContent> 
	//		<xsd:extension base="se:SymbolizerType"> 
	//			<xsd:sequence> 
	//				<xsd:element ref="se:Geometry" minOccurs="0"/> 
	//				<xsd:element ref="se:Stroke" minOccurs="0"/> 
	//				<xsd:element ref="se:PerpendicularOffset" minOccurs="0"/> 
	//			</xsd:sequence> 
	//		</xsd:extension> 
	//	</xsd:complexContent> 
	//</xsd:complexType> 
	//========================================================================
	class LineSymbolizer : public Symbolizer
	{
	public:
		LineSymbolizer(){}
		virtual ~LineSymbolizer(){}
	public:
		virtual const char*		GetGeometry() = 0;
		virtual Stroke*			GetStroke() = 0;

		virtual bool			SetGeometry(const char* prop_name) = 0;
		virtual bool			SetStroke(Stroke* pStroke) = 0;

		virtual void			SetLineType(augeLineType type) = 0;
		virtual augeLineType	GetLineType() = 0;

		virtual void			SetSymbol(LineSymbol* pSymbol) = 0;
		virtual LineSymbol*		GetSymbol() = 0;
	};

	//========================================================================
	// Auge PolygonSymbolizer
	//------------------------------------------------------------------------
	//<xsd:element name="PolygonSymbolizer" type="se:PolygonSymbolizerType" substitutionGroup="se:Symbolizer"/> 
	//<xsd:complexType name="PolygonSymbolizerType"> 
	//	<xsd:complexContent> 
	//		<xsd:extension base="se:SymbolizerType"> 
	//			<xsd:sequence> 
	//				<xsd:element ref="se:Geometry" minOccurs="0"/> 
	//				<xsd:element ref="se:Fill" minOccurs="0"/> 
	//				<xsd:element ref="se:Stroke" minOccurs="0"/> 
	//				<xsd:element ref="se:Displacement" minOccurs="0"/> 
	//				<xsd:element ref="se:PerpendicularOffset" minOccurs="0"/> 
	//			</xsd:sequence> 
	//		</xsd:extension> 
	//	</xsd:complexContent> 
	//</xsd:complexType>  
	//========================================================================
	class PolygonSymbolizer : public Symbolizer
	{
	public:
		PolygonSymbolizer(){}
		virtual ~PolygonSymbolizer(){}
	public:
		virtual Fill*			GetFill() = 0;
		virtual void			SetFill(Fill* pFill) = 0;

		virtual Stroke*			GetStroke() = 0;
		virtual void			SetStroke(Stroke* pStroke) = 0;

		virtual void			SetOpacity(float opacity) = 0;
		virtual float			GetOpacity() = 0;

		virtual float			GetDisplacement_X() = 0;
		virtual float			GetDisplacement_Y() = 0;
		virtual void			SetDisplacement(float x, float y) = 0;

		virtual const char*		GetGemetry() = 0;
		virtual void			SetGeometry(const char* prop_name) = 0;

		virtual void			SetSymbol(RegionSymbol* pSymbol) = 0;
		virtual RegionSymbol*	GetSymbol() = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	// Marker Symbol
	//////////////////////////////////////////////////////////////////////////

	class Symbol : public GObject
	{
	protected:
		Symbol(){}
		virtual ~Symbol(){}
	public:
		virtual const char*			GetName() = 0;
		virtual const char*			GetIcon() = 0;
		virtual augeSymbolizerType	GetType() = 0;
		virtual RESULTCODE			Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform) = 0;
	};

	class MarkerSymbol : public Symbol
	{
	protected:
		MarkerSymbol(){}
		virtual ~MarkerSymbol(){}
	public:
		virtual void			SetStroke(Stroke* pStroke) = 0;
		virtual Stroke*			GetStroke() = 0;

		virtual void			SetFill(Fill* pFill) = 0;
		virtual Fill*			GetFill() = 0;

		virtual void			SetOpacity(float opacity) = 0;
		virtual float			GetOpacity() = 0;

		virtual void			SetSize(float size) = 0;
		virtual float			GetSize() = 0;

		virtual void			SetRotation(float rotation) = 0;
		virtual float			GetRotation() = 0;
	};

	class LineSymbol : public Symbol
	{
	public:
		LineSymbol(){}
		virtual ~LineSymbol(){}

	public:
		virtual Stroke*			GetStroke() = 0;
		virtual void			SetStroke(Stroke* pStroke) = 0;
		virtual augeLineType	GetLineType() = 0;
	};

	class RegionSymbol : public Symbol
	{
	public:
		RegionSymbol(){}
		virtual ~RegionSymbol(){}

	public:
		virtual Fill*			GetFill() = 0;
		virtual void			SetFill(Fill* pFill) = 0;

		virtual Stroke*			GetStroke() = 0;
		virtual void			SetStroke(Stroke* pStroke) = 0;

		virtual void			SetOpacity(float opacity) = 0;
		virtual float			GetOpacity() = 0;

		//virtual float			GetDisplacement_X() = 0;
		//virtual float			GetDisplacement_Y() = 0;
		//virtual void			SetDisplacement(float x, float y) = 0;
	};

	class EnumSymbol : public GObject
	{
	protected:
		EnumSymbol(){}
		virtual ~EnumSymbol(){}
	public:
		virtual void			Reset() = 0;
		virtual Symbol*			Next() = 0;
		virtual void			Release() = 0;
	};

	class SymbolManager
	{
	protected:
		SymbolManager(){}
		virtual ~SymbolManager(){}

	public:
		virtual EnumSymbol*		GetMarkerSymbols() = 0;
		virtual EnumSymbol*		GetLineSymbols() = 0;
		virtual EnumSymbol*		GetRegionSymbols() = 0;

		virtual MarkerSymbol*	CreateMarkerSymbol(augeMarkerType type) = 0;
		virtual MarkerSymbol*	CreateMarkerSymbol(const char* name) = 0;

		virtual LineSymbol*		CreateLineSymbol(augeLineType type) = 0;
		virtual LineSymbol*		CreateLineSymbol(const char* name) = 0;

		virtual RegionSymbol*	CreateRegionSymbol(augeRegionType type) = 0;	
		virtual RegionSymbol*	CreateRegionSymbol(const char* name) = 0;
	};

	extern "C"
	{
		AUGE_CARTO_API SymbolManager*	augeGetSymbolManagerInstance();

		void auge_make_symbol_icon_path(const char* icon_name, char* icon_path, size_t size);
	}

}

#endif //__AUGE_SYMBOL_H__
