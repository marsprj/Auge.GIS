#ifndef __AUGE_LINE_SYMBOL_H__
#define __AUGE_LINE_SYMBOL_H__

#include "AugeSymbol.h"

namespace auge
{
	//========================================================================
	// augeLineType Enum
	//========================================================================
	typedef enum
	{
		augeLineSimple			= 0,
		augeLineRailway			= 1
	}augeLineType;


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

		virtual void		SetLineType(augeLineType type) = 0;
		virtual augeLineType GetLineType() = 0;
	};
}

#endif //__AUGE_LINE_SYMBOL_H__
