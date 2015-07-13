#ifndef __AUGE_SYMBOL_H__
#define __AUGE_SYMBOL_H__

#include "AugeObject.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeGeometry.h"
#include "AugeFilter.h"
#include "AugeFeature.h"

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
	class Filter;
	class StyleFactory;

	//========================================================================
	// augeSymbolType Enum
	//========================================================================
	typedef enum
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
}

#endif //__AUGE_SYMBOL_H__
