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

	const char*	MapImpl::GetURI()
	{
		return m_uri.c_str();
	}

	RESULTCODE MapImpl::SetURI(const char* uri)
	{
		if(uri==NULL)
		{
			m_uri.clear();
		}
		else
		{
			m_uri = uri;
		}
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
		return m_layers[i];
	}

	Layer* MapImpl::GetLayer(const char* szName)
	{
		if(szName==NULL)
		{
			return NULL;
		}
		//std::string strName = szName;
		//return m_layers[strName];
		std::vector<Layer*>::iterator iter;
		for(iter = m_layers.begin(); iter!=m_layers.end(); iter++)
		{
			if(!strcmp(szName, (*iter)->GetName()))
			{
				return *iter;
			}
		}
		return NULL;
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

		//std::string strName = name;
		//m_layers[strName] = pLayer;
		m_layers.push_back(pLayer);
		return AG_SUCCESS;
	}

	void MapImpl::Cleanup()
	{
		//Layer* pLayer = NULL;
		//std::map<std::string, Layer*>::iterator iter;
		//for(iter=m_layers.begin(); iter!=m_layers.end(); iter++)
		//{
		//	pLayer = iter->second;
		//	if(pLayer!=NULL)
		//	{
		//		pLayer->Release();
		//	}
		//}
		//m_layers.clear();

		Layer* pLayer = NULL;
		std::vector<Layer*>::iterator iter;
		for(iter=m_layers.begin(); iter!=m_layers.end(); iter++)
		{
			pLayer = *iter;
			if(pLayer!=NULL)
			{
				pLayer->Release();
			}
		}
		m_layers.clear();
	}

	GEnvelope& MapImpl::GetExtent()
	{
		if(!m_layers.size())
		{
			return m_exent;
		}
		
		//std::map<std::string, Layer*>::iterator iter = m_layers.begin();
		//m_exent = iter->second->GetExtent();
		//for(iter++;iter!=m_layers.end(); iter++)
		//{
		//	Layer* pLayer = iter->second;
		//	GEnvelope& extent = pLayer->GetExtent();
		//	m_exent.Union(extent);
		//}
		std::vector<Layer*>::iterator iter = m_layers.begin();
		m_exent = (*iter)->GetExtent();
		for(iter++;iter!=m_layers.end(); iter++)
		{
			Layer* pLayer = *iter;
			GEnvelope& extent = pLayer->GetExtent();
			m_exent.Union(extent);
		}
		return m_exent;
	}

	g_int MapImpl::GetSRID()
	{
		return m_srid; 
	}

	void MapImpl::SetExtent(double xmin, double ymin, double xmax, double ymax)
	{
		m_exent.Set(xmin,ymin,xmax,ymax);
	}
	
	void MapImpl::SetSRID(g_int srid)
	{
		m_srid = srid;
	}
}