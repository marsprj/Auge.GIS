#ifndef __AUGE_WFS_GET_FEATUR_REQUEST_H__
#define __AUGE_WFS_GET_FEATUR_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>

namespace auge
{
	class GetFeatureRequest : public WebRequest
	{
	public:
		GetFeatureRequest();
		virtual ~GetFeatureRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetTypeName();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		void		SetVersion(const char* value);
		void		SetTypeName(const char* typeName);
		bool		Create(GPropertySet& props);

	private:
		std::string m_version;
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;
	};
}

#endif //__AUGE_WFS_GET_FEATUR_REQUEST_H__
