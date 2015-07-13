#ifndef __AUGE_REGION_SYMBOL_H__
#define __AUGE_REGION_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
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
}

#endif //__AUGE_REGION_SYMBOL_H__
