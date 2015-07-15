#ifndef __AUGE_IMS_GET_SYMBOL_RESPONSE_H__
#define __AUGE_IMS_GET_SYMBOL_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Symbol;
	class EnumSymbol;
	class WebContext;

	class GetSymbolResponse : public WebResponse
	{
	public:
		GetSymbolResponse(GetSymbolRequest* pRequest);
		virtual ~GetSymbolResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetSymbol(Symbol* pSymbol);
		void	SetSymbols(EnumSymbol* pSymbols);
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	WriteSymbol(XDocument* pxDoc);
		void	WriteSymbols(XDocument* pxDoc);
		void	AddSymbolNode(XElement* pxParent, Symbol* pSymbol);

	private:		
		GetSymbolRequest *m_pRequest;	
		WebContext		 *m_pWebContext;

		Symbol		*m_pSymbol;
		EnumSymbol	*m_pSymbols;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

