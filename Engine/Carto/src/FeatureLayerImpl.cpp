#include "FeatureLayerImpl.h"
#include "AugeField.h"

namespace auge
{
	FeatureLayerImpl::FeatureLayerImpl()
	{
		m_queryable = true;
		m_visiable = true;
		m_pStyle = NULL;
		m_style_id = -1;
		m_pFeatureClass = NULL;
		m_srid = AUGE_DEFAULT_SRID;
		m_min_scale = -1.0;
		m_max_scale = -1.0;
	}

	FeatureLayerImpl::~FeatureLayerImpl()
	{
		if(m_pFeatureClass!=NULL)
		{
			m_pFeatureClass->Release();
			m_pFeatureClass = NULL;
		}
		if(m_pStyle!=NULL)
		{
			m_pStyle->Release();
			m_pStyle = NULL;
		}
	}

	augeLayerType FeatureLayerImpl::GetType()
	{
		return augeLayerFeature;
	}

	g_int FeatureLayerImpl::GetSRID()
	{
		return m_srid;
	}

	void FeatureLayerImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}

	const char* FeatureLayerImpl::GetName()
	{
		return m_name.c_str();
	}

	void FeatureLayerImpl::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			m_name.clear();
		}
		m_name = szName;
	}

	RESULTCODE FeatureLayerImpl::SetFeatureClass(FeatureClass* pFeatureClass)
	{
		if(m_pFeatureClass!=NULL)
		{
			m_pFeatureClass->Release();
			m_pFeatureClass = NULL;
		}
		m_pFeatureClass = pFeatureClass;
		//if(m_pFeatureClass!=NULL)
		//{
		//	m_pFeatureClass->AddRef();
		//}
		return AG_SUCCESS;
	}

	//FeatureClass* FeatureLayerImpl::GetFeatureClass()
	//{
	//	return m_pFeatureClass;
	//}

	FeatureClass* FeatureLayerImpl::GetFeatureClass()
	{
		if(m_pFeatureClass==NULL)
		{
			FeatureWorkspace* pWorkspace = NULL;
			ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

			pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspaceById(m_source_id));
			if(pWorkspace!=NULL)
			{	
				m_pFeatureClass = pWorkspace->OpenFeatureClass(m_class_name.c_str());
			}
		}
		return m_pFeatureClass;
	}
	
	GEnvelope& FeatureLayerImpl::GetExtent()
	{
		if(m_pFeatureClass==NULL)
		{
			m_pFeatureClass = GetFeatureClass();
		}
		if(m_pFeatureClass!=NULL)
		{
			m_extent = m_pFeatureClass->GetExtent();
		}
		return m_extent;
	}

	bool FeatureLayerImpl::IsQueryable()
	{
		return m_queryable;
	}

	void FeatureLayerImpl::SetQueryable(bool flag)
	{
		m_queryable = flag;
	}

	bool FeatureLayerImpl::IsVisiable()
	{
		return m_visiable;
	}

	bool FeatureLayerImpl::IsVisiable(double scale)
	{
		if(scale<0)
		{
			return true;
		}

		return ((scale>m_min_scale) && (scale<m_max_scale));
	}

	void FeatureLayerImpl::SetVisiable(bool flag)
	{
		m_visiable = flag;
	}

	void FeatureLayerImpl::SetStyleID(g_int style_id)
	{
		m_style_id = style_id;
	}

	RESULTCODE FeatureLayerImpl::SetStyle(Style* pStyle)
	{
		if(m_pStyle!=NULL)
		{
			m_pStyle->Release();
		}
		m_pStyle = pStyle;
		return AG_SUCCESS;
	}

	Style* FeatureLayerImpl::GetStyle()
	{
		if(m_pStyle==NULL)
		{
			if(m_style_id>0)
			{
				FeatureClass* pFeatureClass = GetFeatureClass();
				if(pFeatureClass!=NULL)
				{
					CartoManager* pCartoManager = augeGetCartoManagerInstance();
					m_pStyle = pCartoManager->GetStyle(m_style_id, pFeatureClass);
				}
			}
			else
			{
				FeatureClass* pFeatureClass = GetFeatureClass();
				if(pFeatureClass!=NULL)
				{
					GField* pField = pFeatureClass->GetFields()->GetGeometryField();
					if(pField!=NULL)
					{
						StyleFactory* pStyleFactory = augeGetStyleFactoryInstance();
						m_pStyle = pStyleFactory->CreateFeatureStyle(pField->GetGeometryDef()->GeometryType());
					}
				}

				GLogger* pLogger = augeGetLoggerInstance();
				char msg[AUGE_MSG_MAX];
				memset(msg, 0, AUGE_MSG_MAX);
				g_snprintf(msg, AUGE_MSG_MAX, "Layer [%s] set random style", m_name.c_str());
				pLogger->Info(msg, __FILE__, __LINE__);
			}
		}
		return m_pStyle;
	}

	double FeatureLayerImpl::GetMinScale()
	{
		return m_min_scale;
	}

	void FeatureLayerImpl::SetMinScale(double scale)
	{
		m_min_scale = scale;
	}

	double FeatureLayerImpl::GetMaxScale()
	{
		return m_max_scale;
	}

	void FeatureLayerImpl::SetMaxScale(double scale)
	{
		m_max_scale = scale;
	}

	void FeatureLayerImpl::SetFeatureSource(g_uint source_id)
	{
		m_source_id = source_id;
	}

	void FeatureLayerImpl::SetFeatureClassName(const char* className)
	{
		m_class_name = className;
	}
}