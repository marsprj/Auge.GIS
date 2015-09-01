#include "SQLBuilder.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeFilter.h"
#include "AugeGeometry.h"

#include "FeatureClassByd.h"

namespace auge
{
	SQLBuilder::SQLBuilder()
	{

	}

	SQLBuilder::~SQLBuilder()
	{

	}

	void SQLBuilder::BuildQuery(std::string& sql, FeatureClassByd* pFeatureClass, GFilter* pFilter)
	{
		GField 	*pField = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		g_uint count = pFields->Count();
		const char* fname  = NULL;

		bool first = true;

		sql = "select ";
		for(g_uint i=0; i<count; i++)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				sql.append(",");
			}

			pField = pFields->GetField(i);
			fname = pField->GetName();
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				sql.append("st_asbinary(");
				sql.append(fname);
				sql.append(") as ");
				sql.append(fname);
			}
			else
			{
				sql.append(fname);
			}
		}

		sql.append(" from ");
		//sql.append("\"");
		sql.append(pFeatureClass->GetName());
		//sql.append("\"");

		if(pFilter!=NULL)
		{
			std::string where;
			BuildFilter(where, pFeatureClass, pFilter);
		}
	}

	void SQLBuilder::BuildQuery(std::string& sql, FeatureClassByd* pFeatureClass, GQuery* pQuery)
	{
		GField 	*pField = NULL;
		GFilter	*pFilter = (GFilter	*)pQuery->GetFilter();
		GFields	*pFields = pFeatureClass->GetFields();
		const char* fname  = NULL;

		std::string fields;
		if(pQuery->GetSubFieldCount())
		{
			BuildFields(fields, pQuery, pFeatureClass);
		}
		else
		{
			BuildFields(fields, pFeatureClass);
		}

		bool first = true;
		sql = "select ";


		if(pQuery->GetMaxFeatures()>0)
		{
			char str[AUGE_NAME_MAX];
			g_sprintf(str, " first %d ", pQuery->GetMaxFeatures());
			sql.append(str);
		}

		sql.append(fields);
		sql.append(" from ");
		//sql.append("\"");
		sql.append(pFeatureClass->GetName());
		//sql.append("\"");

		if(pFilter!=NULL)
		{
			sql.append(" where ");
			std::string where = "";
			BuildFilter(where, pFeatureClass, pFilter);
			sql.append(where);
		}

		OrderBy* pOrderBy = pQuery->GetOrderBy();
		if(pOrderBy!=NULL)
		{
			int count = pOrderBy->GetFieldCount();
			if(count>0)
			{
				sql.append(" order by ");
				for(int i=0; i<count; i++)
				{
					if(i)
					{
						sql.append(",");
					}
					fname = pOrderBy->GetField(i);
					sql.append(fname==NULL ? "" : fname);
				}
				sql.append(pOrderBy->IsAsc() ? " asc" : " desc");
			}
		}
		if(pQuery->GetOffset()>0)
		{
			char str[AUGE_NAME_MAX];
			g_sprintf(str, " offset %d", pQuery->GetOffset());
			sql.append(str);
		}
	}

	void SQLBuilder::BuildQuery(std::string& sql, FeatureClassByd* pFeatureClass, GEnvelope& extent)
	{
		GField 	*pField = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		g_uint count = pFields->Count();
		const char* fname  = NULL;

		bool first = true;

		sql = "select ";
		for(g_uint i=0; i<1/*count*/; i++)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				sql.append(",");
			}

			pField = pFields->GetField(i);
			fname = pField->GetName();
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				sql.append("st_asbinary(");
				sql.append(fname);
				sql.append(") as ");
				sql.append(fname);
			}
			else
			{
				sql.append(fname);
			}
		}

		sql.append(" from ");
		//sql.append("\"");
		sql.append(pFeatureClass->GetName());
		//sql.append("\"");

		std::string where;
		BuildFilter(where, pFeatureClass, extent);
		sql.append(" where ");
		sql.append(where);
	}

	RESULTCODE SQLBuilder::BuildFilter(std::string& sql, FeatureClassByd* pFeatureClass, GEnvelope& extent)
	{
		char szsql[AUGE_SQL_MAX];
		memset(szsql, 0, AUGE_SQL_MAX);

		double xmax = extent.m_xmax;
		double xmin = extent.m_xmin;
		double ymin = extent.m_ymin;
		double ymax = extent.m_ymax;

		g_uint srid = pFeatureClass->GetSRID();
		if(srid>0)
		{
			const char* format = "st_within(%s,ST_GeomFromText('POLYGON((%f %f,%f %f,%f %f,%f %f,%f %f))',%d))=True";
			g_snprintf(szsql, AUGE_SQL_MAX, format, pFeatureClass->m_geom_field.c_str(), 
				xmin,ymin,
				xmin,ymax,
				xmax,ymax,
				xmax,ymin,
				xmin,ymin,
				pFeatureClass->GetSRID());
		}
		else
		{
			const char* format = "st_within(%s,ST_GeomFromText('POLYGON((%f %f,%f %f,%f %f,%f %f,%f %f))'))=True";
			g_snprintf(szsql, AUGE_SQL_MAX, format, pFeatureClass->m_geom_field.c_str(), 
				xmin,ymin,
				xmin,ymax,
				xmax,ymax,
				xmax,ymin,
				xmin,ymin);
		}
		
		sql = szsql;

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildFilter(std::string& sql, FeatureClassByd* pFeatureClass, GFilter* pFilter)
	{
		augeFilterType type = pFilter->GetType();
		switch(type)
		{
		case augeFilterFID:
			{
				BuildIDFilter(sql, pFeatureClass, static_cast<IDFilter*>(pFilter));
			}
			break;
		case augeFilterComparison:
			{
				BuildComparisionFilter(sql, pFeatureClass, static_cast<ComparisonFilter*>(pFilter));
			}
			break;
		case augeFilterLogic:
			{
				BuildLogicFilter(sql, pFeatureClass, static_cast<LogicFilter*>(pFilter));
			}
			break;
		case augeFilterSpatial:
			{
				BuildSpatialFilter(sql, pFeatureClass, static_cast<SpatialFilter*>(pFilter));
			}
			break;
		}
		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildIDFilter(std::string& sql,FeatureClassByd* pFeatureClass, IDFilter* pFilter)
	{	
		//FilterFactory* pFactory = augeGetFieldFactoryInstance();

		char sid[AUGE_MSG_MAX];
		g_uint count = pFilter->Count();
		//const char* oper = pFactory->AsString(pFilter->GetOperator());
		bool first = true;

		sql.append("gid in (");
		for(g_uint i=0; i<count; i++)
		{
			g_sprintf(sid,"%d",pFilter->GetID(i));
			if(first)
			{
				first = false;
			}
			else
			{
				sql.append(",");
			}
			sql.append(sid);
		}
		sql.append(")");
		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildComparisionFilter(std::string& sql,FeatureClassByd* pFeatureClass, ComparisonFilter* pFilter)
	{
		augeComparisonOperator oper = pFilter->GetOperator();

		switch(oper)
		{
		case augeComOprEqual:
		case augeComOprNotEqual:
		case augeComOprLessThan:
		case augeComOprGreaterThan:
		case augeComOprLessThanOrEqualTo:
		case augeComOprGreaterThanOrEqualTo:		
			{
				BuildBinaryComparisonFilter(sql, pFeatureClass, static_cast<BinaryComparisonFilter *>(pFilter));
			}
			break;
		case augeComOprIsLike:
			{
				BuildIsLikeFilter(sql, pFeatureClass, static_cast<IsLikeFilter *>(pFilter));
			}
			break;
		case augecomOprIsNull:
			{
				BuildIsNullFilter(sql, pFeatureClass, static_cast<IsNullFilter *>(pFilter));
			}
			break;
		case augeComOprIsBetween:
			{
				BuildIsBwtweenFilter(sql, pFeatureClass, static_cast<IsBetweenFilter *>(pFilter));
			}
			break;
		}
		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildBinaryComparisonFilter(std::string& sql, FeatureClassByd* pFeatureClass, BinaryComparisonFilter* pFilter)
	{
		RESULTCODE rc = AG_FAILURE;
		Expression* pLeft  = pFilter->GetExpression1();
		Expression* pRight = pFilter->GetExpression2();

		if(pLeft==NULL||pRight==NULL)
		{
			return AG_FAILURE;
		}

		std::string strLeft;
		std::string strRight;

		rc = BuildExpression(strLeft, pFeatureClass, pLeft);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		rc = BuildExpression(strRight, pFeatureClass, pRight);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		const char* szoper = pFactory->AsString(pFilter->GetOperator());
		sql.append("(");
		sql.append(strLeft);
		sql.append(")");
		sql.append(szoper);
		sql.append("(");
		sql.append(strRight);
		sql.append(")");
		return rc;
	}

	/*********************************************************/
	/* select name from country where name like 'C%';        */
	/*********************************************************/
	RESULTCODE SQLBuilder::BuildIsLikeFilter(std::string& sql,FeatureClassByd* pFeatureClass, IsLikeFilter *pFilter)
	{
		PropertyName* pPropName =  static_cast<PropertyName*>(pFilter->GetPropertyName());
		if(pPropName==NULL)
		{
			return AG_FAILURE;
		}
		Literal* pLiteral = static_cast<Literal*>(pFilter->GetLiteral());
		if(pLiteral==NULL)
		{
			return AG_FAILURE;
		}

		const char* fname = pPropName->GetName();
		if(fname==NULL)
		{
			return AG_FAILURE;
		}
		GValue* pValue = pLiteral->GetValue();
		if(pValue==NULL)
		{
			return AG_FAILURE;
		}

		const char* singleChar = pFilter->GetSingleChar();
		const char* escapeChar = pFilter->GetEscapeChar();
		const char* wildChar = pFilter->GetWildCard();

		GLogger *pLogger = augeGetLoggerInstance();
		pLogger->Trace(wildChar,__FILE__,__LINE__);

		const char* contidion = pValue->GetString();
		char contidion_2[AUGE_MSG_MAX];
		memset(contidion_2, 0, AUGE_MSG_MAX);
		size_t len = strlen(contidion); 
		char* ptr1 = (char*)contidion;
		char* ptr2 = contidion_2;
		for(size_t i=0; i<len; i++, ptr1++, ptr2++)
		{
			if((*ptr1)==(*singleChar))
			{
				*ptr2 = '?';
			}
			else if((*ptr1)==(*wildChar))
			{
				*ptr2 = '%';
			}
			else
			{
				*ptr2 = *ptr1;
			}
		}

		sql  = fname;
		sql.append(" like '");
		sql.append(contidion_2);
		sql.append("'");

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildIsNullFilter(std::string& sql,FeatureClassByd* pFeatureClass, IsNullFilter *pFilter)
	{
		RESULTCODE rc = AG_FAILURE;
		std::string strPropertyName = "";
		Expression* pPropertyName = pFilter->GetPropertyName();

		rc = BuildExpression(strPropertyName, pFeatureClass, pPropertyName);
		if(rc!=AG_SUCCESS)
		{
			return AG_FAILURE;
		}

		sql.append(" is null");
		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildIsBwtweenFilter(std::string& sql,FeatureClassByd* pFeatureClass, IsBetweenFilter *pFilter)
	{
		RESULTCODE rc = AG_FAILURE;
		Expression	*pExpression = pFilter->GetExpression();
		Expression	*pLowerBound = pFilter->GetLowerBound();
		Expression	*pUpperBound = pFilter->GetUpperBound();
		if(pExpression==NULL||pLowerBound==NULL||pUpperBound==NULL)
		{
			return AG_FAILURE;
		}

		std::string strExp="", strLower="", strUpper="";
		rc = BuildExpression(strExp,   pFeatureClass, pExpression);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		rc = BuildExpression(strLower, pFeatureClass, pLowerBound);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		rc = BuildExpression(strUpper, pFeatureClass, pUpperBound);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		sql.append(strExp);
		sql.append(" BETWEEN (");
		sql.append(strLower);
		sql.append(") AND (");
		sql.append(strUpper);
		sql.append(")");
		return AG_SUCCESS;
	}
	
	RESULTCODE SQLBuilder::BuildLogicFilter(std::string& sql,FeatureClassByd* pFeatureClass, LogicFilter* pFilter)
	{
		RESULTCODE rc = AG_FAILURE;

		augeLogicalOperator	oper = pFilter->GetOperator();
		switch(oper)
		{
		case augeLogOprAnd:
		case augeLogOprOr:
			{
				rc = BuildBinaryLogicFilter(sql, pFeatureClass, static_cast<BinaryLogicFilter *>(pFilter));
			}
			break;
		case augeLogOprNot:
			{
				rc = BuildUnaryLogicFilter(sql, pFeatureClass, static_cast<UnaryLogicFilter *>(pFilter));
			}
			break;
		}

		return rc;
	}

	RESULTCODE SQLBuilder::BuildBinaryLogicFilter(std::string& sql,FeatureClassByd* pFeatureClass, BinaryLogicFilter *pFilter)
	{
		RESULTCODE rc = AG_SUCCESS;
		GFilter* pSubFilter = NULL;
		EnumFilter* pSubFilters = pFilter->GetFilters();
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		const char* oper = pFactory->AsString(pFilter->GetOperator());
		std::string subsql;

		if(pSubFilters==NULL)
		{
			return AG_FAILURE;
		}

		bool first = true;
		pSubFilters->Reset();
		sql.append("(");
		while((pSubFilter=pSubFilters->Next())!=NULL)
		{
			subsql.clear();
			rc = BuildFilter(subsql, pFeatureClass, pSubFilter);
			if(rc==AG_SUCCESS)
			{
				if(first)
				{
					first = false;
				}
				else
				{
					sql.append(") ");
					sql.append(oper);
					sql.append(" (");
				}
				sql.append(subsql);
			}
		}
		sql.append(")");

		/*pFilter1 = pSubFilters->Next();
		pFilter2 = pSubFilters->Next();
		if(pFilter1==NULL||pFilter2==NULL)
		{
			pSubFilters->Release();
			return AG_FAILURE;
		}

		std::string strLeft;
		std::string strRight;
		sql.append("(");
		rc = BuildFilter(strLeft, pFeatureClass, pFilter1);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		sql.append(") ");
		sql.append(oper);
		sql.append(" (");
		rc = BuildFilter(strRight, pFeatureClass, pFilter2);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		sql.append(")");*/

		return rc;
	}

	//RESULTCODE SQLBuilder::BuildBinaryLogicFilter(std::string& sql,FeatureClassByd* pFeatureClass, BinaryLogicFilter *pFilter)
	//{
	//	RESULTCODE rc = AG_SUCCESS;
	//	GFilter* pFilter1 = NULL;
	//	GFilter* pFilter2 = NULL;
	//	EnumFilter* pSubFilters = pFilter->GetFilters();
	//	FilterFactory* pFactory = augeGetFilterFactoryInstance();

	//	if(pSubFilters==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	pSubFilters->Reset();
	//	pFilter1 = pSubFilters->Next();
	//	pFilter2 = pSubFilters->Next();
	//	if(pFilter1==NULL||pFilter2==NULL)
	//	{
	//		pSubFilters->Release();
	//		return AG_FAILURE;
	//	}

	//	std::string strLeft;
	//	std::string strRight;
	//	sql.append("(");
	//	rc = BuildFilter(strLeft, pFeatureClass, pFilter1);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		return rc;
	//	}
	//	sql.append(") ");
	//	sql.append(pFactory->AsString(pFilter->GetOperator()));
	//	sql.append(" (");
	//	rc = BuildFilter(strRight, pFeatureClass, pFilter2);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		return rc;
	//	}
	//	sql.append(")");

	//	return rc;
	//}

	RESULTCODE SQLBuilder::BuildUnaryLogicFilter(std::string& sql,FeatureClassByd* pFeatureClass, UnaryLogicFilter *pFilter)
	{
		RESULTCODE rc = AG_SUCCESS;
		GFilter* pSubFilter = pFilter->GetFilter();

		if(pSubFilter==NULL)
		{
			return AG_FAILURE;
		}

		std::string strSubFilter;

		sql.append("NOT (");
		rc = BuildFilter(sql, pFeatureClass, pSubFilter);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		sql.append(")");
		return rc;
	}

	RESULTCODE SQLBuilder::BuildSpatialFilter(std::string& sql,FeatureClassByd* pFeatureClass, SpatialFilter* pFilter)
	{
		RESULTCODE rc = AG_FAILURE;
		augeSpatialOperator	oper = pFilter->GetOperator();

		switch(oper)
		{
		case augeSpBBox:
			{
				rc = BuildBBoxFilter(sql, pFeatureClass, static_cast<BBoxFilter*>(pFilter));
			}
			break;
		case augeSpIntersects:
			{

			}
			break;
		case augeSpDWithin:
			{
				rc = BuildDWithinFilter(sql, pFeatureClass, static_cast<DistanceBufferFilter*>(pFilter));
			}
			break;
		case augeSpEquals:
			{

			}
			break;
		case augeSpDisjoint:
			{

			}
			break;
		case augeSpTouchs:
			{

			}
			break;
		case augeSpCrosses:
			{

			}
			break;
		case augeSpWithin:
			{
				rc = BuildWithinFilter(sql, pFeatureClass, static_cast<BinarySpatialFilter*>(pFilter));
			}
			break;
		case augeSpByond:
			{

			}
			break;
		case augeSpContains:
			{

			}
			break;
		case augeSpOverlaps:
			{

			}
			break;
		}

		return rc;
	}

	RESULTCODE SQLBuilder::BuildBBoxFilter(std::string& sql,FeatureClassByd* pFeatureClass, BBoxFilter* pFilter)
	{
		GEnvelope extent;
		pFilter->GetExtent(extent);
		char where[AUGE_BUFFER_MAX];
		//g_snprintf(where,AUGE_BUFFER_MAX," (%s && 'BOX3D(%.7f %.7f,%.7f %.7f)'::box3d)", pFeatureClass->m_geom_filed_name.c_str(),
		//	extent.m_m_xmin,
		//	extent.m_m_ymin,
		//	extent.m_m_xmax,
		//	extent.m_m_ymax);

		sql.append(where);

		//Expression *pPropertyName = NULL;
		//std::string strFieldName;
		//RESULTCODE rc = AG_FAILURE;

		//GEnvelope& extent = pFilter->GetExtent(extent);
		//Expression *pPropertyName = pFilter->GetPropertyName();
		//if(pPropertyName==NULL)
		//{
		//	 		//Field* pField = pTable->GetFeatureClassInfo()->GetDefaultGeometryField();
		//	 		//if(pField==NULL)
		//	 		//{
		//	 		//	return AG_FAILURE;
		//	 		//}
		//	 		//strFieldName = pField->GetName();
		//}
		//else
		//{
		//	rc = BuildExpression(strFieldName, pTable, pPropertyName);
		//	if(rc!=AG_SUCCESS)
		//	{
		//		return AG_FAILURE;
		//	}
		//}

		//char szWhereClause[AUGE_PATH_MAX];
		//ag_snprintf(szWhereClause, AUGE_PATH_MAX, " (%s%s%s && 'BOX3D(%f %f,%f %f)'::box3d)", 
		//	GEOPGS_DOUBLE_QUTOES, strFieldName.c_str(), GEOPGS_DOUBLE_QUTOES, 
		//	extent.m_xmin, 
		//	extent.m_ymin, 
		//	extent.m_xmax, 
		//	extent.m_ymax);

		//strWhere = szWhereClause;

		return AG_SUCCESS;
	}

	/*
	class BinarySpatialFilter : public SpatialFilter
	{
	public:
	BinarySpatialFilter(){}
	virtual ~BinarySpatialFilter(){}
	public:
	virtual		Expression*			GetPropertyName() = 0;
	virtual		bool				GetExtent(GEnvelope& extent) = 0;
	virtual		Geometry*			GetGeometry() = 0;
	virtual		augeSpatialOperator	GetOperator() = 0;
	};
	*/
	RESULTCODE SQLBuilder::BuildWithinFilter(std::string& sql,FeatureClassByd* pFeatureClass, BinarySpatialFilter* pFilter)
	{
		RESULTCODE rc = AG_SUCCESS;
		Geometry* pGeometry = pFilter->GetGeometry();
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}

		GField* pField = pFeatureClass->GetFields()->GetGeometryField();
		if(pField==NULL)
		{
			return AG_FAILURE;
		}

		const char* geo_field = pField->GetName();
		const char* wkt = pGeometry->AsText(true);

		char srid[AUGE_NAME_MAX] = {0};
		g_snprintf(srid, AUGE_NAME_MAX,"%d",pField->GetGeometryDef()->GetSRID());

		sql = "st_within(";
		sql.append("st_geomfromtext(upper('");
		sql.append(wkt);
		sql.append("'),");
		sql.append(srid);
		sql.append("),");
		sql.append(geo_field);
		sql.append(")");

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildDWithinFilter(std::string& sql,FeatureClassByd* pFeatureClass, DistanceBufferFilter* pFilter)
	{
		RESULTCODE rc = AG_SUCCESS;
		Geometry* pGeometry = pFilter->GetGeometry();
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}

		GField* pField = pFeatureClass->GetFields()->GetGeometryField();
		if(pField==NULL)
		{
			return AG_FAILURE;
		}

		const char* geo_field = pField->GetName();
		const char* wkt = pGeometry->AsText(true);

		char srid[AUGE_NAME_MAX] = {0};
		g_snprintf(srid, AUGE_BUFFER_MAX,"%d",pField->GetGeometryDef()->GetSRID());

		char distance[AUGE_NAME_MAX] = {0};
		g_snprintf(distance, AUGE_BUFFER_MAX,"%f",pFilter->GetDistance());
		//sboolean ST_DWithin(geometry g1, geometry g2, double precision distance_of_srid);

		sql = "st_dwithin(";
		sql.append("st_geomfromtext(upper('");
		sql.append(wkt);
		sql.append("'),");
		sql.append(srid);

		sql.append("),");
		sql.append(geo_field);
		sql.append(",");
		sql.append(distance);
		sql.append(")");

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildExpression(std::string &sql, FeatureClassByd* pFeatureClass, Expression* pExpression)
	{
		RESULTCODE rc = AG_FAILURE;
		augeExpressionType type = pExpression->GetType();

		switch(type)
		{
		case augeExpArithmetic:
			{
				rc = BuildArithmeticExpression(sql, pFeatureClass, static_cast<ArithmeticExpression*>(pExpression));
			}
			break;
		case augeExpPropertyName:
			{
				rc = BuildPropertyNameExpression(sql, pFeatureClass, static_cast<PropertyName*>(pExpression));
			}
			break;
		case augeExpLiteral:
			{
				rc = BuildLiteralExpression(sql, pFeatureClass, static_cast<Literal*>(pExpression));
			}
			break;
		case augeExpFunction:
			{
				rc = BuildFunctitonExpression(sql, pFeatureClass, static_cast<Function*>(pExpression));
			}
			break;
		}

		return rc;
	}

	RESULTCODE SQLBuilder::BuildArithmeticExpression(std::string &sql, FeatureClassByd* pFeatureClass, ArithmeticExpression* pExpression)
	{
		Expression* pLeft  = pExpression->GetLeft();
		Expression* pRight = pExpression->GetRight();

		if(pLeft==NULL||pRight==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		std::string strLeft, strRight;

		rc = BuildExpression(strLeft,  pFeatureClass, pLeft);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		rc = BuildExpression(strRight, pFeatureClass, pRight);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		sql.append(strLeft);
		sql.append(" ");
		sql.append(pFactory->AsString(pExpression->GetOperator()));
		sql.append(" ");
		sql.append(strRight);

		return rc;
	}

	RESULTCODE SQLBuilder::BuildPropertyNameExpression(std::string &sql, FeatureClassByd* pFeatureClass, PropertyName* pExpression)
	{
		if(pExpression==NULL)
		{
			return AG_FAILURE;
		}
		const char* fname = pExpression->GetName();
		if(fname==NULL)
		{
			return AG_FAILURE;
		}
		sql.append(fname);
		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildLiteralExpression(std::string &sql, FeatureClassByd* pFeatureClass, Literal* pExpression)
	{	
		GValue* pValue = pExpression->GetValue();
		if(pValue==NULL)
		{
			return AG_FAILURE;
		}
		char str[AUGE_BUFFER_MAX]; 
		augeValueType type = (augeValueType)pValue->GetType();
		switch(type)
		{					 
		case augeValueTypeShort:
			{
				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pValue->GetShort());
				sql.append(str);
			}
			break;
		case augeValueTypeInt:
			{
				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pValue->GetInt());
				sql.append(str);
			}
			break;
		case augeValueTypeLong:
			{
				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pValue->GetLong());
				sql.append(str);
			}
			break;
		case augeValueTypeInt64:
			{
				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pValue->GetInt64());
				sql.append(str);
			}
			break;
		case augeValueTypeFloat:
			{
				g_snprintf(str, AUGE_BUFFER_MAX,"%f",pValue->GetFloat());
				sql.append(str);
			}
			break;
		case augeValueTypeDouble:
			{
				g_snprintf(str, AUGE_BUFFER_MAX,"%f",pValue->GetDouble());
				sql.append(str);
			}
			break;
		case augeValueTypeChar:			 
			{
				g_snprintf(str, AUGE_BUFFER_MAX,"'%c'",pValue->GetChar());
				sql.append(str);
			}
			break;
		case augeValueTypeString:
			{
				sql.append("'");
				sql.append(pValue->GetString()==NULL?"":pValue->GetString());
				sql.append("'");
			}
			break;
		case augeValueTypeTime:	
			{
				TIME_STRU* tim = pValue->GetTime();
				sprintf(str,"%d-%02d-%02d %02d:%02d:%02d",tim->usYear,tim->usMonth,tim->usDay,tim->usHour,tim->usMinute,tim->usSecond);
				sql.append("'");
				sql.append(str);
				sql.append("'");
			}
			break;
		case augeValueTypeBool:			 
			{

			}
			break;
		case augeValueTypeBLOB:			 
			{

			}
			break;
		case augeValueTypeGeometry:
			{
				Geometry *pGeometry = pValue->GetGeometry();
				if(pGeometry!=NULL)
				{
					const char* wkt = pGeometry->AsText(true);
					if(wkt!=NULL)
					{	
						sql.append("st_geomfromtext(");
						sql.append("'");
						sql.append(wkt);
						sql.append("'");
						//sql.append("',");
						//sql.append(str);
						//sql.append(")");
					}
				}
			}
			break;
		}//switch

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildFunctitonExpression(std::string &sql, FeatureClassByd* pFeatureClass, Function* pExpression)
	{
		RESULTCODE rc = AG_FAILURE;
		return rc;
	}

	void SQLBuilder::BuildFields(std::string& fields, FeatureClassByd* pFeatureClass)
	{
		GField 	*pField = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		g_uint count = pFields->Count();
		const char* fname  = NULL;

		bool first = true;
		for(g_uint i=0; i<count; i++)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				fields.append(",");
			}

			pField = pFields->GetField(i);
			fname = pField->GetName();
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				fields.append("st_asbinary(");
				fields.append(fname);
				fields.append(") as ");
				fields.append(fname);
			}
			else
			{
				fields.append(fname);
			}
		}
	}

	void SQLBuilder::BuildFields(std::string& fields, GQuery* pQuery, FeatureClassByd* pFeatureClass)
	{	
		const char* fname = NULL;
		GField	*pField  = NULL;
		GFields *pFields = pFeatureClass->GetFields();
		int count = pQuery->GetSubFieldCount();

		bool hasid = false;
		bool first = true;
		for(g_uint i=0; i<count; i++)
		{
			if(first)
			{
				first = false;
			}
			else
			{
				fields.append(",");
			}

			fname = pQuery->GetSubField(i);
			if(!hasid)
			{
				if(strcmp(fname,"gid")==0)
				{
					hasid = true;
				}
			}

			pField = pFields->GetField(fname);
			if(pField==NULL)
			{
				fields.append(fname);
			}
			else
			{
				if(pField->GetType()==augeFieldTypeGeometry)
				{
					fields.append("st_asbinary(");
					fields.append(fname);
					fields.append(") as ");
					fields.append(fname);
				}
				else
				{
					fields.append(fname);
				}
			}
		}
		if(!hasid)
		{
			if(fields.empty())
			{
				fields = "gid";
			}
			else
			{
				fields.append(",gid");
			}
		}
	}

	RESULTCODE SQLBuilder::BuildCreateSequence(std::string& sql, const char* name)
	{
		sql = "create sequence seq_";
		sql.append(name);

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildCreateTable(std::string& sql, const char* name, GFields* pFields)
	{
		sql = "create table ";
		sql.append(name);
		sql.append("(");
		sql.append("fid integer with default next value for seq_");
		sql.append(name);
		sql.append(",");

		GField* pField = NULL;
		g_uint nfields = pFields->Count();

		for (int i = 0; i < nfields; ++i)
		{
			pField = pFields->GetField(i);
			if (pField != NULL)
			{
				const char* fname = pField->GetName();
				//std::string strFieldName = fname;
				//transform(strFieldName.begin(), strFieldName.end(), strFieldName.begin(), toupper);
				if (strcmp(fname,"fid")== 0)
				{
					continue;
				}
				std::string field_sql;
				RESULTCODE ret = BuildFieldSQL(field_sql,pField);
				if (ret == AG_SUCCESS)
				{
					sql.append(field_sql);

					if (i < nfields -1 )
					{
						sql.append(",");
					}
				}

			}
		}
		sql.append(")");

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildFieldSQL(std::string& sql, GField* pField)
	{
		sql.empty();

		if (pField == NULL)
		{
			return AG_FAILURE;
		}

		augeFieldType ftype = augeFieldTypeNone;
		const char* fname = NULL;
		long flength = -1;
		char szType[AUGE_NAME_MAX];
		memset(szType, 0, AUGE_NAME_MAX);

		fname = pField->GetName();
		ftype = pField->GetType();
		flength = pField->GetLength();

		switch(ftype)
		{
		case augeFieldTypeInt:
			sprintf(szType,"integer");
			break;
		case  augeFieldTypeChar:
			sprintf(szType,"char(1)");
			break;
		case  augeFieldTypeString:
			sprintf(szType,"varchar(%ld)",flength);
			break;
		case augeFieldTypeGeometry:
			sprintf(szType, "ST_Geometry");
			break;
		case  augeFieldTypeTime:
			sprintf(szType,"date");
			break;
		case  augeFieldTypeFloat:
			sprintf(szType,"float");
			break;
		case augeFieldTypeBool:
			sprintf(szType,"boolean");
			break;
		case augeFieldTypeDouble:
			sprintf(szType,"float8");
		default:
			break;
		}

		sql += fname;
		sql += " ";
		sql += szType;

		return AG_SUCCESS;
	}
}