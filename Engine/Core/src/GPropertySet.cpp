#include "AugeCore.h"

namespace auge
{
	GPropertySet::GPropertySet()
	{

	}

	GPropertySet::GPropertySet(const GPropertySet& other)
	{

	}

	GPropertySet::~GPropertySet()
	{

	}

	bool GPropertySet::Parse(const char* connection_string, const char *delimit/*=";"*/)
	{
		if(connection_string==NULL)
		{
			return false;
		}
		m_props.clear();

		char *cs = strdup(connection_string);
		if(cs!=NULL)
		{
			std::string key;
			char *token = strtok( cs, delimit );
			while( token != NULL )
			{
				char* ptr = strchr(token,'=');
				if(ptr!=NULL)
				{
					*ptr = 0;
					ptr++;
					key = token;
					m_props[key] = ptr;
				}
				token = strtok( NULL, delimit );
			}
			free(cs);
		}

		//UpdateKeys();

		return (!m_props.empty());
	}

	void GPropertySet::Clear()
	{
		m_props.clear();
	}

	bool GPropertySet::IsEmpty() const
	{
		return m_props.empty();
	}

	bool GPropertySet::SetValue(const char* key,const char* value)
	{
		if(NULL == key || NULL == value) 
		{
			return false;
		}

		std::string skey = key;
		if(NULL == value) 
		{
			std::map<std::string, std::string>::iterator iter= m_props.find(key);
			if(iter != m_props.end()) 
			{
				m_props.erase(iter);
			} 
			else 
			{
				return false;
			}
		} 
		else 
		{
			m_props[skey] = value;
		}

		//UpdateKeys();

		return true;
	}

	const char*	GPropertySet::GetValue(const char* key) const
	{
		if(NULL == key) 
		{
			return NULL;
		}

		std::string skey = key;		

		std::map<std::string, std::string>::const_iterator iter;
		for(iter=m_props.begin(); iter!=m_props.end(); iter++)
		{
			if(g_stricmp(iter->first.c_str(), key)==0)
			{
				return iter->second.c_str();
			}
		}
		return NULL;

		//const std::map<std::string, std::string>::const_iterator iter = m_props.find(skey);
		//if(iter != m_props.end()) 
		//{
		//	return (*iter).second.c_str();
		//}
		//return NULL;
	}
}