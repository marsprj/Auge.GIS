#include "AugeCore.h"
#include "SLDConstant.h"
#include "StyleReaderImpl.h"
#include "StyleReader_1_0_0.h"
#include "AugeFeature.h"

namespace auge
{

	StyleReaderImpl::StyleReaderImpl():
	m_pxDoc(NULL)
	{

	}

	StyleReaderImpl::~StyleReaderImpl()
	{
		if(m_pxDoc!=NULL)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;
		}
	}

	void StyleReaderImpl::Release()
	{
		delete this;
	}

	Style* StyleReaderImpl::Read(const char* szPath, FeatureClass* pFeatureClass)
	{
		Style* pStyle = NULL;

		if(m_pxDoc!=NULL)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;
		}

		GLogger *pLogger = augeGetLoggerInstance();
		XParser parser;
		m_pxDoc = parser.Parse(szPath);
		if(m_pxDoc==NULL)
		{
			return NULL;
		}

		XNode *psldRootNode = NULL;
		psldRootNode = m_pxDoc->GetRootNode();
		if(psldRootNode==NULL)
		{
			return NULL;
		}
		const char* szRootName = psldRootNode->GetName();
		if(g_stricmp(szRootName, AUGE_SLD_ROOT_NAME)!=0)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;

			pLogger->Error("不是SLD文档", __FILE__, __LINE__);
			return NULL;
		}

		XAttribute* pxmlVersionNode = NULL;
		XElement* psldRootElement = (XElement*)psldRootNode;
		pxmlVersionNode = psldRootElement->GetAttribute(AUGE_SLD_VERSION);
		if(pxmlVersionNode==NULL)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;

			pLogger->Error("未指定SLD版本", __FILE__, __LINE__);
			return NULL;
		}

		const char* szVersion = pxmlVersionNode->GetValue();
		if(g_stricmp(szVersion, AUGE_SLD_VERSION_1_0_0)==0)
		{
			pStyle = Read_1_0_0(psldRootElement, pFeatureClass);
		}
		else if(g_stricmp(szVersion, AUGE_SLD_VERSION_1_0_0)==0)
		{
			pStyle = Read_1_1_0(psldRootElement, pFeatureClass);
		}
		else
		{
			pLogger->Error("不支持的SLD版本", __FILE__, __LINE__);
			return NULL;
		}

		m_pxDoc->Close();
		m_pxDoc->Release();
		m_pxDoc = NULL;
		return pStyle;
	}

	Style* StyleReaderImpl::Read(const char* buffer, int length, FeatureClass* pFeatureClass)
	{
		Style* pStyle = NULL;

		if(m_pxDoc!=NULL)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;
		}

		GLogger *pLogger = augeGetLoggerInstance();
		pLogger->Trace(buffer, __FILE__, __LINE__);

		XParser parser;
		m_pxDoc = parser.ParseMemory(buffer);
		if(m_pxDoc==NULL)
		{
			return NULL;
		}

		XNode *psldRootNode = NULL;
		psldRootNode = m_pxDoc->GetRootNode();
		if(psldRootNode==NULL)
		{
			return NULL;
		}
		const char* szRootName = psldRootNode->GetName();
		if(g_stricmp(szRootName, AUGE_SLD_ROOT_NAME)!=0)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;

			pLogger->Error("不是SLD文档", __FILE__, __LINE__);
			return NULL;
		}

		XAttribute* pxmlVersionNode = NULL;
		XElement* psldRootElement = (XElement*)psldRootNode;
		pxmlVersionNode = psldRootElement->GetAttribute(AUGE_SLD_VERSION);
		if(pxmlVersionNode==NULL)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;

			pLogger->Error("未指定SLD版本", __FILE__, __LINE__);
			return NULL;
		}

		const char* szVersion = pxmlVersionNode->GetValue();
		if(g_stricmp(szVersion, AUGE_SLD_VERSION_1_0_0)==0)
		{
			pStyle = Read_1_0_0(psldRootElement,pFeatureClass);
		}
		else if(g_stricmp(szVersion, AUGE_SLD_VERSION_1_0_0)==0)
		{
			pStyle = Read_1_1_0(psldRootElement,pFeatureClass);
		}
		else
		{
			pLogger->Error("不支持的SLD版本", __FILE__, __LINE__);
			return NULL;
		}

		m_pxDoc->Close();
		m_pxDoc->Release();
		m_pxDoc = NULL;
		return pStyle;
	}


	//////////////////////////////////////////////////////////////////////////

	Style* StyleReaderImpl::Read_1_0_0(XElement* pxRoot, FeatureClass* pFeatureClass)
	{
		Style* pStyle = NULL;
		StyleReader_1_0_0 reader;
		pStyle = reader.Read(pxRoot, pFeatureClass);
		return pStyle;
	}

	Style* StyleReaderImpl::Read_1_1_0(XElement* pxRoot, FeatureClass* pFeatureClass)
	{
		Style* pStyle = NULL;

		return pStyle;
	}

}
