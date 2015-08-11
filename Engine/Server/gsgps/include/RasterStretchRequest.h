#ifndef __AUGE_GPS_RASTER_STRETCH_REQUEST_H__
#define __AUGE_GPS_RASTER_STRETCH_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class RasterStretchRequest : public WebRequest
	{
	public:
		RasterStretchRequest();
		virtual ~RasterStretchRequest();

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
		const char* GetInputPath();

		const char*	GetOutputDataSource();
		const char*	GetOutputRaster();
		const char* GetOutputPath();
		GEnvelope&	GetRectangle();

		GColor&		GetStartColor();
		GColor&		GetEndColor();
				
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
		void		SetInputPath(const char* rasterPath);

		void		SetOutputDataSource(const char* sourceName);
		void		SetOutputRaster(const char* rasterName);
		void		SetOutputPath(const char* rasterPath);

		void		SetStartColor(const char* value);
		void		SetEndColor(const char* value);
		
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
		std::string m_in_raster_path;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		GColor		m_start_color;
		GColor		m_end_color;
		//////////////////////////////////////////////////////////////////////////

		XDocument	*m_pxDoc;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_RASTER_STRETCH_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
user=user1&service=gps&version=1.0.0&request=RasterSubtract&inputSourceName=rsdb2&inputRasterName=srtm_58_05.tif&outputSourceName=rsdb2&outputRasterName=srtm_58_05_stretch.jpg&outputPath=/
http://182.92.114.80:8088/ows/user1/mgr?service=gps&version=1.0.0&request=RasterSubtract&inputSourceName=rsdb2&inputRasterName=srtm_58_05.tif&outputSourceName=rsdb2&outputRasterName=srtm_58_05_stretch.jpg&outputPath=/

http://182.92.114.80:8088/ows/user1/mgr?service=gps&version=1.0.0&request=RasterSubtract&inputSourceName=rsdb2&inputRasterName=srtm_58_05.tif&outputSourceName=rsdb2&outputRasterName=srtm_58_05_stretch.jpg&outputPath=/&startColor=#000000&endColor=#ffffff

************************************************************************/