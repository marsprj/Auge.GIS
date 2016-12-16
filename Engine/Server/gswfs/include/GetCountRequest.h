#ifndef __AUGE_WFS_GET_COUNT_REQUEST_H__
#define __AUGE_WFS_GET_COUNT_REQUEST_H__

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
	class Map;
	class GFilter;
	class GQuery;
	class FeatureClass;
	class XDocument;

	class GetCountRequest : public WFeatureRequest
	{
	public:
		GetCountRequest();
		virtual ~GetCountRequest();

	public:
		virtual const char*		GetTypeName();
		virtual const char*		GetRequest();
		
	public:
		const char*				GetServiceURI();
		GEnvelope&				GetBBox();
		GFilter*				GetFilter(FeatureClass* pFeatureClass);

		void					SetTypeName(const char* typeName,bool encoding=true);
		void					SetBBox(const char* bbox);
		
		bool					Create(rude::CGI& cgi, Map* pMap);
		bool					Create(XDocument* pxDoc);

	public:
		void					Info();
		augeHttpMethodType		GetMethod();
			
	private:
		GFilter*				ParseXmlFilter(FeatureClass* pFeatureClass);
		GFilter*				ParseKvpFilter(FeatureClass* pFeatureClass);

	private: 
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;
		std::string m_output_format;
		std::string m_bbox;
		GEnvelope	m_extent;
		const char*	m_filter;

		GFilter		*m_pFilter;
		XDocument	*m_pxDoc;
	};
}

#endif //__AUGE_WFS_GET_COUNT_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCount&typeName=world:cities&mapName=world

service=wfs&version=1.0.0&request=GetCount&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCount&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCount&typeName=world:cities&filter=<ogc:Filter xmlns:wfs="http://www.opengis.net/wfs" xmlns:ogc="http://www.opengis.net/ogc"><ogc:FeatureId fid="world.3"/></ogc:Filter>

http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetCount&typeName=world:cities

service=wfs&version=1.0.0&request=GetCount&typeName=world:cities&sourceName=db1
http://127.0.0.1:8088/ows/user1/mgr?service=wfs&version=1.0.0&request=GetCount&typeName=world:cities&sourceName=db1

user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCount&typeName=world:cities&bbox=0,0,10,10
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetCount&typeName=world:cities&bbox=0,0,10,10


//////////////////////////////////////////////////////////////////////////
// Standard WFS Request
//////////////////////////////////////////////////////////////////////////
[ HTTP Post 1.0.0]
<wfs:GetCount service="WFS" version="1.0.0"
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
</wfs:GetCount>

[ HTTP Post 1.1.0]
<wfs:GetCount service="WFS" version="1.1.0"
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
</wfs:GetCount>

//////////////////////////////////////////////////////////////////////////
// Extended WFS Request
//////////////////////////////////////////////////////////////////////////
[ HTTP Post 1.0.0]
<wfs:GetCount service="WFS" version="1.0.0"
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
</wfs:GetCount>

[ HTTP Post 1.1.0]
<wfs:GetCount service="WFS" version="1.1.0"
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
</wfs:GetCount>

************************************************************************/
