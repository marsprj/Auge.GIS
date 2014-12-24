#include "FeatureShp.h"
#include "FeatureClassShp.h"
#include "ShpUtil.h"

namespace auge
{
	FeatureShp::FeatureShp():
	m_pFeatureClass(NULL),
	m_pGeometry(NULL)
	{

	}

	FeatureShp::~FeatureShp()
	{
		AUGE_SAFE_RELEASE(m_pFeatureClass);
		AUGE_SAFE_RELEASE(m_pGeometry);
	}

	g_int FeatureShp::GetFID()
	{
		return m_fid;
	}

	Geometry* FeatureShp::GetGeometry()
	{
		if(m_pGeometry==NULL)
		{
			m_pGeometry = CreateGeometry(m_fid, m_pFeatureClass->m_pSHPHandle);
		}
		return m_pGeometry;
	}

	void FeatureShp::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool FeatureShp::Create(g_int lFID, FeatureClassShp* pFeatureClassShp)
	{
		m_fid = lFID;
		m_pFeatureClass = pFeatureClassShp;
		m_pFeatureClass->AddRef();

		return true;
	}

	Geometry* FeatureShp::CreateGeometry(long index, SHPHandle pSHPHandle)
	{
		if(pSHPHandle==NULL)
			return NULL;

		int	  iWKBLen = 0;
		g_uchar* pWKB = NULL;
		Geometry*	pGeometry  = NULL;
		SHPObject*	pSHPObject = NULL;
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		pSHPObject = ::SHPReadObject(pSHPHandle, index);
		if(pSHPObject==NULL)
			return NULL;

		iWKBLen = ShpUtil::GetWKBLength(pSHPObject);
		if(iWKBLen==0)
		{
			::SHPDestroyObject(pSHPObject);
			return NULL;
		}

		pWKB = (g_uchar*)auge_malloc(iWKBLen,sizeof(g_uchar));
		if(pWKB==NULL)
		{
			::SHPDestroyObject(pSHPObject);
			return NULL;
		}
		memset(pWKB, 0, iWKBLen);
		if(ShpUtil::SHPObject_2_WKB(pSHPObject, pWKB, iWKBLen)==0)
		{
			delete[] pWKB;
			::SHPDestroyObject(pSHPObject);
			return NULL;
		}

		pGeometry = pGeometryFactory->CreateGeometryFromWKB(pWKB, true);

		if(pGeometry==NULL)
		{
			delete[] pWKB;
		}

		::SHPDestroyObject(pSHPObject);

		return pGeometry;
	}
	/*
	Geometry* FeatureShp::CreateGeometry(long index, SHPHandle pSHPHandle)
	{
	if(pSHPHandle==NULL)
	return NULL;

	int	  iWKBLen = 0;
	g_uchar* pWKB = NULL;
	Geometry*	pGeometry  = NULL;
	SHPObject*	pSHPObject = NULL;
	UtilShp*  pSHPUtil = UtilShp::GetInstance();
	GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

	pSHPObject = ::SHPReadObject(pSHPHandle, index);
	if(pSHPObject==NULL)
	return NULL;

	iWKBLen = ShpUtil::GetWKBLength(pSHPObject);
	if(iWKBLen==0)
	{
	::SHPDestroyObject(pSHPObject);
	return NULL;
	}

	pWKB = new g_uchar[iWKBLen];
	if(pWKB==NULL)
	{
	::SHPDestroyObject(pSHPObject);
	return NULL;
	}
	memset(pWKB, 0, iWKBLen);
	if(ShpUtil::SHPObject_2_WKB(pSHPObject, pWKB, iWKBLen)==0)
	{
	delete[] pWKB;
	::SHPDestroyObject(pSHPObject);
	return NULL;
	}

	pGeometry = pGeometryFactory->CreateGeometryFromWKB(pWKB, true, true);

	if(pGeometry==NULL)
	{
	delete[] pWKB;
	}

	::SHPDestroyObject(pSHPObject);

	return pGeometry;
	}*/


}