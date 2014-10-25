#include "FeatureSet.h"

namespace auge
{
	FeatureSet::FeatureSet():
	m_pFeatureType(NULL),
	m_pgResult(NULL),
	m_total(0),
	m_index(0),
	m_format(WKB)
	{

	}

	FeatureSet::~FeatureSet()
	{
		if(m_pgResult!=NULL)
		{
			PQclear(m_pgResult);
			m_pgResult = NULL;
			m_total = 0;
			m_index = 0;
		}
	}

	Feature& FeatureSet::Next()
	{
		m_feature.m_index = m_index++;
		return m_feature;
	}

	bool FeatureSet::IsEOF()
	{
		return (m_index>=m_total);
	}

	void FeatureSet::Reset()
	{
		m_index = 0;
	}

	bool FeatureSet::Create(FeatureType* pFeatureType, PGresult* pgResult, GeometryFormat format)
	{
		m_pFeatureType = pFeatureType;
		m_pgResult = pgResult;
		m_format = format;

		m_total = PQntuples(pgResult);
		m_feature.m_index = 0;
		m_feature.m_pgResult = pgResult;
		m_feature.m_format = m_format;
		m_feature.m_pFeatureType = pFeatureType;

		return true;

	}

	FeatureType* FeatureSet::GetFeatureType()
	{
		return m_pFeatureType;
	}
}