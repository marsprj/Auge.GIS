#ifndef __AUGE_FIELD_H__
#define __AUGE_FIELD_H__

#include "Base.h"
#include "Geometry.h"
#include <string>

namespace auge
{
	typedef enum FieldType
	{
		augeFieldTypeNone     = 0,
		augeFieldTypeBool     = 1,
		augeFieldTypeChar     = 2,
		augeFieldTypeShort    = 3,
		augeFieldTypeInt      = 4,
		augeFieldTypeLong     = 5,
		augeFieldTypeInt64    = 6,
		augeFieldTypeFloat    = 7,
		augeFieldTypeDouble   = 8,
		augeFieldTypeString   = 9,
		augeFieldTypeTime     = 10,
		augeFieldTypeBLOB     = 11,
		augeFieldTypeGeometry = 12,
		augeFieldTypeTopoGeometry = 13,
		augeFieldTypeCLOB	  = 14,
		augeFieldTypeNCLOB	  = 15,
		augeFieldTypeSerial	  = 16

	}FieldType;

	class AUGE_API Field : public Base
	{
	public:
		Field();
		virtual ~Field();

	public:
		const char*		GetName();
		int				SetName(const char* szName);

		FieldType		GetType();
		void			SetType(FieldType type);

		int				GetLength();
		void			SetLength(int length);

		GeometryType	GetGeometryType();
		void			SetGeometryType(GeometryType type);

	private:
		std::string		m_name;
		FieldType		m_type;
		int				m_length;
		GeometryType	m_geom_type;
	};

}//namespace

#endif //__AUGE_FIELD_H__
