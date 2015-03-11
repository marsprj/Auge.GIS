#ifndef __AUGE_WFS_TRANSACTION_REQUEST_H__
#define __AUGE_WFS_TRANSACTION_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class XDocument;

	class TransactionRequest : public WebRequest
	{
	public:
		TransactionRequest();
		virtual ~TransactionRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetTypeName();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		void		SetVersion(const char* value);
		void		SetTypeName(const char* typeName);
		bool		Create(XDocument* pxDoc);
		XDocument*	GetXmlDoc();

	private: 
		std::string m_version;
		std::string m_type_name;
		std::string m_full_name;
		std::string m_mime_type;

		XDocument	*m_pxDoc;
	};
}

#endif //__AUGE_WFS_TRANSACTION_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wfs&version=1.0.0&request=Transaction&typeName=world:cities
user=user1&servicename=world&service=wfs&version=1.0.0&request=Transaction&typeName=world:cities
http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=Transaction&typeName=world:cities

[ HTTP Post 1.0.0]
<wfs:Transaction service="WFS" version="1.0.0" 
  xmlns:wfs="http://www.opengis.net/wfs" 
  xmlns:world="http://www.openplans.org/world" 
  xmlns:gml="http://www.opengis.net/gml" 
  xmlns:ogc="http://www.opengis.net/ogc" 
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
  xsi:schemaLocation="http://www.opengis.net/wfs http://schemas.opengis.net/wfs/1.0.0/WFS-transaction.xsd http://www.openplans.org/cities  http://localhost:8080/geoserver/wfs/DescribeFeaturename?name=world:cities">
  <wfs:Insert>
    <world:cities>
      <world:the_geom>
        <gml:Point srsName="http://www.opengis.net/gml/srs/epsg.xml#4326">
			<gml:coordinates>33.086040496826172,68.963546752929687</gml:coordinates>
		</gml:Point>
      </world:the_geom>
      <world:name>alley</world:name>
    </world:cities>
  </wfs:Insert>
   <wfs:Update name="world:cities">
    <wfs:Property>
      <wfs:Name>name</wfs:Name>
      <wfs:Value>xxxx</wfs:Value>
    </wfs:Property>
    <ogc:Filter>
      <ogc:FeatureId fid="cities.1"/>
    </ogc:Filter>
  </wfs:Update>
  <wfs:Delete name="world:cities">
    <ogc:Filter>
      <ogc:PropertyIsEqualTo>
        <ogc:PropertyName>gid</ogc:PropertyName>
        <ogc:Literal>607</ogc:Literal>
      </ogc:PropertyIsEqualTo>
    </ogc:Filter>
   </wfs:Delete>
</wfs:Transaction>


************************************************************************/
