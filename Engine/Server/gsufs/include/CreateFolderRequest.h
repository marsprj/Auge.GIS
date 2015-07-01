#ifndef __AUGE_UFS_CREATE_FOLDER_REQUEST_H__
#define __AUGE_UFS_CREATE_FOLDER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class CreateFolderRequest : public WebRequest
	{
	public:
		CreateFolderRequest();
		virtual ~CreateFolderRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char*	GetPath();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetPath(const char* path);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_path;
	};
}

#endif //__AUGE_UFS_CREATE_FOLDER_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=ufs&version=1.0.0&request=CreateFolder&Path=/bbb
http://127.0.0.1:8088/ows/user1/mgr?service=ufs&version=1.0.0&request=CreateFolder&Path=/ccc

************************************************************************/