#ifndef __AUGE_SQL_BUILDER_PGS_H__
#define __AUGE_SQL_BUILDER_PGS_H__

#include "AugeField.h"
#include "AugeFeature.h"

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
		static void	BuildQuery(std::string& sql, FeatureClass* pFeatureClass);

		static void BuildInsertFeature(std::string& sql, const char* className, Feature* pFeature, g_uint srid);

		static const char* GetPgFieldType(augeFieldType type);
	};
}

#endif __AUGE_SQL_BUILDER_PGS_H__
