#ifndef __AUGE_WFS_GET_FEATURE_REQUEST_H__
#define __AUGE_WFS_GET_FEATURE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>
#include "WFeatureRequest.h"

namespace auge
{
	#define AUGE_WFS_OUTPUT_FORMAT_WKT		"WKT"
	#define AUGE_WFS_OUTPUT_FORMAT_GML2		"GML2"

	class Map;
	class OrderBy;
	class GFilter;
	class GQuery;
	class FeatureClass;
	class XDocument;
	class XElement;

	class GetFeatureRequest : public WFeatureRequest
	{
	public:
		GetFeatureRequest();
		virtual ~GetFeatureRequest();

	public:
		//virtual const char*		GetEngine();
		//virtual const char*		GetVersion();
		virtual const char*		GetTypeName();
		virtual const char*		GetRequest();
		//virtual const char*		GetMimeType();
		
	public:

		//const char*	GetServiceName();
		const char* GetServiceURI();

		const char*	GetOutputFormat();
		g_int		GetMaxFeatures();
		g_int		GetOffset();
		GEnvelope&	GetBBox();
		GQuery*		GetQuery(FeatureClass* pFeatureClass);
		const char* GetEncoding();

		//void		SetVersion(const char* value);
		//void		SetServiceName(const char* name);
		//void		SetServiceURI(const char* uri);
		void		SetTypeName(const char* typeName,bool encoding);
		void		SetOutputFormat(const char* format);
		void		SetMaxFeatures(const char* maxFeatures);
		void		SetOffset(const char* offset);
		void		SetBBox(const char* bbox);
		void		SetEncoding(const char* encoding);
		//void		SetQuery(const char* filter, const char* fields, const char* typeName, Map* pMap);

		bool		Create(rude::CGI& cgi, Map* pMap);
		bool		Create(XDocument* pxDoc);

	public:
		void		Info();
		augeHttpMethodType GetMethod();

	private:
		void		ParseFieldName(const char* property_name, char* field_name, size_t size);
		void		SetFields(GQuery* pQuery, const char* fields);
		GQuery*		ParseXmlQuery(FeatureClass* pFeatureClass);
		GQuery*		ParseKvpQuery(FeatureClass* pFeatureClass);
		OrderBy*	ParserOrderBy(XElement* pxOrderBy);

	private: 
		//std::string m_version;
		//std::string m_service_name;
		//std::string m_service_uri;
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;
		std::string m_output_format;
		std::string m_bbox;
		std::string m_encoding;
		g_int		m_max_features;
		g_int		m_offset;
		GEnvelope	m_extent;
		const char*	m_filter;
		const char* m_fields;

		//GFilter		*m_pFilter;
		GQuery		*m_pQuery;

		XDocument	*m_pxDoc;
	};
}

#endif //__AUGE_WFS_GET_FEATURE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&mapName=world
http://127.0.0.1:8088/ows/user1/mgr?user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&mapName=world

service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&filter=<ogc:Filter xmlns:wfs="http://www.opengis.net/wfs" xmlns:ogc="http://www.opengis.net/ogc"><ogc:FeatureId fid="world.3"/></ogc:Filter>

http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities

service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&sourceName=db1
http://127.0.0.1:8088/ows/user1/mgr?service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&sourceName=db1

user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&bbox=0,0,10,10
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&bbox=0,0,10,10

user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&fields=gid,name,geom&orderby=

// output shapefile
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&outputformat=shape-zip
user=user1&sourcename=db1&service=wfs&version=1.0.0&request=GetFeature&typeName=cities&outputformat=shape-zip
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetFeature&typeName=world:cities&outputformat=shape-zip


//////////////////////////////////////////////////////////////////////////
// Standard WFS Request
//////////////////////////////////////////////////////////////////////////
[ HTTP Post 1.0.0]
<wfs:GetFeature service="WFS" version="1.0.0"
	outputFormat="GML2"
	mapName="world"
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
	mapName="world"
	xmlns:world="http://www.radi.ac.cn/world"
	xmlns:wfs="http://www.opengis.net/wfs"
	xmlns:ogc="http://www.opengis.net/ogc"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.opengis.net/wfs
	http://schemas.opengis.net/wfs/1.1.0/wfs.xsd">
	<wfs:Query typeName="world:cities">
		<wfs:PropertyName>world:gid</wfs:PropertyName>
		<wfs:PropertyName>world:name</wfs:PropertyName>
		<wfs:PropertyName>world:geom</wfs:PropertyName>
		<!--
		<ogc:Filter>
			<ogc:FeatureId fid="world.3"/>
		</ogc:Filter>
		-->
	</wfs:Query>
</wfs:GetFeature>

//////////////////////////////////////////////////////////////////////////
// Extended WFS Request
//////////////////////////////////////////////////////////////////////////
[ HTTP Post 1.0.0]
<wfs:GetFeature service="WFS" version="1.0.0"
	outputFormat="GML2"
	sourceName="world"
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

<wfs:GetFeature service="WFS" version="1.0.0"
	outputFormat="GML2"
	sourceName="world"
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
		<ogc:OrderBy order="asc">
			<wfs:PropertyName>world:gid</wfs:PropertyName>
			<wfs:PropertyName>world:name</wfs:PropertyName>
		</ogc:OrderBy>
	</wfs:Query>
</wfs:GetFeature>

[ HTTP Post 1.1.0]
<wfs:GetFeature service="WFS" version="1.1.0"
	sourceName="world"
	xmlns:world="http://www.radi.ac.cn/world"
	xmlns:wfs="http://www.opengis.net/wfs"
	xmlns:ogc="http://www.opengis.net/ogc"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.opengis.net/wfs
	http://schemas.opengis.net/wfs/1.1.0/wfs.xsd">
	<wfs:Query typeName="world:cities">
		<wfs:PropertyName>world:gid</wfs:PropertyName>
		<wfs:PropertyName>world:name</wfs:PropertyName>
		<wfs:PropertyName>world:geom</wfs:PropertyName>
		<!--
		<ogc:Filter>
			<ogc:FeatureId fid="world.3"/>
		</ogc:Filter>
		-->
	</wfs:Query>
</wfs:GetFeature>

************************************************************************/
