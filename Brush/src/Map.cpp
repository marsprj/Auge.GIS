#include "Map.h"
#include "Layer.h"

namespace auge
{
	Map::Map()
	{

	}

	Map::~Map()
	{
		Cleanup();
	}


	int	Map::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}
		m_name = szName;

		return AG_SUCCESS;
	}

	const char*	Map::GetName()
	{
		return m_name.c_str();
	}

	Envelope& Map::GetExtent()
	{	
		return m_extent;
	}

	vector<Layer*>& Map::GetLayers()
	{
		return m_layers;
	}

	uint Map::GetLayerCount()
	{
		return m_layers.size();
	}

	Layer* Map::GetLayer(uint i)
	{
		return m_layers[i];
	}

	Layer* Map::GetLayer(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		Layer* l = NULL;
		std::vector<Layer*>::iterator iter;
		for(iter=m_layers.begin(); iter!=m_layers.end(); iter++)
		{
			l = *iter;
			if(strcmp(l->GetName(), name)==0)
			{
				return l;
			}
		}
		return NULL;
	}

	uint Map::AddLayer(Layer* pLayer)
	{
		if(pLayer==NULL)
		{
			return AG_FAILURE;
		}

		Layer* l = GetLayer(pLayer->GetName());
		if(l!=NULL)
		{
			return AG_FAILURE;
		}

		if(m_layers.empty())
		{
			m_extent = pLayer->GetExtent();
		}
		else
		{
			m_extent.Union(pLayer->GetExtent());
		}
		pLayer->SetMap(this);
		m_layers.push_back(pLayer);
			
		return AG_FAILURE;
	}

	uint Map::RemoveLayer(uint i)
	{
		Layer* l = m_layers[i];
		if(l!=NULL)
		{
			l->Release();
			m_layers.erase(m_layers.begin()+i);
		}
		return AG_FAILURE;
	}

	uint Map::RemoveLayer(const char* name)
	{
		Layer* l = NULL;
		std::vector<Layer*>::iterator iter;
		for(iter=m_layers.begin(); iter!=m_layers.end(); iter++)
		{
			l = *iter;
			if(strcmp(l->GetName(), name)==0)
			{
				m_layers.erase(iter);
				break;
			}
		}
		return AG_FAILURE;
	}

	void Map::Release()
	{
		delete this;
	}

	void Map::Cleanup()
	{
		Layer *pLayer = NULL;
		vector<Layer*>::iterator iter;
		for(iter=m_layers.begin(); iter!=m_layers.end(); iter++)
		{
			pLayer = *iter;
			pLayer->Release();
		}
		m_layers.clear();
	}
}