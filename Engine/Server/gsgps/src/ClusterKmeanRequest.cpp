#include "ClusterKmeanRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string KMeanRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string KMeanRequest::DEFAULT_METHOD = "gps";

	KMeanRequest::KMeanRequest():
	m_version("1.0.0"),
	m_clusters(-1),
	m_mime_type("text/xml"),
	m_encoding(AUGE_DEFAULT_ENCODING),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{

	}

	KMeanRequest::~KMeanRequest()
	{
		
	}

	const char*	KMeanRequest::GetEngine()
	{
		return "gps";
	}

	const char*	KMeanRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	KMeanRequest::GetRequest()
	{
		return "KMean";
	}

	void KMeanRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	KMeanRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool KMeanRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInputFeatureClass(cgi["inputTypeName"]);
		SetInputDataSource(cgi["inputSourceName"]);

		SetOutputFeatureClass(cgi["outputTypeName"]);
		SetOutputDataSource(cgi["outputSourceName"]);
		SetClusters(cgi["clusters"]);

		return true;
	}

	bool KMeanRequest::Create(XDocument* pxDoc)
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
		//XNode* pxTypeName = pxNodeInput->GetFirstChild("TypeName");
		//if(pxTypeName!=NULL)
		//{
		//	SetInputType(pxTypeName->GetContent());
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
		//pxTypeName = pxNodeOutput->GetFirstChild("TypeName");
		//if(pxTypeName!=NULL)
		//{
		//	SetOutputType(pxTypeName->GetContent());
		//}
		//// rectangle
		///*XNode* pxNodeSrid = pxNodeOutput->GetFirstChild("SRID");
		//if(pxNodeSrid!=NULL)
		//{
		//SetOutputSrid(pxNodeSrid->GetContent());
		//}*/

		return true;
	}


	const char*	KMeanRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void KMeanRequest::SetUser(const char* user)
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

	const char* KMeanRequest::GetUser()
	{
		return m_user.c_str();
	}

	void KMeanRequest::SetRequestMethod(const char* method)
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

	void KMeanRequest::SetHost(const char* host)
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

	const char* KMeanRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* KMeanRequest::GetHost()
	{
		return m_host.c_str();
	}

	void KMeanRequest::SetClusters(const char* clusters)
	{
		m_clusters = clusters==NULL ? -1 : atoi(clusters);
	}

	void KMeanRequest::SetInputDataSource(const char* sourceName)
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

	void KMeanRequest::SetInputFeatureClass(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_class_name.clear();
		}
		else
		{
			m_in_class_name = rasterName;
		}
	}

	void KMeanRequest::SetOutputDataSource(const char* sourceName)
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

	void KMeanRequest::SetOutputFeatureClass(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_out_class_name.clear();
		}
		else
		{
			m_out_class_name = rasterName;
		}
	}

	g_int KMeanRequest::GetClusters()
	{
		return m_clusters;
	}

	const char*	KMeanRequest::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	KMeanRequest::GetInputFeatureClass()
	{
		return m_in_class_name.empty() ? NULL : m_in_class_name.c_str();
	}

	const char*	KMeanRequest::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	KMeanRequest::GetOutputFeatureClass()
	{
		return m_out_class_name.empty() ? NULL : m_out_class_name.c_str();
	}

	void KMeanRequest::SetOutputFormat(const char* format)
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

	//const char* KMeanRequest::GetOutputFormat()
	//{
	//	return m_output_format.c_str();
	//}
	
	void KMeanRequest::Info()
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
