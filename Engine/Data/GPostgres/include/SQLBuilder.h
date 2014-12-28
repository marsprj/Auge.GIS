#ifndef __AUGE_SQL_BUILDER_PGS_H__
#define __AUGE_SQL_BUILDER_PGS_H__

#include "AugeField.h"

namespace auge
{
	class GFields;

	class SQLBuilder
	{
	public:
		SQLBuilder(){}
		virtual ~SQLBuilder(){}
	public:
		static void BuildCreateTable(std::string& sql, const char* name, GFields* pFields);

		static const char* GetPgFieldType(augeFieldType type);
	};
}

#endif __AUGE_SQL_BUILDER_PGS_H__
