#include "DemHillshadeRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string DemHillshadeRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string DemHillshadeRequest::DEFAULT_METHOD = "gps";

	DemHillshadeRequest::DemHillshadeRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_encoding(AUGE_DEFAULT_ENCODING),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{
		m_azimuth_rad= 0.0f;
		m_zenith_rad = 45.0f * AUGE_DEGREE_TO_RADIAN;
		m_zfactor = 1.0f;
	}

	DemHillshadeRequest::~DemHillshadeRequest()
	{
		
	}

	const char*	DemHillshadeRequest::GetEngine()
	{
		return "gps";
	}

	const char*	DemHillshadeRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DemHillshadeRequest::GetRequest()
	{
		return "DemHillshade";
	}

	void DemHillshadeRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DemHillshadeRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool DemHillshadeRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInputRaster(cgi["inputRasterName"]);
		SetInputDataSource(cgi["inputSourceName"]);
		SetInputPath(cgi["inputPath"]);

		SetOutputRaster(cgi["outputRasterName"]);
		SetOutputDataSource(cgi["outputSourceName"]);
		SetOutputPath(cgi["outputPath"]);

		SetAzimuth(cgi["Azimuth"]);
		SetZenith(cgi["Zenith"]);
		SetZFactor(cgi["zfactor"]);

		return true;
	}

/************************************************************************
<RasterExtractByRectange service="gps" version="1.0.0">
  <Input>
	<SourceName>test</SourceName>
	<RasterName>cities</RasterName>
  </Input>
  <Output>
	<SourceName>test</SourceName>
	<RasterName>cities_900913_post</RasterName>
	<SRID>900913</SRID>
  </Output>
</RasterExtractByRectange>
************************************************************************/
	bool DemHillshadeRequest::Create(XDocument* pxDoc)
	{
		XElement	*pxRoot = NULL;
		XAttribute	*pxAttr = NULL;

		m_pxDoc = pxDoc;

		pxRoot = pxDoc->GetRootNode();
		pxAttr = pxRoot->GetAttribute("version");
		if(pxAttr!=NULL)
		{
			SetVersion(pxAttr->GetValue());
		}

		XNodeSet* pxNodeSet = NULL;
		// Input 
		XNode* pxNodeInput = pxRoot->GetFirstChild("Input");
		if(pxNodeInput==NULL)
		{
			return false;
		}
		XNode* pxNodeSourceName = pxNodeInput->GetFirstChild("SourceName");
		if(pxNodeSourceName!=NULL)
		{
			SetInputDataSource(pxNodeSourceName->GetContent());
		}
		XNode* pxRasterName = pxNodeInput->GetFirstChild("RasterName");
		if(pxRasterName!=NULL)
		{
			SetInputRaster(pxRasterName->GetContent());
		}

		// output
		XNode* pxNodeOutput = pxRoot->GetFirstChild("Output");
		if(pxNodeOutput==NULL)
		{
			return false;
		}
		pxNodeSourceName = pxNodeOutput->GetFirstChild("SourceName");
		if(pxNodeSourceName!=NULL)
		{
			SetOutputDataSource(pxNodeSourceName->GetContent());
		}
		pxRasterName = pxNodeOutput->GetFirstChild("RasterName");
		if(pxRasterName!=NULL)
		{
			SetOutputRaster(pxRasterName->GetContent());
		}
		// rectangle
		/*XNode* pxNodeSrid = pxNodeOutput->GetFirstChild("SRID");
		if(pxNodeSrid!=NULL)
		{
		SetOutputSrid(pxNodeSrid->GetContent());
		}*/

		return true;
	}


	const char*	DemHillshadeRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DemHillshadeRequest::SetUser(const char* user)
	{
		if(user==NULL)
		{
			m_user.clear();
		}
		else
		{
			m_user = user;
		}
	}

	const char* DemHillshadeRequest::GetUser()
	{
		return m_user.c_str();
	}

	void DemHillshadeRequest::SetRequestMethod(const char* method)
	{
		if(method==NULL)
		{
			m_request_method = DEFAULT_METHOD;
		}
		else if(!strlen(method))
		{
			m_request_method = DEFAULT_METHOD;
		}
		else
		{
			m_request_method = method;
		}
	}

	void DemHillshadeRequest::SetHost(const char* host)
	{
		if(host==NULL)
		{
			m_host = DEFAULT_HOST;
		}
		else if(!strlen(host))
		{
			m_host = DEFAULT_HOST;
		}
		else
		{
			m_host = host;
		}
	}

	const char* DemHillshadeRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* DemHillshadeRequest::GetHost()
	{
		return m_host.c_str();
	}

	void DemHillshadeRequest::SetInputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name.clear();
		}
		else
		{
			m_in_source_name = sourceName;
		}
	}

	void DemHillshadeRequest::SetInputRaster(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_raster_name.clear();
		}
		else
		{
			m_in_raster_name = rasterName;
		}
	}

	void DemHillshadeRequest::SetInputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path = "/";
		}
		else
		{
			if(strlen(rasterPath)==0)
			{
				m_in_raster_path = "/";
			}
			else
			{
				m_in_raster_path = rasterPath;
			}
		}
	}

	void DemHillshadeRequest::SetOutputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_out_source_name.clear();
		}
		else
		{
			m_out_source_name = sourceName;
		}
	}

	void DemHillshadeRequest::SetOutputRaster(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_out_raster_name.clear();
		}
		else
		{
			m_out_raster_name = rasterName;
		}
	}

	void DemHillshadeRequest::SetOutputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_out_raster_path = "/";
		}
		else
		{
			if(strlen(rasterPath)==0)
			{
				m_out_raster_path = "/";
			}
			else
			{
				m_out_raster_path = rasterPath;
			}
		}
	}

	void DemHillshadeRequest::SetAzimuth(const char* azimuth)
	{
		if(azimuth!=NULL)
		{
			if(strlen(azimuth)>0)
			{
				m_azimuth_rad = atof(azimuth);
			}
		}
	}

	void DemHillshadeRequest::SetZenith(const char* zenith)
	{
		if(zenith!=NULL)
		{
			if(strlen(zenith)>0)
			{
				m_zenith_rad = atof(zenith);
			}
		}
	}

	void DemHillshadeRequest::SetZFactor(const char* zfactor)
	{
		if(zfactor!=NULL)
		{
			if(strlen(zfactor)>0)
			{
				m_zfactor = atof(zfactor);
			}
		}
	}

	const char*	DemHillshadeRequest::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	DemHillshadeRequest::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char* DemHillshadeRequest::GetInputPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char*	DemHillshadeRequest::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	DemHillshadeRequest::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char* DemHillshadeRequest::GetOutputPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	float DemHillshadeRequest::GetAzimuth()
	{
		return m_azimuth_rad;
	}

	float DemHillshadeRequest::GetZenith()
	{
		return m_zenith_rad;
	}

	float DemHillshadeRequest::GetZFactor()
	{
		return m_zfactor;
	}

	void DemHillshadeRequest::SetOutputFormat(const char* format)
	{
		if(format==NULL)
		{
			m_output_format = "GML2";
		}
		else
		{
			m_output_format = format;
		}
	}

	//const char* DemHillshadeRequest::GetOutputFormat()
	//{
	//	return m_output_format.c_str();
	//}
	
	void DemHillshadeRequest::Info()
	{
		GLogger* pLogger = augeGetLoggerInstance();
		char str[AUGE_MSG_MAX];
		const char* value = GetRequest();
		pLogger->Debug("[Requet Parameters]");
		g_sprintf(str,"\t%s:%s", "Request", (value==NULL)?"":value);
		pLogger->Debug(str);
		value = GetUser();
		g_sprintf(str,"\t%s:%s", "User", (value==NULL)?"":value);
		pLogger->Debug(str);
		g_sprintf(str,"\t%s:%s", "Service", "ims");
		pLogger->Debug(str);
		value = GetVersion();
		g_sprintf(str,"\t%s:%s", "Version", (value==NULL)?"":value);
		pLogger->Debug(str);

	}

}
