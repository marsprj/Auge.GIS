#ifndef __AUGE_WPS_DESCRIBE_PROCESS_HANDLER_H__
#define __AUGE_WPS_DESCRIBE_PROCESS_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class DescribeProcessRequest;
	class DescribeProcessResponse;
	class XElement;

	class DescribeProcessHandler : public WebHandler
	{
	public:
		DescribeProcessHandler();
		virtual ~DescribeProcessHandler();
	public:
		virtual const char*		GetName();

		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext);

	private:
		DescribeProcessResponse*	WriteDescribeProcess_1_0_0(DescribeProcessRequest* pRequest, WebContext* pWebContext);
	//	DescribeProcessResponse*	WriteDescribeProcess_1_3_0(DescribeProcessRequest* pRequest, WebContext* pWebContext);

	//	//virtual WebRequest*	ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
	//	//virtual WebRequest*	ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);
	//	//virtual WebResponse*	Execute(WebRequest* pWebRequest);
	//	//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

	private:
		void					SetRooteNode_1_1_0(XElement* pxRoot, const char* version);
		void					AddProcessDescriptionNode(XElement* pxParent, const char* identifier);
		void					AddDataInputsNode(XElement* pxDataInputes, const char* identifier);
		void					AddProcessOutputsNode(XElement* pxParent);

	protected:
		void*	m_handler;
		std::vector<WebHandler*>	m_handlers;
	};
}

#endif //__AUGE_WPS_DESCRIBE_PROCESS_HANDLER_H__
