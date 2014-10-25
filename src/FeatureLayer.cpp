#include "Layer.h"
#include "FeatureLayer.h"
#include "FeatureType.h"
#include "Style.h"

namespace auge
{

	FeatureLayer::FeatureLayer():
	m_pFeatureType(NULL),
	m_pStyle(NULL)
	{

	}

	FeatureLayer::~FeatureLayer()
	{
		if(m_pFeatureType!=NULL)
		{
			m_pFeatureType->Release();
			m_pFeatureType = NULL;
		}
		if(m_pStyle!=NULL)
		{
			m_pStyle->Release();
			m_pStyle = NULL;
		}
	}

	LayerType FeatureLayer::GetType()
	{
		return augeLayerFeature;
	}

	int FeatureLayer::SetFeatureType(FeatureType* pFeatureType)
	{
		if(pFeatureType==NULL)
		{
			return AG_FAILURE;
		}
		if(m_pFeatureType!=NULL)
		{
			m_pFeatureType->Release();
		}
		m_pFeatureType = pFeatureType;
		return AG_SUCCESS;
	}

	FeatureType* FeatureLayer::GetFeatureType()
	{
		return m_pFeatureType;
	}

	Envelope& FeatureLayer::GetExtent()
	{
		return m_pFeatureType->GetExtent();
	}

	Style* FeatureLayer::GetStyle()
	{
		return m_pStyle;
	}

	void FeatureLayer::SetStyle(Style* pStyle)
	{
		if(pStyle==m_pStyle)
		{
			return;
		}

		if(m_pStyle!=NULL)
		{
			m_pStyle->Release();
			m_pStyle = NULL;
		}
		m_pStyle = pStyle;
	}
}