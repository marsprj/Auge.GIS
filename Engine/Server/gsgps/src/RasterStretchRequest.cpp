#include "RasterStretchRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string RasterStretchRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string RasterStretchRequest::DEFAULT_METHOD = "gps";

	RasterStretchRequest::RasterStretchRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_encoding(AUGE_DEFAULT_ENCODING),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{
		m_start_color.Set(0,0,0,255);
		m_start_color.Set(255,255,255,255);
	}

	RasterStretchRequest::~RasterStretchRequest()
	{
		
	}

	const char*	RasterStretchRequest::GetEngine()
	{
		return "gps";
	}

	const char*	RasterStretchRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RasterStretchRequest::GetRequest()
	{
		return "RasterStretch";
	}

	void RasterStretchRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RasterStretchRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool RasterStretchRequest::Create(rude::CGI& cgi)
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

		SetStartColor(cgi["startColor"]);
		SetEndColor(cgi["endColor"]);

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
	bool RasterStretchRequest::Create(XDocument* pxDoc)
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


	const char*	RasterStretchRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RasterStretchRequest::SetUser(const char* user)
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

	const char* RasterStretchRequest::GetUser()
	{
		return m_user.c_str();
	}

	void RasterStretchRequest::SetRequestMethod(const char* method)
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

	void RasterStretchRequest::SetHost(const char* host)
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

	const char* RasterStretchRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* RasterStretchRequest::GetHost()
	{
		return m_host.c_str();
	}

	void RasterStretchRequest::SetInputDataSource(const char* sourceName)
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

	void RasterStretchRequest::SetInputRaster(const char* rasterName)
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

	void RasterStretchRequest::SetInputPath(const char* rasterPath)
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


	void RasterStretchRequest::SetOutputDataSource(const char* sourceName)
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

	void RasterStretchRequest::SetOutputRaster(const char* rasterName)
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

	void RasterStretchRequest::SetOutputPath(const char* rasterPath)
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
	
	const char*	RasterStretchRequest::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterStretchRequest::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char* RasterStretchRequest::GetInputPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char*	RasterStretchRequest::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterStretchRequest::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char* RasterStretchRequest::GetOutputPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	void RasterStretchRequest::SetOutputFormat(const char* format)
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

	void RasterStretchRequest::SetStartColor(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		size_t size = strlen(value);
		if(!size)
		{
			return;
		}
		if(value[0]!='#')
		{
			return;
		}
		if(size!=7)
		{
			return;
		}
		int r=0,g=0,b=0;
		sscanf(value, "#%2x%2x%2x", &r,&g,&b);
		m_start_color.Set(r,g,b,255);
	}

	void RasterStretchRequest::SetEndColor(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		size_t size = strlen(value);
		if(!size)
		{
			return;
		}
		if(value[0]!='#')
		{
			return;
		}
		if(size!=7)
		{
			return;
		}
		int r=0,g=0,b=0;
		sscanf(value, "#%2x%2x%2x", &r,&g,&b);
		m_end_color.Set(r,g,b,255);
	}

	GColor&	RasterStretchRequest::GetStartColor()
	{
		return m_start_color;
	}

	GColor&	RasterStretchRequest::GetEndColor()
	{
		return m_end_color;
	}

	//const char* RasterStretchRequest::GetOutputFormat()
	//{
	//	return m_output_format.c_str();
	//}
	
	void RasterStretchRequest::Info()
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
