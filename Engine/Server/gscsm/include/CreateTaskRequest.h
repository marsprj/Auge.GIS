#ifndef __AUGE_CSM_CREATE_TASK_REQUEST_H__
#define __AUGE_CSM_CREATE_TASK_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class CreateTaskRequest : public WebRequest
	{
	public:
		CreateTaskRequest();
		virtual ~CreateTaskRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetName();

	public:
		const char*	GetMapName();
		const char* GetDescription();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetName(const char* name);
		void		SetMapName(const char* mapName);
		void		SetDescription(const char* description);
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_name;
		std::string m_mapName;
		std::string m_description;
		std::string m_encoding;
	};
}

#endif //__AUGE_CSM_CREATE_TASK_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=csm&version=1.0.0&request=CreateTask&name=world&mapName=1117&description=new task

http://127.0.0.1:8088/ows/user1/mgr?service=csm&version=1.0.0&request=CreateTask&name=world&mapName=world&description=new task

************************************************************************/