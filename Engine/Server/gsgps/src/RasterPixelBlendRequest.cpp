#include "RasterPixelBlendRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string RasterPixelBlendRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string RasterPixelBlendRequest::DEFAULT_METHOD = "gps";

	RasterPixelBlendRequest::RasterPixelBlendRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_encoding(AUGE_DEFAULT_ENCODING),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{

	}

	RasterPixelBlendRequest::~RasterPixelBlendRequest()
	{
		
	}

	const char*	RasterPixelBlendRequest::GetEngine()
	{
		return "gps";
	}

	const char*	RasterPixelBlendRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RasterPixelBlendRequest::GetRequest()
	{
		return "RasterPixelBlend";
	}

	void RasterPixelBlendRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RasterPixelBlendRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool RasterPixelBlendRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInputRaster_1(cgi["inputRasterName_1"]);
		SetInputDataSource_1(cgi["inputSourceName_1"]);

		SetInputRaster_2(cgi["inputRasterName_2"]);
		SetInputDataSource_2(cgi["inputSourceName_2"]);

		SetOutputRaster(cgi["outputRasterName"]);
		SetOutputDataSource(cgi["outputSourceName"]);

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
	bool RasterPixelBlendRequest::Create(XDocument* pxDoc)
	{
		//XElement	*pxRoot = NULL;
		//XAttribute	*pxAttr = NULL;

		//m_pxDoc = pxDoc;

		//pxRoot = pxDoc->GetRootNode();
		//pxAttr = pxRoot->GetAttribute("version");
		//if(pxAttr!=NULL)
		//{
		//	SetVersion(pxAttr->GetValue());
		//}

		//XNodeSet* pxNodeSet = NULL;
		//// Input 
		//XNode* pxNodeInput = pxRoot->GetFirstChild("Input");
		//if(pxNodeInput==NULL)
		//{
		//	return false;
		//}
		//XNode* pxNodeSourceName = pxNodeInput->GetFirstChild("SourceName");
		//if(pxNodeSourceName!=NULL)
		//{
		//	SetInputDataSource(pxNodeSourceName->GetContent());
		//}
		//XNode* pxRasterName = pxNodeInput->GetFirstChild("RasterName");
		//if(pxRasterName!=NULL)
		//{
		//	SetInputRaster(pxRasterName->GetContent());
		//}

		//// output
		//XNode* pxNodeOutput = pxRoot->GetFirstChild("Output");
		//if(pxNodeOutput==NULL)
		//{
		//	return false;
		//}
		//pxNodeSourceName = pxNodeOutput->GetFirstChild("SourceName");
		//if(pxNodeSourceName!=NULL)
		//{
		//	SetOutputDataSource(pxNodeSourceName->GetContent());
		//}
		//pxRasterName = pxNodeOutput->GetFirstChild("RasterName");
		//if(pxRasterName!=NULL)
		//{
		//	SetOutputRaster(pxRasterName->GetContent());
		//}
		//// rectangle
		///*XNode* pxNodeSrid = pxNodeOutput->GetFirstChild("SRID");
		//if(pxNodeSrid!=NULL)
		//{
		//SetOutputSrid(pxNodeSrid->GetContent());
		//}*/

		return true;
	}


	const char*	RasterPixelBlendRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RasterPixelBlendRequest::SetUser(const char* user)
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

	const char* RasterPixelBlendRequest::GetUser()
	{
		return m_user.c_str();
	}

	void RasterPixelBlendRequest::SetRequestMethod(const char* method)
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

	void RasterPixelBlendRequest::SetHost(const char* host)
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

	const char* RasterPixelBlendRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* RasterPixelBlendRequest::GetHost()
	{
		return m_host.c_str();
	}

	void RasterPixelBlendRequest::SetInputDataSource_1(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name_1.clear();
		}
		else
		{
			m_in_source_name_1 = sourceName;
		}
	}

	void RasterPixelBlendRequest::SetInputRaster_1(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_raster_name_1.clear();
		}
		else
		{
			m_in_raster_name_1 = rasterName;
		}
	}


	void RasterPixelBlendRequest::SetInputPath_1(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path_1 = "/";
		}
		else
		{
			if(strlen(rasterPath)==0)
			{
				m_in_raster_path_1 = "/";
			}
			else
			{
				m_in_raster_path_1 = rasterPath;
			}
		}
	}


	void RasterPixelBlendRequest::SetInputDataSource_2(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name_2.clear();
		}
		else
		{
			m_in_source_name_2 = sourceName;
		}
	}

	void RasterPixelBlendRequest::SetInputRaster_2(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_raster_name_2.clear();
		}
		else
		{
			m_in_raster_name_2 = rasterName;
		}
	}

	void RasterPixelBlendRequest::SetInputPath_2(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path_2 = "/";
		}
		else
		{
			if(strlen(rasterPath)==0)
			{
				m_in_raster_path_2 = "/";
			}
			else
			{
				m_in_raster_path_2 = rasterPath;
			}
		}
	}

	void RasterPixelBlendRequest::SetOutputDataSource(const char* sourceName)
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

	void RasterPixelBlendRequest::SetOutputRaster(const char* rasterName)
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

	void RasterPixelBlendRequest::SetOutputPath(const char* rasterPath)
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
	
	const char*	RasterPixelBlendRequest::GetInputDataSource_1()
	{
		return m_in_source_name_1.empty() ? NULL : m_in_source_name_1.c_str();
	}

	const char*	RasterPixelBlendRequest::GetInputRaster_1()
	{
		return m_in_raster_name_1.empty() ? NULL : m_in_raster_name_1.c_str();
	}

	const char* RasterPixelBlendRequest::GetInputPath_1()
	{
		return m_in_raster_path_1.empty()? NULL : m_in_raster_path_1.c_str();
	}

	const char*	RasterPixelBlendRequest::GetInputDataSource_2()
	{
		return m_in_source_name_2.empty() ? NULL : m_in_source_name_2.c_str();
	}

	const char*	RasterPixelBlendRequest::GetInputRaster_2()
	{
		return m_in_raster_name_2.empty() ? NULL : m_in_raster_name_2.c_str();
	}

	const char* RasterPixelBlendRequest::GetInputPath_2()
	{
		return m_in_raster_path_2.empty()? NULL : m_in_raster_path_2.c_str();
	}

	const char*	RasterPixelBlendRequest::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterPixelBlendRequest::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char* RasterPixelBlendRequest::GetOutputPath()
	{
		return m_out_raster_path.empty()? NULL : m_out_raster_path.c_str();
	}

	void RasterPixelBlendRequest::SetOutputFormat(const char* format)
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

	//const char* RasterPixelBlendRequest::GetOutputFormat()
	//{
	//	return m_output_format.c_str();
	//}
	
	void RasterPixelBlendRequest::Info()
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
