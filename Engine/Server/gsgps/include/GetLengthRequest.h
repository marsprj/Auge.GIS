#ifndef __AUGE_GPS_GET_LENGTH_REQUEST_H__
#define __AUGE_GPS_GET_LENGTH_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GFilter;

	class GetLengthRequest : public WebRequest
	{
	public:
		GetLengthRequest();
		virtual ~GetLengthRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char* GetTypeName();
		const char*	GetSourceName();
		GFilter*	GetFilter();

		const char* GetOutputFormat();
		const char* GetOutSourceName();
		const char* SetOutTypeName();
				
	public:
		bool		Create(rude::CGI& cgi);
		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		
		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);
		void		SetOutputFormat(const char* format);

		void		SetInTypeName(const char* typeName);
		void		SetInSourceName(const char* sourceName);		

		void		SetOutSourceName(const char* sourceName);
		void		SetOutTypeName(const char* typeName);

		void		SetFilter(XElement* pxFilter);
		
		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;

		std::string m_in_source_name;
		std::string m_in_type_name;

		std::string m_out_source_name;
		std::string m_out_type_name;

		std::string m_output_format;
		
		GFilter*	m_pFilter;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_GET_LENGTH_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
user=user1&service=gps&version=1.0.0&request=GetLength&sourceName=db1&typeName=country
http://127.0.0.1:8088/ows/user1/mgr?service=gps&version=1.0.0&request=GetLength&sourceName=db1&typeName=country
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=GetLength&sourceName=db1&typeName=country

service=gps&version=1.0.0&request=GetLength&sourceName=db1&typeName=country&distancefeild=id
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=GetLength&sourceName=db1&typeName=country&distancefeild=id
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=GetLength&sourceName=db1&typeName=country&distancefeild=id

2)
service=gps&version=1.0.0&request=GetLength&featureset=
	<FeatureCollection>
		<FeatureMember>
		</FeatureMember>
	</FeatureCollection>


[ HTTP Post ]
-------------------------------------------------------------------------
1)
<GetLength service="gps" version="1.0.0"
		  typeName="country"
		  outputFormat="GML2"
		  distance="1.0"
		  distancefield="id"
		  xmlns:ogc="http://www.opengis.net/ogc">
<GetLength>

2)
<GetLength service="gps" version="1.0.0"
		  typeName="country"
		  outputFormat="GML2"
		  distance="1.0"
		  xmlns:ogc="http://www.opengis.net/ogc">
	<Filter>
		<ogc:FeatureId fid="world.3"/>
	</Filter>
<GetLength>

3)
<GetLength service="gps" version="1.0.0"
		  typeName="country"
		  outputFormat="GML2"
		  xmlns:ogc="http://www.opengis.net/ogc">
	<Filter>
		<ogc:BBOX>
			<ogc:PropertyName>shape</ogc:PropertyName>
			<gml:Envelope xmlns:gml="http://www.opengis.net/gml">
				<gml:lowerCorner>0.0 90.0</gml:lowerCorner>
				<gml:upperCorner>0.0 180.0</gml:upperCorner>
			</gml:Envelope>
		</ogc:BBOX>
	</Filter>
<GetLength>
************************************************************************/
