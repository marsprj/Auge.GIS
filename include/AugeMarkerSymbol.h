#ifndef __AUGE_MARKER_SYMBOL_H__
#define __AUGE_MARKER_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
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

		virtual void			SetMarkType(augeMarkerType type) = 0;
		virtual augeMarkerType	GetMarkType() = 0;

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
}

#endif //__AUGE_MARKER_SYMBOL_H__
