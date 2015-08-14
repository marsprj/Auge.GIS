#ifndef __AUGE_IMS_GET_SYMBOL_ICON_RESPONSE_H__
#define __AUGE_IMS_GET_SYMBOL_ICON_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Symbol;
	class EnumSymbol;
	class WebContext;

	class GetSymbolIconResponse : public WebResponse
	{
	public:
		GetSymbolIconResponse(GetSymbolIconRequest* pRequest);
		virtual ~GetSymbolIconResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetSymbol(Symbol* pSymbol);
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	WriteSymbol(XDocument* pxDoc);
		void	AddSymbolNode(XElement* pxParent, Symbol* pSymbol);

	private:		
		GetSymbolIconRequest *m_pRequest;	
		WebContext		 *m_pWebContext;

		Symbol		*m_pSymbol;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__

