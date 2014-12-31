#ifndef __AUGE_WFS_GET_FEATUR_REQUEST_H__
#define __AUGE_WFS_GET_FEATUR_REQUEST_H__

#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeFilter.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>

namespace auge
{
	class GFilter;

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
		virtual GFilter*		GetFilter();

	public:
		bool		Create(GPropertySet& props);
		bool		Create(XElement* pxRequest);

		void		SetVersion(const char* value);
		void		SetTypeName(const char* typeName);
		void		SetFilter(const char* xml);
		void		SetFilter(XElement* pxFilter);
		void		SetMaxFeatures(const char* maxFeatures);
		void		SetOrderBy(XElement* pxOrderBy);

	private:
		std::string m_version;
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;
		g_int		m_max_features;
		GFilter		*m_pFilter;

		std::string m_orderby_field;
		augeOrderType	m_orderby_type;
		
	};
}

#endif //__AUGE_WFS_GET_FEATUR_REQUEST_H__
