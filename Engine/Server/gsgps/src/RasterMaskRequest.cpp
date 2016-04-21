#include "RasterMaskRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string RasterMaskRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string RasterMaskRequest::DEFAULT_METHOD = "gps";

	RasterMaskRequest::RasterMaskRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_encoding(AUGE_DEFAULT_ENCODING),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{

	}

	RasterMaskRequest::~RasterMaskRequest()
	{
		
	}

	const char*	RasterMaskRequest::GetEngine()
	{
		return "gps";
	}

	const char*	RasterMaskRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RasterMaskRequest::GetRequest()
	{
		return "RasterMask";
	}

	void RasterMaskRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RasterMaskRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool RasterMaskRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInputRaster_1(cgi["inputRasterName_1"]);
		SetInputDataSource_1(cgi["inputSourceName_1"]);
		SetInputPath_1(cgi["inputPath_1"]);

		SetInputRaster_2(cgi["inputRasterName_2"]);
		SetInputDataSource_2(cgi["inputSourceName_2"]);
		SetInputPath_2(cgi["inputPath_2"]);

		SetOutputRaster(cgi["outputRasterName"]);
		SetOutputDataSource(cgi["outputSourceName"]);
		SetOutputPath(cgi["outputPath"]);

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
	bool RasterMaskRequest::Create(XDocument* pxDoc)
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


	const char*	RasterMaskRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RasterMaskRequest::SetUser(const char* user)
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

	const char* RasterMaskRequest::GetUser()
	{
		return m_user.c_str();
	}

	void RasterMaskRequest::SetRequestMethod(const char* method)
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

	void RasterMaskRequest::SetHost(const char* host)
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

	const char* RasterMaskRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* RasterMaskRequest::GetHost()
	{
		return m_host.c_str();
	}

	void RasterMaskRequest::SetInputDataSource_1(const char* sourceName)
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

	void RasterMaskRequest::SetInputRaster_1(const char* rasterName)
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


	void RasterMaskRequest::SetInputPath_1(const char* rasterPath)
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


	void RasterMaskRequest::SetInputDataSource_2(const char* sourceName)
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

	void RasterMaskRequest::SetInputRaster_2(const char* rasterName)
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

	void RasterMaskRequest::SetInputPath_2(const char* rasterPath)
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

	void RasterMaskRequest::SetOutputDataSource(const char* sourceName)
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

	void RasterMaskRequest::SetOutputRaster(const char* rasterName)
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

	void RasterMaskRequest::SetOutputPath(const char* rasterPath)
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
	
	const char*	RasterMaskRequest::GetInputDataSource_1()
	{
		return m_in_source_name_1.empty() ? NULL : m_in_source_name_1.c_str();
	}

	const char*	RasterMaskRequest::GetInputRaster_1()
	{
		return m_in_raster_name_1.empty() ? NULL : m_in_raster_name_1.c_str();
	}

	const char* RasterMaskRequest::GetInputPath_1()
	{
		return m_in_raster_path_1.empty()? NULL : m_in_raster_path_1.c_str();
	}

	const char*	RasterMaskRequest::GetInputDataSource_2()
	{
		return m_in_source_name_2.empty() ? NULL : m_in_source_name_2.c_str();
	}

	const char*	RasterMaskRequest::GetInputRaster_2()
	{
		return m_in_raster_name_2.empty() ? NULL : m_in_raster_name_2.c_str();
	}

	const char* RasterMaskRequest::GetInputPath_2()
	{
		return m_in_raster_path_2.empty()? NULL : m_in_raster_path_2.c_str();
	}

	const char*	RasterMaskRequest::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterMaskRequest::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char* RasterMaskRequest::GetOutputPath()
	{
		return m_out_raster_path.empty()? NULL : m_out_raster_path.c_str();
	}

	void RasterMaskRequest::SetOutputFormat(const char* format)
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

	//const char* RasterMaskRequest::GetOutputFormat()
	//{
	//	return m_output_format.c_str();
	//}
	
	void RasterMaskRequest::Info()
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
