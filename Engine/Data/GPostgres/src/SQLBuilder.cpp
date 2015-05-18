#include "SQLBuilder.h"
#include "FeatureClassPgs.h"

#include "AugeFilter.h"


namespace auge
{
	void SQLBuilder::BuildCreateTable(std::string& sql, const char* name, GFields* pFields)
	{
		char str[AUGE_NAME_MAX];

		sql = "create table ";
		sql.append(name);
		sql.append("(gid serial");

		g_int fcount = pFields->Count();
		GField* pField = NULL;
		for(g_int i=0; i<fcount; i++)
		{
			pField = pFields->GetField(i);
			
			if(!g_stricmp(pField->GetName(), "gid"))
			{
				continue;
			}
			if(pField->GetType()==augeFieldTypeGeometry)
			{
				continue;
			}

			sql.append(",");
			sql.append(pField->GetName());
			sql.append(" ");
			if(pField->GetType()==augeFieldTypeString)
			{
				g_snprintf(str, AUGE_NAME_MAX, "%s(%d)", GetPgFieldType(pField->GetType()), pField->GetLength());
				sql.append(str);
			}
			else
			{
				sql.append(GetPgFieldType(pField->GetType()));
			}
		}

		sql.append(")");
	}

	void SQLBuilder::BuildQuery(std::string& sql, FeatureClass* pFeatureClass)
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
		sql.append(pFeatureClass->GetName());
	}

	void SQLBuilder::BuildQuery(std::string& sql, GEnvelope& extent, FeatureClassPgs* pFeatureClass)
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
		sql.append(pFeatureClass->GetName());

		char where[AUGE_BUFFER_MAX];
		g_snprintf(where,AUGE_BUFFER_MAX," where (%s && 'BOX3D(%.7f %.7f,%.7f %.7f)'::box3d)", pFeatureClass->m_geom_filed_name.c_str(),
			extent.m_xmin,
			extent.m_ymin,
			extent.m_xmax,
			extent.m_ymax);

		sql.append(where);
	}

	void SQLBuilder::BuildQuery(std::string& sql, GFilter* pFilter, FeatureClassPgs* pFeatureClass)
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
		sql.append(pFeatureClass->GetName());

		if(pFilter!=NULL)
		{
			sql.append(" where ");
			std::string where = "";
			BuildFilter(where, pFeatureClass, pFilter);
			sql.append(where);
		}
	}

	void SQLBuilder::BuildQuery(std::string& sql, GQuery* pQuery, FeatureClassPgs* pFeatureClass)
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
		sql.append(fields);
		sql.append(" from ");
		sql.append(pFeatureClass->GetName());

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

		if(pQuery->GetMaxFeatures()>0)
		{
			char str[AUGE_NAME_MAX];
			g_sprintf(str, " limit %d", pQuery->GetMaxFeatures());
			sql.append(str);
		}

		if(pQuery->GetOffset()>0)
		{
			char str[AUGE_NAME_MAX];
			g_sprintf(str, " offset %d", pQuery->GetOffset());
			sql.append(str);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void SQLBuilder::BuildCount(std::string& sql, FeatureClassPgs* pFeatureClass)
	{
		sql = "select count(*)";
		sql.append(" from ");
		sql.append(pFeatureClass->GetName());
	}

	void SQLBuilder::BuildCount(std::string& sql, GEnvelope& extent, FeatureClassPgs* pFeatureClass)
	{
		sql = "select count(*)";
		sql.append(" from ");
		sql.append(pFeatureClass->GetName());

		char where[AUGE_BUFFER_MAX];
		g_snprintf(where,AUGE_BUFFER_MAX," where (%s && 'BOX3D(%.7f %.7f,%.7f %.7f)'::box3d)", pFeatureClass->m_geom_filed_name.c_str(),
			extent.m_xmin,
			extent.m_ymin,
			extent.m_xmax,
			extent.m_ymax);

		sql.append(where);
	}

	void SQLBuilder::BuildCount(std::string& sql, GFilter* pFilter, FeatureClassPgs* pFeatureClass)
	{
		sql = "select count(*)";
		sql.append(" from ");

		sql.append(" from ");
		sql.append(pFeatureClass->GetName());

		if(pFilter!=NULL)
		{
			sql.append(" where ");
			std::string where = "";
			BuildFilter(where, pFeatureClass, pFilter);
			sql.append(where);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	void SQLBuilder::BuildFields(std::string& fields, FeatureClassPgs* pFeatureClass)
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

	void SQLBuilder::BuildFields(std::string& fields, GQuery* pQuery, FeatureClassPgs* pFeatureClass)
	{	
		const char* fname = NULL;
		GField	*pField  = NULL;
		GFields *pFields = pFeatureClass->GetFields();
		int count = pQuery->GetSubFieldCount();

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
	}

	RESULTCODE SQLBuilder::BuildFilter(std::string& sql, FeatureClassPgs* pFeatureClass, GFilter* pFilter)
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
				BuildComparisioFilter(sql, pFeatureClass, static_cast<ComparisionFilter*>(pFilter));
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

	RESULTCODE SQLBuilder::BuildIDFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IDFilter* pFilter)
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

	RESULTCODE SQLBuilder::BuildComparisioFilter(std::string& sql,FeatureClassPgs* pFeatureClass, ComparisionFilter* pFilter)
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

	RESULTCODE SQLBuilder::BuildBinaryComparisonFilter(std::string& sql, FeatureClassPgs* pFeatureClass, BinaryComparisonFilter* pFilter)
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

	RESULTCODE SQLBuilder::BuildIsLikeFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IsLikeFilter *pFilter)
	{
		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildIsNullFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IsNullFilter *pFilter)
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

	RESULTCODE SQLBuilder::BuildIsBwtweenFilter(std::string& sql,FeatureClassPgs* pFeatureClass, IsBetweenFilter *pFilter)
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
		sql.append(" BWTWEEN (");
		sql.append(strLower);
		sql.append(") AND (");
		sql.append(strUpper);
		sql.append(")");
		return AG_SUCCESS;
	}
	
	RESULTCODE SQLBuilder::BuildLogicFilter(std::string& sql,FeatureClassPgs* pFeatureClass, LogicFilter* pFilter)
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

	RESULTCODE SQLBuilder::BuildBinaryLogicFilter(std::string& sql,FeatureClassPgs* pFeatureClass, BinaryLogicFilter *pFilter)
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

	//RESULTCODE SQLBuilder::BuildBinaryLogicFilter(std::string& sql,FeatureClassPgs* pFeatureClass, BinaryLogicFilter *pFilter)
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

	RESULTCODE SQLBuilder::BuildUnaryLogicFilter(std::string& sql,FeatureClassPgs* pFeatureClass, UnaryLogicFilter *pFilter)
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

	RESULTCODE SQLBuilder::BuildSpatialFilter(std::string& sql,FeatureClassPgs* pFeatureClass, SpatialFilter* pFilter)
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

	RESULTCODE SQLBuilder::BuildBBoxFilter(std::string& sql,FeatureClassPgs* pFeatureClass, BBoxFilter* pFilter)
	{
		GEnvelope extent;
		pFilter->GetExtent(extent);
		char where[AUGE_BUFFER_MAX];
		g_snprintf(where,AUGE_BUFFER_MAX," (%s && 'BOX3D(%.7f %.7f,%.7f %.7f)'::box3d)", pFeatureClass->m_geom_filed_name.c_str(),
			extent.m_xmin,
			extent.m_ymin,
			extent.m_xmax,
			extent.m_ymax);

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
		//	extent.xmin, 
		//	extent.ymin, 
		//	extent.xmax, 
		//	extent.ymax);

		//strWhere = szWhereClause;

		return AG_SUCCESS;
	}

	RESULTCODE SQLBuilder::BuildExpression(std::string &sql, FeatureClassPgs* pFeatureClass, Expression* pExpression)
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

	RESULTCODE SQLBuilder::BuildArithmeticExpression(std::string &sql, FeatureClassPgs* pFeatureClass, ArithmeticExpression* pExpression)
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

	RESULTCODE SQLBuilder::BuildPropertyNameExpression(std::string &sql, FeatureClassPgs* pFeatureClass, PropertyName* pExpression)
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

	RESULTCODE SQLBuilder::BuildLiteralExpression(std::string &sql, FeatureClassPgs* pFeatureClass, Literal* pExpression)
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
				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pValue->GetDouble());
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

	RESULTCODE SQLBuilder::BuildFunctitonExpression(std::string &sql, FeatureClassPgs* pFeatureClass, Function* pExpression)
	{
		RESULTCODE rc = AG_FAILURE;
		return rc;
	}

	void SQLBuilder::BuildDeleteFeature(std::string& sql, GFilter* pFilter, FeatureClassPgs* pFeatureClass)
	{
		sql = "delete ";		
		sql.append(" from ");
		sql.append(pFeatureClass->GetName());

		if(pFilter!=NULL)
		{
			sql.append(" where ");
			std::string where = "";
			BuildFilter(where, pFeatureClass, pFilter);
			sql.append(where);
		}
	}

	void SQLBuilder::BuildInsertFeature(std::string& sql, const char* className, Feature* pFeature, g_uint srid)
	{
		std::string fields = "";
		std::string values = "";

		GField	*pField = NULL;
		GFields	*pFields= pFeature->GetFeatureClass()->GetFields();
		GValue	*pValue = NULL;
		g_uint   fcount = pFields->Count();
		const char* fname= NULL;
		augeFieldType type = augeFieldTypeNone;
		char str[AUGE_BUFFER_MAX]; 

		bool first = true;		
		for(g_uint i=0; i<fcount; i++)
		{
			pField = pFields->GetField(i);
			fname = pField->GetName();

			if(!g_stricmp(fname, "gid"))
			{
				continue;
			}

			pValue = pFeature->GetValue(i);
			if(pValue==NULL)
			{
				continue;
			}

			if(first)
			{
				first = false;
			}
			else
			{
				fields.append(",");
				values.append(",");
			}

			type = pField->GetType();
			switch(type)
			{					 
			case augeFieldTypeShort:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetShort(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeInt:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeLong:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetLong(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeInt64:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt64(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeFloat:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%f",pFeature->GetFloat(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeDouble:
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetDouble(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeChar:			 
				{
					g_snprintf(str, AUGE_BUFFER_MAX,"'%c'",pFeature->GetChar(i));
					fields.append(fname);
					values.append(str);
				}
				break;
			case augeFieldTypeString:
				{
					fields.append(fname);
					values.append("'");
					values.append(pFeature->GetString(i));
					values.append("'");
				}
				break;
			case augeFieldTypeTime:	
				{

				}
				break;
			case augeFieldTypeBool:			 
				{

				}
				break;
			case augeFieldTypeBLOB:			 
				{

				}
				break;
			case augeFieldTypeGeometry:
				{
					Geometry *pGeometry = pFeature->GetGeometry();
					if(pGeometry!=NULL)
					{
						const char* wkt = pGeometry->AsText(true);
						if(wkt!=NULL)
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%d",srid);

							fields.append(fname);
							values.append("st_geomfromtext(");
							values.append("'");
							values.append(wkt);
							values.append("',");
							values.append(str);
							values.append(")");
						}
					}
				}
				break;
			}//switch

			pValue->Release();
		}

		// construct sdesql
		sql.append("insert into ");
		sql.append(className);
		sql.append(" (");
		sql.append(fields);
		sql.append(") values (");
		sql.append(values);
		sql.append(")");
	}

	//void SQLBuilder::BuildInsertFeature(std::string& sql, const char* className, Feature* pFeature, g_uint srid)
	//{
	//	std::string fields = "";
	//	std::string values = "";
	//	
	//	GField	*pField = NULL;
	//	GFields	*pFields= pFeature->GetFeatureClass()->GetFields();
	//	g_uint   fcount = pFields->Count();
	//	const char* fname= NULL;
	//	augeFieldType type = augeFieldTypeNone;
	//	char str[AUGE_BUFFER_MAX]; 

	//	bool first = true;		
	//	for(g_uint i=0; i<fcount; i++)
	//	{
	//		pField = pFields->GetField(i);
	//		fname = pField->GetName();

	//		if(!g_stricmp(fname, "gid"))
	//		{
	//			continue;
	//		}


	//		if(first)
	//		{
	//			first = false;
	//		}
	//		else
	//		{
	//			fields.append(",");
	//			values.append(",");
	//		}

	//		type = pField->GetType();
	//		switch(type)
	//		{					 
	//		case augeFieldTypeShort:
	//			{
	//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetShort(i));
	//				fields.append(fname);
	//				values.append(str);
	//			}
	//			break;
	//		case augeFieldTypeInt:
	//			{
	//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt(i));
	//				fields.append(fname);
	//				values.append(str);
	//			}
	//			break;
	//		case augeFieldTypeLong:
	//			{
	//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetLong(i));
	//				fields.append(fname);
	//				values.append(str);
	//			}
	//			break;
	//		case augeFieldTypeInt64:
	//			{
	//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt64(i));
	//				fields.append(fname);
	//				values.append(str);
	//			}
	//			break;
	//		case augeFieldTypeFloat:
	//			{
	//				g_snprintf(str, AUGE_BUFFER_MAX,"%f",pFeature->GetFloat(i));
	//				fields.append(fname);
	//				values.append(str);
	//			}
	//			break;
	//		case augeFieldTypeDouble:
	//			{
	//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetDouble(i));
	//				fields.append(fname);
	//				values.append(str);
	//			}
	//			break;
	//		case augeFieldTypeChar:			 
	//			{
	//				g_snprintf(str, AUGE_BUFFER_MAX,"'%c'",pFeature->GetChar(i));
	//				fields.append(fname);
	//				values.append(str);
	//			}
	//			break;
	//		case augeFieldTypeString:
	//			{
	//				fields.append(fname);
	//				values.append("'");
	//				values.append(pFeature->GetString(i));
	//				values.append("'");
	//			}
	//			break;
	//		case augeFieldTypeTime:	
	//			{
	//				
	//			}
	//			break;
	//		case augeFieldTypeBool:			 
	//			{

	//			}
	//			break;
	//		case augeFieldTypeBLOB:			 
	//			{
	//				
	//			}
	//			break;
	//		case augeFieldTypeGeometry:
	//			{
	//				Geometry *pGeometry = pFeature->GetGeometry();
	//				if(pGeometry!=NULL)
	//				{
	//					const char* wkt = pGeometry->AsText(true);
	//					if(wkt!=NULL)
	//					{
	//						g_snprintf(str, AUGE_BUFFER_MAX,"%d",srid);

	//						fields.append(fname);
	//						values.append("st_geomfromtext(");
	//						values.append("'");
	//						values.append(wkt);
	//						values.append("',");
	//						values.append(str);
	//						values.append(")");
	//					}
	//				}
	//			}
	//			break;
	//		}//switch
	//	}

	//	// construct sdesql
	//	sql.append("insert into ");
	//	sql.append(className);
	//	sql.append(" (");
	//	sql.append(fields);
	//	sql.append(") VALUES (");
	//	sql.append(values);
	//	sql.append(")");
	//}

	const char* SQLBuilder::GetPgFieldType(augeFieldType type)
	{
		switch(type)
		{
		case augeFieldTypeBool:
			return "boolean";
		case augeFieldTypeChar:
			return "";
		case augeFieldTypeShort:
			return "smallint";
		case augeFieldTypeInt:
			return "integer";
		case augeFieldTypeLong:
			return "integer";
		case augeFieldTypeInt64:
			return "bigint";
		case augeFieldTypeFloat:
			return "double precision";
		case augeFieldTypeDouble:
			return "double precision";
		case augeFieldTypeString:
			return "character varying";
		case augeFieldTypeTime:
			return "date";
		case augeFieldTypeBLOB:
			return "blob";
		case augeFieldTypeGeometry:
			return "geometry";
		case augeFieldTypeCLOB:
			return "clob";
		case augeFieldTypeNCLOB:
			return "blob";
		case augeFieldTypeSerial:
			return "serial";
		}
		return "";
	}

	RESULTCODE SQLBuilder::BuildGetUniqueValueSQL(std::string& sql, const char* field_name, FeatureClassPgs* pFeatureClass, augeOrderType order)
	{
		sql = "select distinct ";
		sql.append(field_name);
		sql.append(" as ");
		sql.append(field_name);
		sql.append(" from ");
		sql.append(pFeatureClass->GetName());
		sql.append(" order by ");
		sql.append(field_name);
		sql.append(order==augeOrderAsc?" asc":" desc");
		return AG_SUCCESS;
	}
}