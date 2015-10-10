#ifndef __AUGE_SQL_BUILDER_H__
#define __AUGE_SQL_BUILDER_H__

#include "SQLBuilder.h"
#include "AugeFilter.h"
#include <string>

namespace auge
{
	class GField;
	class GFields;
	class GFilter;
	class Feature;
	class FeatureClass;
	class FeatureClassByd;

	class SQLBuilder
	{
	public:
		SQLBuilder();
		virtual ~SQLBuilder();
	public:
		static void	BuildQuery(std::string& sql, FeatureClassByd* pFeatureClass, GFilter* pFilter);
		static void	BuildQuery(std::string& sql, FeatureClassByd* pFeatureClass, GQuery* pQuery);
		static void	BuildQuery(std::string& sql, FeatureClassByd* pFeatureClass, GEnvelope& extent);

		static void BuildFields(std::string& fields, FeatureClassByd* pFeatureClass);
		static void BuildFields(std::string& fields, GQuery* pQuery, FeatureClassByd* pFeatureClass);

		static void BuildInsertFeature(std::string& sql, FeatureClassByd* pFeatureClass, Feature* pFeature);

		static void BuildCount(std::string& sql, FeatureClassByd* pFeatureClass);

		static RESULTCODE BuildFilter(std::string& sql, FeatureClassByd* pFeatureClass, GEnvelope& extent);
		static RESULTCODE BuildFilter(std::string& sql, FeatureClassByd* pFeatureClass, GFilter* pFilter);
		static RESULTCODE BuildIDFilter(std::string& sql,FeatureClassByd* pFeatureClass, IDFilter* pFilter);
		static RESULTCODE BuildComparisionFilter(std::string& sql,FeatureClassByd* pFeatureClass, ComparisonFilter* pFilter);
		static RESULTCODE BuildLogicFilter(std::string& sql,FeatureClassByd* pFeatureClass, LogicFilter* pFilter);
		static RESULTCODE BuildSpatialFilter(std::string& sql,FeatureClassByd* pFeatureClass, SpatialFilter* pFilter);

		static RESULTCODE BuildBinaryComparisonFilter(std::string& sql,FeatureClassByd* pFeatureClass, BinaryComparisonFilter *pFilter);
		static RESULTCODE BuildIsLikeFilter(std::string& sql,FeatureClassByd* pFeatureClass, IsLikeFilter *pFilter);
		static RESULTCODE BuildIsNullFilter(std::string& sql,FeatureClassByd* pFeatureClass, IsNullFilter *pFilter);
		static RESULTCODE BuildIsBwtweenFilter(std::string& sql,FeatureClassByd* pFeatureClass, IsBetweenFilter *pFilter);

		static RESULTCODE BuildBinaryLogicFilter(std::string& sql,FeatureClassByd* pFeatureClass, BinaryLogicFilter *pFilter);
		static RESULTCODE BuildUnaryLogicFilter(std::string& sql,FeatureClassByd* pFeatureClass, UnaryLogicFilter *pFilter);

		static RESULTCODE BuildBBoxFilter(std::string& sql,FeatureClassByd* pFeatureClass, BBoxFilter* pFilter);
		static RESULTCODE BuildWithinFilter(std::string& sql,FeatureClassByd* pFeatureClass, BinarySpatialFilter* pFilter);
		static RESULTCODE BuildDWithinFilter(std::string& sql,FeatureClassByd* pFeatureClass, DistanceBufferFilter* pFilter);

		static RESULTCODE BuildExpression(std::string &sql, FeatureClassByd* pFeatureClass, Expression* pExpression);
		static RESULTCODE BuildArithmeticExpression(std::string &sql, FeatureClassByd* pFeatureClass, ArithmeticExpression* pExpression);
		static RESULTCODE BuildPropertyNameExpression(std::string &sql, FeatureClassByd* pFeatureClass, PropertyName* pExpression);
		static RESULTCODE BuildLiteralExpression(std::string &sql, FeatureClassByd* pFeatureClass, Literal* pExpression);
		static RESULTCODE BuildFunctitonExpression(std::string &sql, FeatureClassByd* pFeatureClass, Function* pExpression);

		static RESULTCODE BuildCreateSequence(std::string& sql, const char* name);
		static RESULTCODE BuildCreateTable(std::string& sql, const char* name, GFields* pFields);
		static RESULTCODE BuildFieldSQL(std::string& sql, GField* pField);
		
	};

}


#endif //__AUGE_SQL_BUILDER_H__
