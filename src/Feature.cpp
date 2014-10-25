#include "Feature.h"
#include "FeatureType.h"
#include "Field.h"

namespace auge
{
	Feature::Feature():
	m_pgResult(NULL),
	m_index(0),
	m_wkb(NULL)
	{

	}

	Feature::~Feature()
	{
		if(m_wkb!=NULL)
		{
			PQfreemem(m_wkb);
		}
	}

	std::vector<Field*>& Feature::GetFields()
	{
		return m_pFeatureType->GetFields();
	}

	char* Feature::GetValueAsString(uint i)
	{
		return PQgetvalue(m_pgResult, m_index, i);
	}

	byte* Feature::GetBytes(uint i)
	{
		byte* b = NULL;
		Field& f = m_pFeatureType->GetField(i);
		if(f.GetType()==augeFieldTypeGeometry)
		{
			switch(m_format)
			{
			case WKB:
				{
					if(m_wkb!=NULL)
					{
						PQfreemem(m_wkb);
						m_wkb = NULL;
					}
					size_t len;
					m_wkb = PQunescapeBytea((uchar*)PQgetvalue(m_pgResult, m_index, i), &len);
					b = m_wkb;
				}
				break;
			}
		}
		return b;
	}
}