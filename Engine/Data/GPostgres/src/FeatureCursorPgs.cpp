#include "FeatureCursorPgs.h"
#include "FeatureClassPgs.h"
#include "FeaturePgs.h"

namespace auge
{
	FeatureCursorPgs::FeatureCursorPgs():
	m_cursor(-1),
	m_count(0),
	m_pgResult(NULL),
	m_geom_findex(-1)
	{

	}

	FeatureCursorPgs::~FeatureCursorPgs()
	{
		if(m_pgResult!=NULL)
		{
			PQclear(m_pgResult);
			m_pgResult = NULL;
		}
		AUGE_SAFE_RELEASE(m_pFeatureClass);
	}

	Feature* FeatureCursorPgs::NextFeature()
	{
		if(m_cursor<0||(unsigned)m_cursor>=m_count)
		{
			return NULL;
		}

		int index = -1;
		FeaturePgs* pFeature = new FeaturePgs();
		if(pFeature!=NULL)
		{
			//index = (m_bIsAll) ? m_cursor : m_SHPIDs[m_cursor];
			index = m_cursor;
			if(!pFeature->Create(index, m_geom_findex, m_pgResult, m_pFeatureClass))
			{
				delete pFeature;
				pFeature = NULL;
			}
		}

		m_cursor++;
		return pFeature;
	}

	void FeatureCursorPgs::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	bool FeatureCursorPgs::Create(FeatureClassPgs* pFeatureClass, PGresult* pgResult)
	{
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();

		m_pgResult = pgResult;
		m_count = PQntuples(m_pgResult);
		m_cursor = 0;

		GFields* pFields = m_pFeatureClass->GetFields();

		m_geom_findex = pFields->FindField(pFields->GetGeometryField()->GetName());

		return true;
	}

}