#ifndef __AUGE_WEB_FEATURE_ENGINE_H__
#define __AUGE_WEB_FEATURE_ENGINE_H__

#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class FeatureClass;
	class GetMapRequest;
	class GetCapabilitiesRequest;
	class DescribeFeatureTypeRequest;
	class GetFeatureRequest;

	class WebFeatureEngine : public WebEngine
	{
	public:
		WebFeatureEngine();
		virtual ~WebFeatureEngine();

	public:
		virtual WebResponse*	Execute(WebRequest* pWebRequest);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap);

		virtual WebRequest*		ParseRequest(const char* url);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc);

	public:
		virtual const char*		GetID();
		virtual const char*		GetType();
		virtual const char*		GetDescription();

		virtual void*			GetHandler();
		virtual void			SetHandler(void* handler);

		virtual const char*		GetLibraryPath();

	protected:
		bool					Support(const char* request);
		WebResponse*			GetCapabilities(GetCapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap);
		WebResponse*			DescribeFeatureType(DescribeFeatureTypeRequest* pRequest, WebContext* pWebContext, Map* pMap);
		WebResponse*			GetFeature(GetFeatureRequest* pRequest, WebContext* pWebContext, Map* pMap);

		bool					WriteCapabilities(const char* version,WebContext* pWebContext, Map* pMap);
		bool					WriteCapabilities_1_1_0(WebContext* pWebContext, Map* pMap);

		bool					WriteDescribeFeatureType(const char* version,WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass);
		bool					WriteDescribeFeatureType_1_1_0(WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass);

		WebResponse*			GetFeature_1_1_0(GetFeatureRequest* pRequest, WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass);

		const char*				GetOgcFieldType(augeFieldType type);
		const char*				GetOgcGeometryType(augeGeometryType type);

	protected:
		void*		m_handler;
		std::string	m_requests;
	};
}

#endif //__AUGE_WEB_FEATURE_ENGINE_H__
