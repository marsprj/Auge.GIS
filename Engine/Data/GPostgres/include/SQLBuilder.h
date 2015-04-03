#ifndef __AUGE_SQL_BUILDER_PGS_H__
#define __AUGE_SQL_BUILDER_PGS_H__

#include "AugeField.h"
#include "AugeFeature.h"

#include "AugeFilter.h"

namespace auge
{
	class GFields;
	class FeatureClassPgs;

	class SQLBuilder
	{
	public:
		SQLBuilder(){}
		virtual ~SQLBuilder(){}
	public:
		static void BuildCreateTable(std::string& sql, const char* name, GFields* pFields);
		static void	BuildQuery(std::string& sql, FeatureClass* pFeatureClass);
		static void	BuildQuery(std::string& sql, GEnvelope& extent, FeatureClassPgs* pFeatureClass);
		static void	BuildQuery(std::string& sql, GFilter* pFilter, FeatureClassPgs* pFeatureClass);
		static void	BuildQuery(std::string& sql, GQuery* pQuery, FeatureClassPgs* pFeatureClass);
		static void BuildFields(std::string& fields, FeatureClassPgs* pFeatureClass);
		static void BuildFields(std::string& fields, GQuery* pQuery, FeatureClassPgs* pFeatureClass);

		static RESULTCODE BuildFilter(std::string& sql, FeatureClassPgs* pFeatureClass, GFilter* pFilter);
		static RESULTCODE BuildIDFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IDFilter* pFilter);
		static RESULTCODE BuildComparisioFilter(std::string& sql,FeatureClassPgs* pFeatureClass, ComparisionFilter* pFilter);
		static RESULTCODE BuildLogicFilter(std::string& sql,FeatureClassPgs* pFeatureClass, LogicFilter* pFilter);
		static RESULTCODE BuildSpatialFilter(std::string& sql,FeatureClassPgs* pFeatureClass, SpatialFilter* pFilter);

		static RESULTCODE BuildBinaryComparisonFilter(std::string& sql,FeatureClassPgs* pFeatureClass, BinaryComparisonFilter *pFilter);
		static RESULTCODE BuildIsLikeFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IsLikeFilter *pFilter);
		static RESULTCODE BuildIsNullFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IsNullFilter *pFilter);
		static RESULTCODE BuildIsBwtweenFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IsBetweenFilter *pFilter);

		static RESULTCODE BuildBinaryLogicFilter(std::string& sql,FeatureClassPgs* pFeatureClass, BinaryLogicFilter *pFilter);
		static RESULTCODE BuildUnaryLogicFilter(std::string& sql,FeatureClassPgs* pFeatureClass, UnaryLogicFilter *pFilter);

		static RESULTCODE BuildBBoxFilter(std::string& sql,FeatureClassPgs* pFeatureClass, BBoxFilter* pFilter);

		static RESULTCODE BuildExpression(std::string &sql, FeatureClassPgs* pFeatureClass, Expression* pExpression);
		static RESULTCODE BuildArithmeticExpression(std::string &sql, FeatureClassPgs* pFeatureClass, ArithmeticExpression* pExpression);
		static RESULTCODE BuildPropertyNameExpression(std::string &sql, FeatureClassPgs* pFeatureClass, PropertyName* pExpression);
		static RESULTCODE BuildLiteralExpression(std::string &sql, FeatureClassPgs* pFeatureClass, Literal* pExpression);
		static RESULTCODE BuildFunctitonExpression(std::string &sql, FeatureClassPgs* pFeatureClass, Function* pExpression);

		static RESULTCODE BuildGetUniqueValueSQL(std::string& sql, const char* field_name, FeatureClassPgs* pFeatureClass, augeOrderType order);

		static void BuildInsertFeature(std::string& sql, const char* className, Feature* pFeature, g_uint srid);
		static void BuildDeleteFeature(std::string& sql, GFilter* pFilter, FeatureClassPgs* pFeatureClass);

		static const char* GetPgFieldType(augeFieldType type);
	};
}

#endif //__AUGE_SQL_BUILDER_PGS_H__

