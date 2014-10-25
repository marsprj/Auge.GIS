#include "Field.h"

namespace auge
{

	Field::Field():
	m_type(augeFieldTypeNone),
	m_length(0)
	{

	}

	Field::~Field()
	{

	}

	const char*	Field::GetName()
	{
		return m_name.c_str();
	}

	int	Field::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}
		m_name = szName;
		return AG_SUCCESS;
	}

	FieldType Field::GetType()
	{
		return m_type;
	}

	void Field::SetType(FieldType type)
	{
		m_type = type;
	}

	int Field::GetLength()
	{
		return m_length;
	}

	void Field::SetLength(int length)
	{
		m_length = length;
	}

	GeometryType Field::GetGeometryType()
	{
		return m_geom_type;
	}

	void Field::SetGeometryType(GeometryType type)
	{
		m_geom_type = type;
	}
}