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
		virtual GField*		CreateField();
		virtual GFields*	CreateFields();
	};
}

#endif //__AUGE_FIELD_FACTORY_IMPL_H__
