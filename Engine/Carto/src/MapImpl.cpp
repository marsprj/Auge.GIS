#include "MapImpl.h"

namespace auge
{
	MapImpl::MapImpl():
	m_srid(4326)
	{

	}

	MapImpl::~MapImpl()
	{
		Cleanup();
	}

	const char*	MapImpl::GetName()
	{
		return m_name.c_str();
	}

	RESULTCODE MapImpl::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}
		m_name = szName;
		return AG_SUCCESS;
	}

	g_uint MapImpl::GetLayerCount()
	{
		return m_layers.size();
	}

	Layer* MapImpl::GetLayer(g_uint i)
	{	
		if(i>=m_layers.size())
		{
			return NULL;
		}
		std::map<std::string, Layer*>::iterator iter;
		iter = m_layers.begin();
		for(g_uint n=0; n<i; n++, iter++);
		return iter->second;
	}

	Layer* MapImpl::GetLayer(const char* szName)
	{
		if(szName==NULL)
		{
			return NULL;
		}
		std::string strName = szName;
		return m_layers[strName];
	}

	RESULTCODE MapImpl::AddLayer(Layer* pLayer)
	{
		if(pLayer==NULL)
		{
			return AG_FAILURE;
		}

		const char* name = pLayer->GetName();
		if(name==NULL)
		{
			return AG_FAILURE;
		}

		Layer* pl = NULL;
		pl = GetLayer(name);
		if(pl!=NULL)
		{
			return AG_FAILURE;
		}

		std::string strName = name;
		m_layers[strName] = pLayer;
		return AG_SUCCESS;
	}

	void MapImpl::Cleanup()
	{
		Layer* pLayer = NULL;
		std::map<std::string, Layer*>::iterator iter;
		for(iter=m_layers.begin(); iter!=m_layers.end(); iter++)
		{
			pLayer = iter->second;
			if(pLayer!=NULL)
			{
				pLayer->Release();
			}
		}
		m_layers.clear();
	}

	GEnvelope& MapImpl::GetExtent()
	{
		return m_exent;
	}

	g_int MapImpl::GetSRID()
	{
		return m_srid; 
	}
}