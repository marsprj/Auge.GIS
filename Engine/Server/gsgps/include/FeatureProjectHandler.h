#ifndef __AUGE_GPS_FEATURE_PROJECT_HANDLER_H__
#define __AUGE_GPS_FEATURE_PROJECT_HANDLER_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class WebContext;
	class FeatureProjectRequest;
	class FeatureProjectResponse;
	class FeatureClass;
	class Job;

	class FeatureProjectHandler : public WebHandler
	{
	public:
		FeatureProjectHandler();
		virtual ~FeatureProjectHandler();
	public:
		virtual const char*		GetName();
		virtual const char*		GetDescription();
		virtual WebRequest*		ParseRequest(rude::CGI& cgi);
		virtual WebRequest*		ParseRequest(rude::CGI& cgi, const char* mapName);
		virtual WebRequest*		ParseRequest(XDocument* pxDoc, const char* mapName);

		virtual WebResponse*	Execute(WebRequest* pWebRequest, User* pUser);
		virtual WebResponse*	Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser);

	protected:
		void	Begin(User* pUser);
		void	End();

	private:
		Job*	m_pJob;
	};
}

#endif //__AUGE_GPS_FEATURE_PROJECT_HANDLER_H__
