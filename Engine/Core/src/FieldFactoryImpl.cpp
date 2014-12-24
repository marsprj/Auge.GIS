#include "FieldFactoryImpl.h"
#include "GFieldImpl.h"
#include "GFieldsImpl.h"

namespace auge
{
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