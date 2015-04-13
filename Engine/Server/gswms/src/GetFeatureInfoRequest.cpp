#include "GetFeatureInfoRequest.h"

namespace auge
{
	GetFeatureInfoRequest::GetFeatureInfoRequest()
	{
		m_version = "1.3.0";
		m_width = 800;
		m_height = 600;
		m_bgColor.Set(255,255,255,0);
		m_transparent = true;
		m_mime_type = "image/png";
	}

	GetFeatureInfoRequest::~GetFeatureInfoRequest()
	{

	}

	const char*	GetFeatureInfoRequest::GetEngine()
	{
		return "wms";
	}

	const char*	GetFeatureInfoRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetFeatureInfoRequest::GetRequest()
	{
		return "GetFeatureInfo";
	}

	GEnvelope& GetFeatureInfoRequest::GetExtent()
	{
		return m_extent;
	}

	g_int GetFeatureInfoRequest::GetWidth()
	{
		return m_width;
	}

	g_int GetFeatureInfoRequest::GetHeight()
	{
		return m_height;
	}

	GColor&	GetFeatureInfoRequest::GetBgColor()
	{
		return m_bgColor;
	}

	bool GetFeatureInfoRequest::IsTransparent()
	{
		return m_transparent;
	}

	const char* GetFeatureInfoRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	g_uint GetFeatureInfoRequest::GetLayerCount()
	{
		return m_layers.size();
	}

	const char*	GetFeatureInfoRequest::GetLayer(g_uint i)
	{
		if(i>=m_layers.size())
		{
			return NULL;
		}
		return m_layers[i].c_str();
	}

	const char* GetFeatureInfoRequest::GetStyle(g_uint i)
	{
		if(i>=m_styles.size())
		{
			return NULL;
		}
		return m_styles[i].c_str();
	}

	bool GetFeatureInfoRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetLayers(cgi["layers"]);
		SetStyles(cgi["styles"]);
		SetBBox(cgi["bbox"]);
		SetWidth(cgi["width"]);
		SetHeight(cgi["height"]);
		SetBgColor(cgi["bgColor"]);
		SetTransparent(cgi["transparent"]);
		return true;
	}

	void GetFeatureInfoRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void GetFeatureInfoRequest::SetLayers(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		if(strlen(value)==0)
		{
			m_layers.resize(0);
			return;
		}

		char* str = strdup(value);
		char* ptr = strtok(str, ",");
		while(ptr!=NULL)
		{
			m_layers.push_back(ptr);

			ptr = strtok(NULL, ",");
		}
		free(str);
		m_styles.resize(m_layers.size());
	}

	void GetFeatureInfoRequest::SetStyles(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		if(strlen(value)==0)
		{
			//m_styles.resize(0);
			return;
		}

		char* str = strdup(value);
		int i = 0;
		int count = m_styles.size();
		char* ps = (char*)str;
		char* pe = NULL;

		while((pe = strstr(ps,","))!=NULL)
		{
			if(i>=count)
			{
				break;
			}
			if(!(pe-ps))
			{
				m_styles[i++] = "";
			}
			else
			{
				*pe = '\0';
				m_styles[i++] = ps;
			}			
			ps = pe+1;
		}
		if(strlen(ps)>0)
		{
			if(i<count)
			{
				m_styles[i++] = ps;
			}
		}
		free(str);
	}

	//void GetFeatureInfoRequest::SetStyles(const char* value)
	//{
	//	if(value==NULL)
	//	{
	//		return;
	//	}
	//	if(strlen(value)==0)
	//	{
	//		m_styles.resize(0);
	//		return;
	//	}

	//	int i=0;
	//	//char* str = strdup(value);
	//	char* str = strdup(",,,country_red");
	//	char* ptr = strtok(str, ",");
	//	while(ptr!=NULL)
	//	{
	//		m_styles[i++] = ptr;

	//		ptr = strtok(NULL, ",");
	//	}
	//	free(str);
	//}

	void GetFeatureInfoRequest::SetBBox(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		float xmin, ymin, xmax, ymax;
		sscanf(value,"%f,%f,%f,%f", &xmin, &ymin, &xmax, &ymax);
		m_extent.Set(xmin, ymin, xmax, ymax);
	}

	void GetFeatureInfoRequest::SetWidth(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_width = atof(value);
	}

	void GetFeatureInfoRequest::SetHeight(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_height = atof(value);
	}

	void GetFeatureInfoRequest::SetBgColor(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		if(strlen(value)==0)
		{
			return;
		}
		if(value[0]!='#')
		{
			return;
		}
		g_byte r=0,g=0,b=0,a=255;
		sscanf(value,"#%02x%02x%02x",&r,&g,&b);
		//sscanf(value,"#%02x%02x%02x%02x",&r,&g,&b,&a);
		m_bgColor.Set(r,g,b,a);
		float r_f = m_bgColor.GetRedF();
		int i=0;
		i=i+1;
	}

	void GetFeatureInfoRequest::SetTransparent(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		if(g_stricmp(value, "TRUE")==0)
		{
			m_bgColor.SetAlpha(0);
			m_transparent = true;
		}
		else
		{
			m_bgColor.SetAlpha(255);
			m_transparent = false;
		}
	}
}