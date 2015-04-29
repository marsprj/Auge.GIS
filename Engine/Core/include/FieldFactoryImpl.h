#ifndef __AUGE_FIELD_FACTORY_IMPL_H__
#define __AUGE_FIELD_FACTORY_IMPL_H__

#include "AugeField.h"

namespace auge
{
	class FieldFactoryImpl : public FieldFactory
	{
	public:
		FieldFactoryImpl();
		virtual ~FieldFactoryImpl();
	public:
		virtual GField*			CreateField();
		virtual GFields*		CreateFields();
		virtual const char*		Encode(augeFieldType type);
		virtual augeFieldType	Decode(const char* type);
	};
}

#endif //__AUGE_FIELD_FACTORY_IMPL_H__
