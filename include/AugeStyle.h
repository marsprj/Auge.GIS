#ifndef __AUGE_STYLE_H__
#define __AUGE_STYLE_H__

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
	//========================================================================
	// LineCap Enum
	//========================================================================
	typedef enum augeLineCap
	{
		augeButtCap		= 0,
		augeSquareCap	= 1,
		augeRoundCap	= 2,
		augeLineCapMAX	= 3
	}augeLineCap; 

	//========================================================================
	// augeLineJoin Enum
	//========================================================================
	typedef enum augeLineJoin
	{
		augeMiterJoin		= 0,
		augeMiterRevertJoin	= 1,
		augeRoundJoin		= 2,
		augeBevelJoin		= 3,
		augeLineJoinMax		= 4
	}augeLineJoin;

	//========================================================================
	// augeMarkType Enum
	//========================================================================
	typedef enum augeMarkType
	{
		augeMarkerSquare		= 0,
		augeMarkerCircle		= 1,
		augeMarkerTriangle		= 2,
		augeMarkerStar			= 3,
		augeMarkerCross			= 4,
		augeMarkerX				= 5,
		augeMarkerPentagon		= 6,
		augeMarkerCapital		= 7
	}augeMarkType;
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
	// augeFontStyle Enum
	//========================================================================
	typedef enum augeFontSlant
	{
		augeFontStyleNormal	= 0,
		augeFontStyleItalic	= 1,
		augeFontStyleOblique= 2,
	}augeFontSlant;

	//========================================================================
	// augeFontWeight Enum
	//========================================================================
	typedef enum augeFontWeight
	{
		augeFontWeightNormal= 0,
		augeFontWeightBold	= 1
	}augeFontWeight;

	typedef enum augeStyleType
	{
		augeStyleFeature	= 0,
		augeStyleRaster		= 1
	}augeStyleType;

	class GColor;
	class Rule;
	class Graphic;
	class Symbolizer;
	class PointSymbolizer;
	class LineSymbolizer;
	class PolygonSymbolizer;
	class TextSymbolizer;
	class ExternalGraphic;
	class Stroke;
	class Fill;
	class GraphicFill;
	class GraphicStroke;
	class Font;
	class Mark;
	class Filter;
	class StyleFactory;

	class Style : public GObject
	{
	protected:
		Style(){}
		virtual ~Style(){}
	public:
		virtual const char*		 GetName() = 0;
		virtual void			 SetName(const char* name) = 0;
		virtual augeStyleType	 GetType() = 0;		
	};

	class FeatureStyle : public Style
	{
	protected:
		FeatureStyle(){}
		virtual ~FeatureStyle(){}
	public:
		virtual int				GetRuleCount() = 0;
		virtual Rule*			GetRule(int i) = 0;
		virtual bool			AddRule(Rule* pRule) = 0;
		virtual augeGeometryType GetGeometryType() = 0;
		virtual void			SetGeometryType(augeGeometryType type) = 0;
	};

	class RasterStyle : public Style
	{
	protected:
		RasterStyle(){}
		virtual ~RasterStyle(){}
	};

	//========================================================================
	// Auge Rule
	//========================================================================
	//<xs:element name="Rule"> 
	//	<xs:complexType> 
	//		<xs:sequence> 
	//			<xs:element ref="sld:Name" minOccurs="0"/> 
	//			<xs:element ref="sld:Title" minOccurs="0"/> 
	//			<xs:element ref="sld:Abstract" minOccurs="0"/> 
	//			<xs:element ref="sld:LegendGraphic" minOccurs="0"/> 
	//			<xs:choice minOccurs="0"> 
	//				<xs:element ref="ogc:Filter"/> 
	//				<xs:element ref="sld:ElseFilter"/> 
	//			</xs:choice> 
	//			<xs:element ref="sld:MinScaleDenominator" minOccurs="0"/> 
	//			<xs:element ref="sld:MaxScaleDenominator" minOccurs="0"/> 
	//			<xs:choice minOccurs="0" maxOccurs="unbounded"> 
	//				<xs:element ref="sld:LineSymbolizer"/> 
	//				<xs:element ref="sld:PolygonSymbolizer"/> 
	//				<xs:element ref="sld:PointSymbolizer"/> 
	//				<xs:element ref="sld:TextSymbolizer"/> 
	//				<xs:element ref="sld:RasterSymbolizer"/> 
	//			</xs:choice> 
	//		</xs:sequence> 
	//	</xs:complexType> 
	//</xs:element> 
	//========================================================================
	class Rule : public GObject
	{
	protected:
		Rule(){}
		virtual ~Rule(){}
	public:
		virtual const char*		GetName() = 0;
		virtual void			SetName(const char* szName) = 0;

		virtual const char*		GetTitle() = 0;
		virtual void			SetTitle(const char* szTitle) = 0;

		virtual const char*		GetAbstract() = 0;
		virtual void			SetAbstract(const char* szAbstract) = 0;

		virtual Graphic*		GetLegendGraphic() = 0;
		virtual void			SetLegendGraphic(Graphic* pGraphic) = 0;

		virtual	Symbolizer*		GetSymbolizer() = 0;
		virtual void			SetSymbolizer(Symbolizer *pSymbolizer) = 0;

		virtual	TextSymbolizer*	GetTextSymbolizer() = 0;
		virtual void			SetTextSymbolizer(TextSymbolizer *pSymbolizer) = 0;

		virtual double			GetMinScale() = 0;
		virtual void			SetMinScale(double scale) = 0;

		virtual double			GetMaxScale() = 0;
		virtual void			SetMaxScale(double scale) = 0;

		virtual GFilter*		GetFilter() = 0;
		virtual void			SetFilter(GFilter* pFilter) = 0;
	};

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

		virtual void			SetMarkType(augeMarkType type) = 0;
		virtual augeMarkType	GetMarkType() = 0;

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
		virtual const char* GetGeometry() = 0;
		virtual Stroke*		GetStroke() = 0;

		virtual bool		SetGeometry(const char* prop_name) = 0;
		virtual bool		SetStroke(Stroke* pStroke) = 0;
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
		virtual Fill*		GetFill() = 0;
		virtual void		SetFill(Fill* pFill) = 0;

		virtual Stroke*		GetStroke() = 0;
		virtual void		SetStroke(Stroke* pStroke) = 0;

		virtual void		SetOpacity(float opacity) = 0;
		virtual float		GetOpacity() = 0;

		virtual float		GetDisplacement_X() = 0;
		virtual float		GetDisplacement_Y() = 0;
		virtual void		SetDisplacement(float x, float y) = 0;

		virtual const char*	GetGemetry() = 0;
		virtual void		SetGeometry(const char* prop_name) = 0;
	};

	//========================================================================
	// Auge TextSymbolizer
	//========================================================================
	//<xsd:element name="TextSymbolizer" type="se:TextSymbolizerType" substitutionGroup="se:Symbolizer"/> 
	//<xsd:complexType name="TextSymbolizerType"> 
	//	<xsd:complexContent> 
	//		<xsd:extension base="se:SymbolizerType"> 
	//			<xsd:sequence> 
	//				<xsd:element ref="se:Geometry" minOccurs="0"/> 
	//				<xsd:element ref="se:Label" minOccurs="0"/> 
	//				<xsd:element ref="se:Font" minOccurs="0"/> 
	//				<xsd:element ref="se:LabelPlacement" minOccurs="0"/> 
	//				<xsd:element ref="se:Halo" minOccurs="0"/> 
	//				<xsd:element ref="se:Fill" minOccurs="0"/> 
	//			</xsd:sequence> 
	//		</xsd:extension> 
	//	</xsd:complexContent> 
	//</xsd:complexType> 
	class TextSymbolizer : public Symbolizer
	{
	protected:
		TextSymbolizer(){}
		virtual ~TextSymbolizer(){}
	public:
		virtual Font*		GetFont() = 0;
		virtual bool		SetFont(Font* pFont) = 0;

		virtual Fill*		GetFill() = 0;
		virtual bool		SetFill(Fill* pFill) = 0;

		virtual Stroke*		GetStroke() = 0;
		virtual bool		SetStroke(Stroke* pStroke) = 0;

		virtual const char*	GetGeometry() = 0;
		virtual bool		SetGeometry(const char* prop_name) = 0;

		virtual const char*	GetLabel() = 0;
		virtual bool		SetLabel(const char* prop_name) = 0;

		virtual const char*	GetLabelText() = 0;
		virtual bool		SetLabelText(const char* text) = 0;
		
		// x --> [0.0, 0.5, 1.0]
		// y --> [0.0, 0.5, 1.0]
		// [top,  middle, bottom] --> [0.0, 0.5, 1.0]
		// [left, center, right ] --> [1.0, 0.5, 0.0]
		virtual double		GetAnchorX() = 0;
		virtual double		GetAnchorY() = 0;
		virtual void		SetAnchor(double x, double y) = 0;
		// 0 --> [middle, center]
		// 1 --> [top	, left]
		// 2 --> [top	, center]
		// 3 --> [top	, right]
		// 4 --> [middle, left]
		// 0 --> [middle, center]
		// 6 --> [middle, right]
		// 7 --> [bottom, left]
		// 8 --> [bottom, center]
		// 9 --> [bottom, right]
		//virtual g_uint		GetAnchor() = 0;
		//virtual void		SetAnchor(const char *h, const char* v) = 0;

		virtual double		GetDisplacementX() = 0;
		virtual double		GetDisplacementY() = 0;
		virtual void		SetDisplacement(double x, double y) = 0;

		virtual double		GetRotation() = 0;
		virtual void		SetRotation(double rotation) = 0;
		virtual const char*	GetRotationField() = 0;
		virtual void		SetRotationFiled(const char* prop_name) = 0;
	};

	//========================================================================
	// Auge RasterSymbolizer
	//========================================================================
	//<xsd:element	name="RasterSymbolizer" 
	//				type="se:RasterSymbolizerType" 
	//				bstitutionGroup="se:Symbolizer"/> 
	//	<xsd:complexType name="RasterSymbolizerType"> 
	//		<xsd:complexContent> 
	//			<xsd:extension base="se:SymbolizerType"> 
	//			<xsd:sequence> 
	//				<xsd:element ref="se:Geometry" minOccurs="0"/> 
	//				<xsd:element ref="se:Opacity" minOccurs="0"/> 
	//				<xsd:element ref="se:ChannelSelection" minOccurs="0"/> 
	//				<xsd:element ref="se:OverlapBehavior" minOccurs="0"/> 
	//				<xsd:element ref="se:ColorMap" minOccurs="0"/> 
	//				<xsd:element ref="se:ContrastEnhancement" minOccurs="0"/
	//				<xsd:element ref="se:ShadedRelief" minOccurs="0"/> 
	//				<xsd:element ref="se:ImageOutline" minOccurs="0"/> 
	//			</xsd:sequence> 
	//		</xsd:extension> 
	//	</xsd:complexContent> 
	//</xsd:complexType> 
	//========================================================================
	class RasterSymbolizer : public Symbolizer
	{
	protected:
		RasterSymbolizer(){}
		virtual ~RasterSymbolizer(){}
	public:
	};

	//========================================================================
	// Auge PointLabelPlacement
	//========================================================================
	//<xsd:element name="PointPlacement" type="se:PointPlacementType"/>
	//<xsd:complexType name="PointPlacementType"> 
	//	<xsd:sequence> 
	//		<xsd:element ref="se:AnchorPoint" minOccurs="0"/> 
	//		<xsd:element ref="se:Displacement" minOccurs="0"/> 
	//		<xsd:element ref="se:Rotation" minOccurs="0"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//========================================================================
	class PointLabelPlacement : public GObject
	{
	protected:
		PointLabelPlacement(){}
		virtual ~PointLabelPlacement(){}
	public:
	};

	//========================================================================
	// Auge LineLabelPlacement
	//========================================================================
	//<xsd:element name="LinePlacement" type="se:LinePlacementType"/> 
	//<xsd:complexType name="LinePlacementType"> 
	//	<xsd:sequence> 
	//		<xsd:element ref="se:PerpendicularOffset" minOccurs="0"/> 
	//		<xsd:element ref="se:IsRepeated" minOccurs="0"/> 
	//		<xsd:element ref="se:InitialGap" minOccurs="0"/> 
	//		<xsd:element ref="se:Gap" minOccurs="0"/> 
	//		<xsd:element ref="se:IsAligned" minOccurs="0"/> 
	//		<xsd:element ref="se:GeneralizeLine" minOccurs="0"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//<xsd:element name="IsRepeated" type="xsd:boolean"/> 
	//<xsd:element name="IsAligned" type="xsd:boolean"/> 
	//<xsd:element name="GeneralizeLine" type="xsd:boolean"/> 
	//========================================================================
	class LineLabelPlacement : public GObject
	{
	protected:
		LineLabelPlacement(){}
		virtual ~LineLabelPlacement(){}
	public:
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
	// Auge Mark
	//------------------------------------------------------------------------
	//<xsd:element name="Mark" type="se:MarkType"/> 
	//	<xsd:complexType name="MarkType"> 
	//		<xsd:sequence> 
	//			<xsd:choice minOccurs="0"> 
	//				<xsd:element ref="se:WellKnownName"/> 
	//				<xsd:sequence> 
	//					<xsd:choice> 
	//						<xsd:element ref="se:OnlineResource"/> 
	//						<xsd:element ref="se:InlineContent"/> 
	//					</xsd:choice> 
	//					<xsd:element ref="se:Format"/> 
	//					<xsd:element ref="se:MarkIndex" minOccurs="0"/
	//				</xsd:sequence> 
	//			</xsd:choice> 
	//			<xsd:element ref="se:Fill" minOccurs="0"/> 
	//			<xsd:element ref="se:Stroke" minOccurs="0"/> 
	//		</xsd:sequence> 
	//	</xsd:complexType> 
	//<xsd:element name="WellKnownName" type="xsd:string"/> 
	//========================================================================
	class Mark : public GObject
	{
	public:
		Mark(){}
		virtual ~Mark(){}
	public:
		virtual augeMarkType	GetType() = 0;
		virtual void			SetType(augeMarkType type) = 0;

		virtual Fill*			GetFill() = 0;
		virtual bool			SetFill(Fill *pFill) = 0;

		virtual Stroke*			GetStoke() = 0;
		virtual bool			SetStroke(Stroke *pStroke) = 0;
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
	// Auge Font
	//------------------------------------------------------------------------
	//<xsd:element name="Fill" type="se:FillType"/> 
	//<xsd:complexType name="FillType"> 
	//	<xsd:sequence> 
	//		<xsd:element ref="se:GraphicFill" minOccurs="0"/> 
	//		<xsd:element ref="se:SvgParameter" minOccurs="0" maxOccurs="unbounded"/> 
	//	</xsd:sequence> 
	//</xsd:complexType> 
	//========================================================================
	class Font : public GObject
	{
	protected:
		Font(){}
		virtual ~Font(){}
	public:
		virtual const char*		GetFamily() = 0;
		virtual bool			SetFamily(const char* szFamily) = 0;

		virtual augeFontSlant	GetStyle() = 0;
		virtual void			SetStyle(augeFontSlant style) = 0;

		virtual augeFontWeight	GetWeight() = 0;
		virtual void			SetWeight(augeFontWeight weight) = 0;

		virtual float			GetSize() = 0;
		virtual void			SetSize(float size) = 0;
	};

	class ColorMap : public GObject
	{
	protected:
		ColorMap(){}
		virtual ~ColorMap(){}
	public:
		virtual	g_uint			GetCount() = 0;
		virtual bool			SetCount(g_uint count) = 0;
		virtual	GColor*			GetColor(g_uint i) = 0;
		virtual GColor&			GetStartColor() = 0;
		virtual GColor&			GetEndColor() = 0;
		virtual const char*		GetImagePath() = 0;
		virtual const char*		GetAbsoluteImagePath() = 0;
		virtual void			Release() = 0;
	};

	class EnumColorMap : public GObject
	{
	protected:
		EnumColorMap(){}
		virtual ~EnumColorMap(){}
	public:
		virtual	void			Reset() = 0;
		virtual ColorMap*		Next() = 0;
		virtual void			Release() = 0;
	};

	class StyleReader : public GObject
	{
	protected:
		StyleReader(){}
		virtual ~StyleReader(){}
	public:
		virtual	Style*			Read(const char* path, FeatureClass* pFeatureClass) = 0;
		virtual	Style*			Read(const char* buffer, int length, FeatureClass* pFeatureClass) = 0;
	};

	class StyleWriter : public GObject
	{
	protected:
		StyleWriter(){}
		virtual ~StyleWriter(){}
	public:
		virtual XDocument*		Write(Style* pStyle, const char* version="1.0.0") = 0;
	};

	class StyleFactory
	{
	protected:
		StyleFactory(){}
		virtual ~StyleFactory(){}
	public:
		virtual	FeatureStyle*		CreateFeatureStyle() = 0;
		virtual FeatureStyle*		CreateFeatureStyle(augeSymbolizerType type) = 0;
		virtual FeatureStyle*		CreateFeatureStyle(augeGeometryType type) = 0;
		virtual RasterStyle*		CreateRasterStyle() = 0;

		virtual Rule*				CreateRule() = 0;
		virtual PointSymbolizer*	CreatePointSymbolizer() = 0;
		virtual LineSymbolizer*		CreateLineSymbolizer() = 0;
		virtual PolygonSymbolizer*	CreatePolygonSymbolizer() = 0;
		virtual TextSymbolizer*		CreateTextSymbolizer() = 0;

		virtual Stroke*				CreateStroke() = 0;
		virtual Fill*				CreateFill() = 0;
		virtual Mark*				CreateMark() = 0;
		virtual Font*				CreateFont() = 0;
		virtual ColorMap*			CreateColorMap(g_uint count, GColor& start, GColor& end) = 0;

		virtual StyleReader*		CreateStyleReader() = 0;
		virtual StyleWriter*		CreateStyleWriter() = 0;
	};

	extern "C"
	{
		AUGE_CARTO_API StyleFactory*	augeGetStyleFactoryInstance();
	}
}

#endif //__AUGE_STYLE_H__
