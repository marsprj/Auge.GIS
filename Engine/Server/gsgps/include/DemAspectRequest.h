#ifndef __AUGE_GPS_DEM_ASPECT_REQUEST_H__
#define __AUGE_GPS_DEM_ASPECT_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class DemAspectRequest : public WebRequest
	{
	public:
		DemAspectRequest();
		virtual ~DemAspectRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char*	GetInputDataSource();
		const char*	GetInputRaster();

		const char*	GetOutputDataSource();
		const char*	GetOutputRaster();
		GEnvelope&	GetRectangle();
				
	public:
		bool		Create(rude::CGI& cgi);
		bool		Create(XDocument* pxDoc);

		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		
		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);
		void		SetOutputFormat(const char* format);

		//////////////////////////////////////////////////////////////////////////
		void		SetInputDataSource(const char* sourceName);
		void		SetInputRaster(const char* rasterName);

		void		SetOutputDataSource(const char* sourceName);
		void		SetOutputRaster(const char* rasterName);
		
		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;
		std::string m_output_format;

		//////////////////////////////////////////////////////////////////////////
		std::string	m_in_source_name;
		std::string	m_in_raster_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		//////////////////////////////////////////////////////////////////////////

		XDocument	*m_pxDoc;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_DEM_ASPECT_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=DemAspect&inputSourceName=rsdb&inputRasterName=Koala.png&outputSourceName=rsdb&outputRasterName=Koala_900913.png&extent=100,100,500,500
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=DemAspect&inputSourceName=rsdb&inputRasterName=cities&outputSourceName=rsdb&outputRasterName=cities_900913&outputSrid=900913
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=DemAspect&inputSourceName=rsdb&inputRasterName=cities&outputSourceName=rsdb&outputRasterName=cities_900913&outputSrid=900913

service=gps&version=1.0.0&request=DemAspect&inputSourceName=rsdb&inputRasterName=rivers&outputSourceName=rsdb&outputRasterName=rivers_900913&outputSrid=900913
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=DemAspect&inputSourceName=rsdb&inputRasterName=rivers&outputSourceName=rsdb&outputRasterName=rivers_900913&outputSrid=900913

service=gps&version=1.0.0&request=DemAspect&inputSourceName=rsdb&inputRasterName=country&outputSourceName=rsdb&outputRasterName=country_900913&outputSrid=900913
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=DemAspect&inputSourceName=rsdb&inputRasterName=country&outputSourceName=rsdb&outputRasterName=country_900913&outputSrid=900913

//service=gps&version=1.0.0&request=DemAspect&sourceName=db1&RasterName=cities&distancefeild=id
//http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=DemAspect&sourceName=db1&RasterName=cities&distancefeild=id
//http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=DemAspect&sourceName=db1&RasterName=cities&distancefeild=id

2)
service=gps&version=1.0.0&request=DemAspect&featureset=
	<FeatureCollection>
		<FeatureMember>
		</FeatureMember>
	</FeatureCollection>


[ HTTP Post ]
-------------------------------------------------------------------------
1)
<DemAspect service="gps" version="1.0.0">
  <Input>
	<SourceName>rsdb</SourceName>
	<RasterName>cities</RasterName>
  </Input>
  <Output>
	<SourceName>rsdb</SourceName>
	<RasterName>cities_900913_post</RasterName>
	<SRID>900913</SRID>
  </Output>
</DemAspect>
************************************************************************/