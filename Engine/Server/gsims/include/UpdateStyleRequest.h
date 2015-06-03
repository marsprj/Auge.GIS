#ifndef __AUGE_IMS_UPDATE_STYLE_REQUEST_H__
#define __AUGE_IMS_UPDATE_STYLE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class UpdateStyleRequest : public WebRequest
	{
	public:
		UpdateStyleRequest();
		virtual ~UpdateStyleRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();


	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetName(const char* name);
		const char*	GetName();

		void		SetStyle(const char* style);
		const char*	GetStyle();

		void		SetType(const char* type);
		const char*	GetType();


	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_name;
		std::string m_type;
		//std::string m_style;
		char*		m_style;
	};
}

#endif //__AUGE_IMS_UPDATE_STYLE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=UpdateStyle&name=point&styl=sld

************************************************************************/