#include "FieldFactoryImpl.h"
#include "GFieldImpl.h"
#include "GFieldsImpl.h"

namespace auge
{
	FieldFactory* augeGetFieldFactoryInstance()
	{
		static FieldFactoryImpl g_field_factory;
		return &g_field_factory;
	}

	FieldFactoryImpl::FieldFactoryImpl()
	{

	}

	FieldFactoryImpl::~FieldFactoryImpl()
	{

	}

	GField*	FieldFactoryImpl::CreateField()
	{
		return (new GFieldImpl());
	}

	GFields* FieldFactoryImpl::CreateFields()
	{
		return (new GFieldsImpl());
	}
}