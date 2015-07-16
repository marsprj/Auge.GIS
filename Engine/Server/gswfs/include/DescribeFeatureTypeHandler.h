#ifndef __AUGE_WFS_DESCRIBE_FEAUTURE_TYPE_HANDLER_H__
#define __AUGE_WFS_DESCRIBE_FEAUTURE_TYPE_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class Map;
	class WebContext;
	class DescribeFeatureTypeRequest;
	class DescribeFeatureTypeResponse;
	class FeatureClass;

	class DescribeFeatureTypeHandler : public WebHandler
	{
	public:
		DescribeFeatureTypeHandler();
		virtual ~DescribeFeatureTypeHandler();
	public:
		virtual const char*		GetName();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		//virtual WebRequest*		ParseRequest(rude::CGI& cgi, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);
		//virtual WebRequest*		ParseRequest(XDocument* pxDoc, WebContext* pWebContext=NULL, Map* pMap=NULL);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		//virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

	private:
		bool					WriteDescribeFeatureType(const char* version,WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass);
		bool					WriteDescribeFeatureType_1_0_0(WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass);
		bool					WriteDescribeFeatureType_1_1_0(WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass);

		FeatureClass*			GetFeatureClass(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassByMap(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);
		FeatureClass*			GetFeatureClassBySource(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

		const char*				GetOgcFieldType(augeFieldType type);
		const char*				GetOgcGeometryType(augeGeometryType type);
	};
}

#endif //__AUGE_WFS_DESCRIBE_FEAUTURE_TYPE_HANDLER_H__

