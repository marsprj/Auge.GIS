#include "Layer.h"
#include "Map.h"

namespace auge
{

	Layer::Layer():
	m_pMap(NULL)
	{

	}

	Layer::~Layer()
	{

	}

	Map* Layer::GetMap()
	{
		return m_pMap;
	}

	void Layer::SetMap(Map* pMap)
	{
		m_pMap = pMap;
	}

	const char* Layer::GetName()
	{
		return m_name.c_str();
	}

	int	Layer::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}
		m_name = szName;

		return AG_SUCCESS;
	}

	LayerType Layer::GetType()
	{
		return augeLayerUnknown;
	}

	Envelope& Layer::GetExtent()
	{
		Envelope extent;
		return extent;
	}

	void Layer::Release()
	{
		delete this;
	}
}