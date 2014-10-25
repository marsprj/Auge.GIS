#ifndef __AUGE_FEATURE_H__
#define __AUGE_FEATURE_H__

#include "Base.h"
#include "Geometry.h"
#include <vector>
#include <libpq-fe.h>

namespace auge
{
	class Field;
	class FeatureType;

	class AUGE_API Feature : public Base
	{
		friend class FeatureSet;
	public:
		Feature();
		virtual ~Feature();

	public:
		byte*		GetBytes(uint i);
		char*		GetValueAsString(uint i);

		std::vector<Field*>& GetFields();

	private:
		FeatureType	*m_pFeatureType;
		PGresult	*m_pgResult;
		int			m_index;

		uchar*		m_wkb;

		GeometryFormat	m_format;
	};
}

#endif //__AUGE_FEATURE_H__
