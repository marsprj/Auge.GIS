#ifndef __AUGE_FILTER_H__
#define __AUGE_FILTER_H__

#include "AugeCore.h"
#include "AugeGeometry.h"
#include "AugeXML.h"

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
		augeComOprLessThanOrEqualTo		= 4,	// 小于等于
		augeComOprGreaterThanOrEqualTo	= 5,	// 大于等于
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
	class OrderBy;
	class GFilter;
	class Expression;
	class Literal;
	class EnumExpression;
	class EnumFilter;
	class FilterFactory;
	class GFields;

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
	 * ComparisionFilter
	 */
	class ComparisionFilter : public GFilter
	{
	public:
		ComparisionFilter(){}
		virtual ~ComparisionFilter(){}
	public:
		virtual	augeComparisonOperator	GetOperator() = 0;
		virtual	void					SetOperator(augeComparisonOperator oper) = 0;
	};

	/**
	 * GLogicFilter
	 */
	class LogicFilter : public GFilter
	{
	public:
		LogicFilter(){}
		virtual ~LogicFilter(){}
	public:
		virtual		augeLogicalOperator	GetOperator() = 0;
	};

	/**
	 * SpatialFilter
	 */
	class SpatialFilter : public GFilter
	{
	public:
		SpatialFilter(){}
		virtual ~SpatialFilter(){}
	public:
		virtual		augeSpatialOperator	GetOperator() = 0;
	};

	//------------------------------------------------------------------------
	/************************************************************************/
	/* Comparison Filter Begin                                              */
	/************************************************************************/
	/**
	 * IsBetweenFilter
	 */
	class IsBetweenFilter : public ComparisionFilter
	{
	public:
		IsBetweenFilter(){}
		virtual ~IsBetweenFilter(){}

	public:	
		virtual		Expression*			GetExpression()	= 0;
		virtual		bool				SetExpression(Expression* pExpression)	= 0;

		virtual		Expression*			GetLowerBound()	= 0;
		virtual		bool				SetLowerBound(Expression* pLowerBound)	= 0;

		virtual		Expression*			GetUpperBound()	= 0;
		virtual		bool				SetUpperBound(Expression* pUpperBound)	= 0;
	};

	/**
	 * IsLikeFilter
	 */
	class IsLikeFilter : public ComparisionFilter
	{
	public:
		IsLikeFilter(){}
		virtual ~IsLikeFilter(){}

	public:	
		virtual		Expression*			GetPropertyName()	= 0;
		virtual		Literal*			GetLiteral()	= 0;
		virtual		const char*			GetWildCard() = 0;
		virtual		const char*			GetSingleChar() = 0;
		virtual		const char*			GetEscapeChar() = 0;
	};

	/**
	 * IsNullFilter
	 */
	class IsNullFilter : public ComparisionFilter
	{
	public:
		IsNullFilter(){}
		virtual ~IsNullFilter(){}

	public:	
		virtual		Expression*		GetPropertyName()	= 0;
	};

	class BinaryComparisonFilter : public ComparisionFilter
	{
	public:
		BinaryComparisonFilter(){}
		virtual ~BinaryComparisonFilter(){}
	public:
		virtual		augeComparisonOperator	GetOperator()	= 0;
		virtual		Expression*				GetExpression1()	= 0;
		virtual		Expression*				GetExpression2()	= 0;
		virtual		void					SetExpression1(Expression* pExpression)	= 0;
		virtual		void					SetExpression2(Expression* pExpression)	= 0;
		virtual		bool					IsMatchCase()		= 0;
	};

	/************************************************************************/
	/* Comparison Filter End                                                */
	/************************************************************************/

	/************************************************************************/
	/* Logic Filter      Begin                                              */
	/************************************************************************/
	/**
	 * BinaryLogicFilter
	 */
	class BinaryLogicFilter : public LogicFilter
	{
	public:
		BinaryLogicFilter(){}
		virtual ~BinaryLogicFilter(){}
	public:
		virtual		void				SetOperator(augeLogicalOperator	oper) = 0;
		virtual		augeLogicalOperator	GetOperator() = 0;
		virtual		bool				AddFilter(GFilter* pFilter) = 0;
		virtual		EnumFilter*			GetFilters() = 0;
	};

	/**
	 * UnaryLogicFilter
	 */
	class UnaryLogicFilter :  public LogicFilter
	{
	public:
		UnaryLogicFilter(){}
		virtual ~UnaryLogicFilter(){}
	public:
		virtual augeFilterType			GetType() = 0;
		virtual		GFilter*			GetFilter() = 0;
		virtual		void				SetFilter(GFilter* pFilter) = 0;
	};

	/************************************************************************/
	/* Logic Filter      End                                                */
	/************************************************************************/

	/**
	 * GIDLogicFilter
	 */
	class IDFilter : public GFilter
	{
	public:
		IDFilter(){}
		virtual ~IDFilter(){}
	public:
		virtual		void					Add(g_uint id) = 0;
		virtual		g_uint					GetID(g_uint i) = 0;
		virtual		g_uint					Count() = 0;
		virtual		augeComparisonOperator	GetOperator() = 0;
		virtual		void					SetOperator(augeComparisonOperator oper) = 0;
	};

	/************************************************************************/
	/* Spatial Filter   Begin                                               */
	/************************************************************************/

	/**
	 * BBoxFilter
	 */
	class BBoxFilter : public SpatialFilter
	{
	public:
		BBoxFilter(){}
		virtual ~BBoxFilter(){}
	public:
	
		virtual		Expression*			GetPropertyName() = 0;
		virtual		bool				SetPropertyName(Expression* pPropertyName) = 0;

		virtual		bool				GetExtent(GEnvelope& extent) = 0;
		virtual		void				SetExtent(GEnvelope& extent) = 0;
		virtual		void				SetExtent(double xmin, double ymin, double xmax, double ymax) = 0;
	};

	/**
	 * BinarySpatialFilter
	 */
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

	class DistanceBufferFilter : public SpatialFilter
	{
	public:
		DistanceBufferFilter(){}
		virtual ~DistanceBufferFilter(){}
	public:
		virtual		Expression*			GetPropertyName() = 0;
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
	 * Expression
	 */
	class Expression : public GObject
	{
	public:
		Expression(){}
		virtual ~Expression(){}
	public:
		virtual		augeExpressionType	GetType() = 0;
		virtual		void				Release() = 0;
	};

	/**
	 * Literal
	 */
	class Literal : public Expression
	{
	public:
		Literal(){}
		virtual ~Literal(){}
	public:
		virtual		bool				SetValue(GValue* pValue) = 0;
		virtual		GValue*				GetValue() = 0;
	};

	/**
	 * Function
	 */
	class Function : public Expression
	{
	public:
		Function(){}
		virtual ~Function(){}
	public:
		virtual		bool				AddExpression(Expression* pExpression) = 0;
		virtual		EnumExpression*		GetExpressions() = 0;
		virtual		const char*			GetName() = 0;
	};

	class PropertyName : public Expression
	{
	public:
		PropertyName(){}
		virtual ~PropertyName(){}
	public:
		virtual		const char*			GetName() = 0;
		virtual		void				SetName(const char* name) = 0;
	};

	class ArithmeticExpression : public Expression
	{
	public:
		ArithmeticExpression(){}
		virtual ~ArithmeticExpression(){}
	public:
		virtual		augeArithmeticOperator	GetOperator() = 0;
		virtual		Expression*			GetLeft() = 0;
		virtual		Expression*			GetRight() = 0;
	};

	//------------------------------------------------------------------------

	class EnumExpression : public GObject
	{
	public:
		EnumExpression(){}
		virtual ~EnumExpression(){}
	public:
		virtual		Expression*			Next() = 0;
		virtual		void				Reset() = 0;
		virtual		void				Release() = 0;
	};

	class EnumFilter : public GObject
	{
	public:
		EnumFilter(){}
		virtual ~EnumFilter(){}
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
		virtual const char*			GetFeatureClassName() const = 0;
		virtual       RESULTCODE	SetFeatureClassName(const char* szClassName) = 0;
		//--------------------------------------------------------------------------
		// SubField
		virtual       int			GetSubFieldCount() const = 0;
		virtual const char*			GetSubField(const int i) const = 0;
		virtual       RESULTCODE	AddSubField(const char* szName) = 0;
		virtual       RESULTCODE	RemoveSubField(const char* szName) = 0;
		virtual       bool			HasSubField(const char* szName) = 0;
		virtual		  bool			IsStarFields() = 0;

		// Filter
		virtual       bool			SetFilter(GFilter* pFilter) = 0;     
		virtual const GFilter*		GetFilter() const = 0;

		// Order
		virtual		  RESULTCODE	SetOrderBy(OrderBy* pOrderBy) = 0;
		virtual		  OrderBy*		GetOrderBy() = 0;

		virtual		  void			SetMaxFeatures(g_int maxFeatures) = 0;
		virtual		  g_int			GetMaxFeatures() = 0;

		virtual		  void			SetOffset(g_int offset) = 0;
		virtual		  g_int			GetOffset() = 0;

		// SetSQL
		virtual bool				SetSQL(const char* szsql) = 0;
		virtual const char*			GetSQL() const = 0;

		virtual       void			Release() = 0;
	};

	//------------------------------------------------------------------------

	/**
	 * OrderBy
	 *
	 * Query 排序
	 */
	class OrderBy : public GObject
	{
	protected:
		OrderBy(){}
		virtual ~OrderBy(){}
	public:
		virtual	RESULTCODE			AddField(const char* szFieldName) = 0;
		virtual const char*			GetField(g_uint i) = 0;
		virtual	g_uint				GetFieldCount() = 0;
		virtual void				Clear() = 0;

		virtual void				SetOrder(augeOrderType order) = 0;
		virtual bool				IsAsc() = 0;

		virtual void				Release() = 0;
	};

	class FilterReader
	{
	protected:
		FilterReader(){}
		virtual ~FilterReader(){}
	public:
		virtual GFilter*			Read(XElement* pxFilter) = 0;
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
		virtual	Literal*				CreateLiteral() = 0;
		virtual	Literal*				CreateLiteral(GValue* pValue) = 0;
		virtual PropertyName*			CreatePropertyName(const char* szName) = 0;

		//---------------------------------------------------------------
		// Filter
		virtual	BBoxFilter*				CreateBBoxFilter() = 0;
		virtual	BBoxFilter*				CreateBBoxFilter(Expression* pPropertyName, GEnvelope& bbox) = 0;

		virtual IDFilter*				CreateIDFilter() = 0;

		virtual IsBetweenFilter*		CreateIsBetweenFilter() = 0;
		virtual IsBetweenFilter*		CreateIsBetweenFilter(Expression* pPropertyName,
															  Expression* pLowerBound,
															  Expression* pUpperBound) = 0;

		virtual BinaryComparisonFilter*	CreateBinaryComparisonFilter() = 0;
		virtual BinaryLogicFilter*		CreateBinaryLogicFilter(const char* oper) = 0;
		virtual UnaryLogicFilter*		CreateUnaryLogicFilter() = 0;

		virtual GQuery*					CreateQuery() = 0;
		virtual OrderBy*				CreateOrderBy() = 0;

		virtual const char*				AsString(augeLogicalOperator oper) = 0;
		virtual const char*				AsString(augeComparisonOperator oper) = 0;
		virtual const char*				AsString(augeArithmeticOperator oper) = 0;

		virtual FilterReader*			CreateFilerReader(GFields* pFields) = 0;
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



