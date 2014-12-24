#include "AugeXML.h"
#include "string.h"
#include <libxml/parserInternals.h>

namespace auge
{

	XParser::XParser()
	{

	}

	XParser::~XParser()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	XDocument* XParser::Parse(const char* szPath, bool keep_blank/*=false*/)
	{
		if(szPath==NULL)
			return NULL;

		xmlKeepBlanksDefault(keep_blank?1:0);
		xmlDoValidityCheckingDefaultValue =0;

		xmlParserCtxtPtr pxParseCtxt = NULL;	
		pxParseCtxt = xmlCreateFileParserCtxt(szPath);
		if(pxParseCtxt==NULL)
		{
			return NULL;
		}

		if(pxParseCtxt->directory == NULL)
		{
			char* directory = xmlParserGetDirectory(szPath);
			pxParseCtxt->directory = (char*)xmlStrdup((xmlChar*)directory);
		}

		if(!ParseContext(pxParseCtxt))
		{
			xmlFreeParserCtxt(pxParseCtxt);
			return NULL;
		}

		XDocument* pagXmlDoc = NULL;
		pagXmlDoc = new XDocument(pxParseCtxt->myDoc);

		xmlFreeParserCtxt(pxParseCtxt);

		return pagXmlDoc;
	}

	XDocument* XParser::ParseMemory(const char* content, bool keep_blank/*=false*/)
	{
		xmlParserCtxtPtr pxParseCtxt = NULL;

		xmlKeepBlanksDefault(keep_blank?1:0);
		xmlDoValidityCheckingDefaultValue =0;
		pxParseCtxt = xmlCreateMemoryParserCtxt((const char*)content, strlen(content));
		if(pxParseCtxt==NULL)
		{
			return NULL;
		}

		if(!ParseContext(pxParseCtxt))
		{
			xmlFreeParserCtxt(pxParseCtxt);
			return NULL;
		}

		XDocument* pagXmlDoc = NULL;
		pagXmlDoc = new XDocument(pxParseCtxt->myDoc);

		xmlFreeParserCtxt(pxParseCtxt);

		return pagXmlDoc;
	}

	bool XParser::ParseContext(xmlParserCtxtPtr pxParseCtxt)
	{
		/** initialize context begin **/
		pxParseCtxt->linenumbers = 1;
		//有效性检查
		//pxParseCtxt->validate = (validate_ ? 1 : 0);
		//初始化error和varning的回调函数
		//pxParseCtxt->vctxt.error = &callback_validity_error;
		//pxParseCtxt->vctxt.warning = &callback_validity_warning;
		//允许callback_validity_*回调函数回去C++指针
		pxParseCtxt->_private = this;
		//pxParseCtxt->replaceEntities = (substitute_entities_ ? 1 : 0);
		/** initialize context  end **/

		xmlParseDocument(pxParseCtxt);

		if(!pxParseCtxt->wellFormed)
		{
			return false;
		}

		if(pxParseCtxt->errNo != 0)
		{	
			return false;
		}
		return true;
	}

}
