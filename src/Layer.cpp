#include "Layer.h"

namespace auge
{

	Layer::Layer()
	{

	}

	Layer::~Layer()
	{

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