#include "FeatureCursorShp.h"
#include "FeatureClassShp.h"
#include "FeatureShp.h"

namespace auge
{
	FeatureCursorShp::FeatureCursorShp():
	m_pFeatureClass(NULL),
	m_pSHPHandle(NULL),
	m_pDBFHandle(NULL),
	m_Cursor(-1),
	m_Count(0),
	m_bIsAll(false)
	{

	}

	FeatureCursorShp::~FeatureCursorShp()
	{
		AUGE_SAFE_RELEASE(m_pFeatureClass);
	}

	FeatureClass* FeatureCursorShp::GetFeatureClass()
	{
		return m_pFeatureClass;
	}

	Feature* FeatureCursorShp::NextFeature()
	{
		if(m_Cursor<0||(unsigned)m_Cursor>=m_Count)
		{
			return NULL;
		}

		int index = -1;
		FeatureShp* pFeatureShp = new FeatureShp();
		if(pFeatureShp!=NULL)
		{
			index = m_Cursor;
			if(!pFeatureShp->Create(index, m_pFeatureClass))
			{
				delete pFeatureShp;
				pFeatureShp = NULL;
			}
		}

		m_Cursor++;
		return pFeatureShp;
	}

	void FeatureCursorShp::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool FeatureCursorShp::Create(FeatureClassShp* pFeatureClass, SHPHandle pSHPHandle, DBFHandle pDBFHandle)
	{
		m_pSHPHandle = pSHPHandle;
		m_pDBFHandle = pDBFHandle;
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();

		m_bIsAll = true;
		m_Cursor = 0;
		m_Count = m_pSHPHandle->nRecords;

		return true;
	}

}