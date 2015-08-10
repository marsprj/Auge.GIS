#ifndef __AUGE_GPS_RASTER_SUBTRACT_REQUEST_H__
#define __AUGE_GPS_RASTER_SUBTRACT_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class RasterSubtractRequest : public WebRequest
	{
	public:
		RasterSubtractRequest();
		virtual ~RasterSubtractRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char*	GetInputDataSource_1();
		const char*	GetInputRaster_1();
		const char* GetInputPath_1();
		const char*	GetInputDataSource_2();
		const char*	GetInputRaster_2();
		const char* GetInputPath_2();

		const char*	GetOutputDataSource();
		const char*	GetOutputRaster();
		const char* GetOutputPath();
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
		void		SetInputDataSource_1(const char* sourceName);
		void		SetInputRaster_1(const char* rasterName);
		void		SetInputPath_1(const char* rasterPath);
		void		SetInputDataSource_2(const char* sourceName);
		void		SetInputRaster_2(const char* rasterName);
		void		SetInputPath_2(const char* rasterPath);

		void		SetOutputDataSource(const char* sourceName);
		void		SetOutputRaster(const char* rasterName);
		void		SetOutputPath(const char* rasterPath);
		
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
		std::string	m_in_source_name_1;
		std::string	m_in_raster_name_1;
		std::string m_in_raster_path_1;

		std::string	m_in_source_name_2;
		std::string	m_in_raster_name_2;
		std::string m_in_raster_path_2;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;
		//////////////////////////////////////////////////////////////////////////

		XDocument	*m_pxDoc;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_RASTER_SUBTRACT_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=RasterSubtract&inputSourceName_1=rsdb2&inputRasterName_1=Koala.jpg&inputSourceName_2=rsdb2&inputRasterName_2=Koala_smooth.jpg&outputSourceName=rsdb2&outputRasterName=Koala_sub.png
http://182.92.114.80:8088/ows/user1/mgr?service=gps&version=1.0.0&request=RasterSubtract&inputSourceName_1=rsdb2&inputRasterName_1=Koala.jpg&inputSourceName_2=rsdb2&inputRasterName_2=Koala_smooth.jpg&outputSourceName=rsdb2&outputRasterName=Koala_sub.png

************************************************************************/