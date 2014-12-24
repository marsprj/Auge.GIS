#ifndef __AUGE_FILTER_H__
#define __AUGE_FILTER_H__

#include "AugeCore.h"
#include "AugeGeometry.h"

#ifdef WIN32
#	ifdef AUGE_FILTER_EXPORTS
#		define AUGE_FILTER_API __declspec(dllexport)
#	else
#		define AUGE_FILTER_API __declspec(dllimport)
#	endif
#else
#	define AUGE_FILTER_API
#endif

namespace auge
{

/**
 * augeOrderType
 */
typedef enum augeOrderType
{
	augeOrderAsc	= 0,
	augeOrderDesc	= 1
}augeOrderType;


enum augeFilterType
{
	augeFilterFID							= 0,
	augeFilterComparison					= 1,
	augeFilterLogic							= 2,
	augeFilterSpatial						= 3

// 	augeFilterUnknown						= 0,
// 	augeFilterSQL							= 1,	// SQL查询
// 	augeFilterExtent						= 2,	// 矩形范围查询
// 	augeFilterIds							= 3,	// 根据fid查询
// 	augeFilterDistance						= 4,	// 距离查询
// 	augeFilterIdentical						= 5,
// 	augeFilterDWithin						= 6,
// 	augeFilterWithin						= 7,
// 	augeFilterContain						= 8,
// 	augeFilterIntersects					= 9,
// 	augeFilterDIntersects					= 10,
// 	augeFilterExpression					= 11,
// 
// 	augeFilterBinaryLogical					= 12,
// 	augeFilterBinaryComparison				= 13,
// 	augeFilterRectangle						= 14,	// 矩形范围查询
// 	augeFilterBinarySpatial					= 15,
// 
// 	augeFilterBetween,
// 	augeFilterIsNull,
// 	augeFilterLike,
// 	augeFilterPropertyName,
// 	augeFilterLiteral

	
};	

/**
 * 逻辑操作符
 */
typedef enum augeLogicalOperator
{
	augeLogOprAnd	= 0,
	augeLogOprOr	= 1,
	augeLogOprNot	= 2
}augeLogicalOperator;

/**
 * 算数操作符
 */
typedef enum augeComparisonOperator
{
	augeComOprEqual					= 0,	// 相等
	augeComOprNotEqual				= 1,	// 不等
	augeComOprLessThan				= 2,	// 小于
	augeComOprGreaterThan			= 3,	// 大于
	augeComOprLessThanOrEqalTo		= 4,	// 小于等于
	augeComOprGreaterThanOrEqalTo	= 5,	// 大于等于
	augeComOprIsLike				= 6,
	augecomOprIsNull				= 7,
	augeComOprIsBetween				= 8
}augeComparisonOperator;

/**
 * 空间操作符
 */
typedef enum augeSpatialOperator
{
	augeSpIntersects		= 0,
	augeSpDWithin			= 1,
	augeSpEquals			= 2,
	augeSpDisjoint			= 3,
	augeSpTouchs			= 4,
	augeSpCrosses			= 5,
	augeSpWithin			= 6,
	augeSpByond				= 7,
	augeSpContains			= 8,
	augeSpOverlaps			= 9,
	augeSpBBox				= 10
}augeSpatialOperator;

/**
 * 算术运算符
 */ 
typedef enum augeArithmeticOperator {
	
	augeAriOprAdd,
	augeAriOprSub,
	augeAriOprMul,
	augeAriOprDiv
}augeArithmeticOperator;

/**
 * 表达式类型
 */
typedef enum augeExpressionType
{
	augeExpArithmetic		= 0,
	augeExpPropertyName		= 1,
	augeExpLiteral			= 2,
	augeExpFunction			= 3
}augeExpressionType;


//========================================================================
// Filter
//========================================================================
class GOrderBy;
class GFilter;
class GExpression;
class GLiteral;
class GEnumExpression;
class GEnumFilter;
class GFilterFactory;

/*
 * GFilter
 */
class GFilter : public GObject
{
public:
	GFilter(){}
	virtual	~GFilter(){}
public:
	virtual		augeFilterType		GetType() = 0;
	virtual		void				Release() = 0;
};

//------------------------------------------------------------------------
/**
 * GComparisionFilter
 */
class GComparisionFilter : public GFilter
{
public:
	GComparisionFilter(){}
	virtual ~GComparisionFilter(){}
public:
	virtual		augeComparisonOperator	GetOperator() = 0;
};

/**
 * GLogicFilter
 */
class GLogicFilter : public GFilter
{
public:
	GLogicFilter(){}
	virtual ~GLogicFilter(){}
public:
	virtual		augeLogicalOperator	GetOperator() = 0;
};

/**
 * GSpatialFilter
 */
class GSpatialFilter : public GFilter
{
public:
	GSpatialFilter(){}
	virtual ~GSpatialFilter(){}
public:
	virtual		augeSpatialOperator	GetOperator() = 0;
};

//------------------------------------------------------------------------
/************************************************************************/
/* Comparison Filter Begin                                              */
/************************************************************************/
/**
 * GIsBetweenFilter
 */
class GIsBetweenFilter : public GComparisionFilter
{
public:
	GIsBetweenFilter(){}
	virtual ~GIsBetweenFilter(){}

public:	
	virtual		GExpression*		GetExpression()	= 0;
	virtual		bool				SetExpression(GExpression* pExpression)	= 0;

	virtual		GExpression*		GetLowerBound()	= 0;
	virtual		bool				SetLowerBound(GExpression* pLowerBound)	= 0;

	virtual		GExpression*		GetUpperBound()	= 0;
	virtual		bool				SetUpperBound(GExpression* pUpperBound)	= 0;
};

/**
 * GIsLikeFilter
 */
class GIsLikeFilter : public GComparisionFilter
{
public:
	GIsLikeFilter(){}
	virtual ~GIsLikeFilter(){}

public:	
	virtual		GExpression*		GetPropertyName()	= 0;
	virtual		GLiteral*			GetLiteral()	= 0;
	virtual		const char*			GetWildCard() = 0;
	virtual		const char*			GetSingleChar() = 0;
	virtual		const char*			GetEscapeChar() = 0;
};

/**
 * GIsNullFilter
 */
class GIsNullFilter : public GComparisionFilter
{
public:
	GIsNullFilter(){}
	virtual ~GIsNullFilter(){}

public:	
	virtual		GExpression*		GetPropertyName()	= 0;
};

class GBinaryComparisonFilter : public GComparisionFilter
{
public:
	GBinaryComparisonFilter(){}
	virtual ~GBinaryComparisonFilter(){}
public:
	virtual		augeComparisonOperator	GetOperator()	= 0;
	virtual		GExpression*			GetExpression1()	= 0;
	virtual		GExpression*			GetExpression2()	= 0;
	virtual		void					SetExpression1(GExpression* pExpression)	= 0;
	virtual		void					SetExpression2(GExpression* pExpression)	= 0;
	virtual		bool					IsMatchCase()		= 0;
};

/************************************************************************/
/* Comparison Filter End                                                */
/************************************************************************/

/************************************************************************/
/* Logic Filter      Begin                                              */
/************************************************************************/
/**
 * GBinaryLogicFilter
 */
class GBinaryLogicFilter : public GLogicFilter
{
public:
	GBinaryLogicFilter(){}
	virtual ~GBinaryLogicFilter(){}
public:
	virtual		void				SetOperator(augeLogicalOperator	oper) = 0;
	virtual		augeLogicalOperator	GetOperator() = 0;
	virtual		bool				AddFilter(GFilter* pFilter) = 0;
	virtual		GEnumFilter*		GetFilters() = 0;
};

/**
 * GUnaryLogicFilter
 */
class GUnaryLogicFilter :  public GLogicFilter
{
public:
	GUnaryLogicFilter(){}
	virtual ~GUnaryLogicFilter(){}
public:
	virtual		GFilter*			GetFilter() = 0;
};

/************************************************************************/
/* Logic Filter      End                                                */
/************************************************************************/

/**
 * GIDLogicFilter
 */
class GIDFilter : public GFilter
{
public:
	GIDFilter(){}
	virtual ~GIDFilter(){}
public:
	virtual		void				AddID(int id) = 0;
//	virtual		GEnumID*			GetIDs() = 0;
	virtual		augeComparisonOperator	GetOperator() = 0;
	virtual		void				SetOperator(augeComparisonOperator oper) = 0;
};

/************************************************************************/
/* Spatial Filter   Begin                                               */
/************************************************************************/

/**
 * BBoxFilter
 */
class GBBoxFilter : public GSpatialFilter
{
public:
	GBBoxFilter(){}
	virtual ~GBBoxFilter(){}
public:
	
	virtual		GExpression*		GetPropertyName() = 0;
	virtual		bool				SetPropertyName(GExpression* pPropertyName) = 0;

	virtual		bool				GetExtent(GEnvelope& extent) = 0;
	virtual		void				SetExtent(GEnvelope& extent) = 0;
	virtual		void				SetExtent(double xmin, double ymin, double xmax, double ymax) = 0;
};

/**
 * GBinarySpatialFilter
 */
class GBinarySpatialFilter : public GSpatialFilter
{
public:
	GBinarySpatialFilter(){}
	virtual ~GBinarySpatialFilter(){}
public:
	virtual		GExpression*		GetPropertyName() = 0;
	virtual		bool				GetExtent(GEnvelope& extent) = 0;
	virtual		Geometry*			GetGeometry() = 0;
	virtual		augeSpatialOperator	GetOperator() = 0;
};

class GDistanceBufferFilter : public GSpatialFilter
{
public:
	GDistanceBufferFilter(){}
	virtual ~GDistanceBufferFilter(){}
public:
	virtual		GExpression*		GetPropertyName() = 0;
	virtual		bool				GetExtent(GEnvelope& extent) = 0;
	virtual		Geometry*			GetGeometry() = 0;

	virtual		double				GetDistance() = 0;
	virtual		augeUnitType		GetUnit() = 0;
};

/************************************************************************/
/* Spatial Filter   End                                                 */
/************************************************************************/

//------------------------------------------------------------------------
/**
 * GExpression
 */
class GExpression : public GObject
{
public:
	GExpression(){}
	virtual ~GExpression(){}
public:
	virtual		augeExpressionType	GetType() = 0;
	virtual		void				Release() = 0;
};

/**
 * GLiteral
 */
class GLiteral : public GExpression
{
public:
	GLiteral(){}
	virtual ~GLiteral(){}
public:
	virtual		bool				SetValue(GValue* pValue) = 0;
	virtual		GValue*				GetValue() = 0;
};

/**
 * GFunction
 */
class GFunction : public GExpression
{
public:
	GFunction(){}
	virtual ~GFunction(){}
public:
	virtual		bool				AddExpression(GExpression* pExpression) = 0;
	virtual		GEnumExpression*	GetExpressions() = 0;
	virtual		const char*			GetName() = 0;
};

class GPropertyName : public GExpression
{
public:
	GPropertyName(){}
	virtual ~GPropertyName(){}
public:
	virtual		const char*			GetName() = 0;
};

class GArithmeticExpression : public GExpression
{
public:
	GArithmeticExpression(){}
	virtual ~GArithmeticExpression(){}
public:
	virtual		augeArithmeticOperator	GetOperator() = 0;
	virtual		GExpression*			GetLeft() = 0;
	virtual		GExpression*			GetRight() = 0;
};

//------------------------------------------------------------------------

class GEnumExpression : public GObject
{
public:
	GEnumExpression(){}
	virtual ~GEnumExpression(){}
public:
	virtual		GExpression*		Next() = 0;
	virtual		void				Reset() = 0;
	virtual		void				Release() = 0;
};

class GEnumFilter : public GObject
{
public:
	GEnumFilter(){}
	virtual ~GEnumFilter(){}
public:
	virtual		GFilter*			Next() = 0;
	virtual		void				Reset() = 0;
	virtual		void				Release() = 0;
};

//------------------------------------------------------------------------

/**
 * Query Class: complete sql statement
 */
class GQuery
{
protected:
	GQuery(){}
	virtual ~GQuery(){}
public:
    //--------------------------------------------------------------------------
    virtual const char*       GetFeatureClassName() const = 0;
    virtual       RESULTCODE  SetFeatureClassName(const char* szClassName) = 0;
    //--------------------------------------------------------------------------
	// SubField
    virtual       int         GetSubFieldCount() const = 0;
    virtual const char*       GetSubField(const int i) const = 0;
    virtual       RESULTCODE  AddSubField(const char* szName) = 0;
    virtual       RESULTCODE  RemoveSubField(const char* szName) = 0;
	virtual       bool		  HasSubField(const char* szName) = 0;
	virtual		  bool		  IsStarFields() = 0;

	// Filter
    virtual       bool        SetFilter(GFilter* pFilter) = 0;     
    virtual const GFilter*   GetFilter() const = 0;

	// Order
	virtual		  RESULTCODE  SetOrderBy(GOrderBy* pOrderBy) = 0;
	virtual		  GOrderBy*  GetOrderBy() = 0;

	// SetSQL
	virtual bool			  SetSQL(const char* szsql) = 0;
    virtual const char*       GetSQL() const = 0;

    virtual       void        Release() = 0;
};

//------------------------------------------------------------------------

/**
 * GOrderBy
 *
 * Query 排序
 */
class GOrderBy : public GObject
{
protected:
	GOrderBy(){}
	virtual ~GOrderBy(){}
public:
	virtual	RESULTCODE			AddField(const char* szFieldName) = 0;
	virtual const char*			GetField(g_uint i) = 0;
	virtual	g_uint				GetFieldCount() = 0;
	virtual void				Clear() = 0;

	virtual void				SetOrder(augeOrderType order) = 0;
	virtual augeOrderType		GetOrder() = 0;

	virtual void				Release() = 0;
};


//------------------------------------------------------------------------
class FilterFactory
{
protected:
	FilterFactory(){}
	virtual ~FilterFactory(){}
public:
	//---------------------------------------------------------------
	// Expression
	virtual	GLiteral*				CreateLiteral() = 0;
	virtual	GLiteral*				CreateLiteral(GValue* pValue) = 0;
	virtual GPropertyName*			CreatePropertyName(const char* szName) = 0;

	//---------------------------------------------------------------
	// Filter
	virtual	GBBoxFilter*			CreateBBoxFilter() = 0;
	virtual	GBBoxFilter*			CreateBBoxFilter(GExpression* pPropertyName, GEnvelope& bbox) = 0;

	virtual GIDFilter*				CreateIDFilter() = 0;

	virtual GIsBetweenFilter*		CreateIsBetweenFilter() = 0;
	virtual GIsBetweenFilter*		CreateIsBetweenFilter(GExpression* pPropertyName,
														  GExpression* pLowerBound,
														  GExpression* pUpperBound) = 0;

	virtual GBinaryComparisonFilter*	CreateBinaryComparisonFilter() = 0;
	virtual GBinaryLogicFilter*			CreateBinaryLogicFilter() = 0;

	virtual GQuery*					CreateQuery() = 0;
};


extern "C"
{
	AUGE_FILTER_API FilterFactory*		augeGetFilterFactoryInstance();
}

}

//==============================================================================
//------------------------------------------------------------------------------
// 连接所需动态库
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_FILTER_EXPORTS
#		define _AUGE_FILTER_NAME "GFilter"
#		if defined(_DEBUG)
#			if defined(_UNICODE)
//#				define _AUGE_FILTER_CONFIGURATION "UD"
#				define _AUGE_FILTER_CONFIGURATION "D"
#			else
#				define _AUGE_FILTER_CONFIGURATION "D"
#			endif // _UNICODE
#		else
#			if defined(_UNICODE)
#				define _AUGE_FILTER_CONFIGURATION "U"
#			else
#				define _AUGE_FILTER_CONFIGURATION ""
#			endif // _UNICODE
#		endif // _DEBUG
#		pragma comment(lib, _AUGE_FILTER_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " _AUGE_FILTER_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef _AUGE_FILTER_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================

#endif //__AUGE_FILTER_H__



