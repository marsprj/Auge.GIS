#ifndef __AUGE_WFS_GET_FEATURE_REQUEST_H__
#define __AUGE_WFS_GET_FEATURE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class Map;
	class GFilter;
	class XDocument;

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

		const char*	GetOutputFormat();
		g_int		GetMaxFeatures();
		g_int		GetOffset();
		GEnvelope&	GetBBox();
		GFilter*	GetFilter();

		void		SetVersion(const char* value);
		void		SetTypeName(const char* typeName);
		void		SetOutputFormat(const char* format);
		void		SetMaxFeatures(const char* maxFeatures);
		void		SetOffset(const char* offset);
		void		SetBBox(const char* bbox);
		void		SetFilter(const char* filter, const char* typeName, Map* pMap);

		bool		Create(rude::CGI& cgi, Map* pMap);
		bool		Create(XDocument* pxDoc, Map* pMap);

	public:
		void		Info();

	private: 
		std::string m_version;
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;
		std::string m_output_format;
		std::string m_bbox;
		g_int		m_max_features;
		g_int		m_offset;
		GEnvelope	m_extent;

		GFilter		*m_pFilter;
	};
}

#endif //__AUGE_WFS_GET_FEATURE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&filter=<ogc:Filter xmlns:wfs="http://www.opengis.net/wfs" xmlns:ogc="http://www.opengis.net/ogc"><ogc:FeatureId fid="world.3"/></ogc:Filter>
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&bbox=0,0,10,10
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&bbox=0,0,10,10


[ HTTP Post 1.0.0]
<wfs:GetFeature service="WFS" version="1.0.0"
	outputFormat="GML2"
	xmlns:topp="http://www.openplans.org/topp"
	xmlns:wfs="http://www.opengis.net/wfs"
	xmlns:ogc="http://www.opengis.net/ogc"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.opengis.net/wfs
	http://schemas.opengis.net/wfs/1.0.0/WFS-basic.xsd">
	<wfs:Query typeName="topp:states">
		<ogc:Filter>
			<ogc:FeatureId fid="states.3"/>
		</ogc:Filter>
	</wfs:Query>
</wfs:GetFeature>

[ HTTP Post 1.1.0]
<wfs:GetFeature service="WFS" version="1.1.0"
	xmlns:topp="http://www.openplans.org/topp"
	xmlns:wfs="http://www.opengis.net/wfs"
	xmlns:ogc="http://www.opengis.net/ogc"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.opengis.net/wfs
	http://schemas.opengis.net/wfs/1.1.0/wfs.xsd">
	<wfs:Query typeName="world:cities">
		<ogc:Filter>
			<ogc:FeatureId fid="world.3"/>
		</ogc:Filter>
	</wfs:Query>
</wfs:GetFeature>

************************************************************************/
