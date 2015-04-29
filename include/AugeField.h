#ifndef __AUGE_FILED_H__
#define __AUGE_FILED_H__

#include "AugeGeometry.h"
#include "AugeCore.h"

#ifdef WIN32 
#	ifdef AUGE_FIELD_EXPORTS
#		define AUGE_FIELD_API __declspec(dllexport)
#	else
#		define AUGE_FIELD_API __declspec(dllimport)
#	endif
#else
#	define AUGE_FIELD_API
#endif

namespace auge
{
	typedef enum augeFieldFlag
	{
		augeFieldFlagUnknown      = 0x0000,
		augeFieldFlagNullable     = 0x0001,
		augeFieldFlagVisible      = 0x0002,
		augeFieldFlagIndexed      = 0x0004,
		augeFieldFlagEditable     = 0x0008,
		augeFieldFlagPrimary      = 0x0010,
		augeFieldFlagFixedLen     = 0x0020,
		augeFieldFlagUnique       = 0x0040,
		augeFieldFlagSystem       = 0x0080
	} augeFieldFlag;

	// -------------------------------------------------------------------
	#define   HasFlagX(flags,flagx) (((flags&flagx)==flagx))
	#define   CnlFlagX(flags,flagx) (flags=((flags&(~flagx))))
	#define   SetFlagX(flags,flagx) (flags=((flags|flagx)))
	// -------------------------------------------------------------------

	class GField;
	class GField_2;
	class GFields;
	class GeometryDef;
	class GeometryDef_2;
	
	/**
	 * GField
	 *
	 * 只读字段类
	 */
	class GField : public GObject
	{
	protected:
		GField(){}
		virtual ~GField(){}
	public:
		//! @brief	获取字段名
		//! @return	字段名
		virtual const char*			GetName() = 0;
		//! @brief	获取字段别名
		//! @return	字段别名
		virtual const char*			GetAlias() = 0;

		//! @brief	获取字段类型
		//! @return	字段类型
		virtual augeFieldType		GetType() const = 0;
		//! @brief	获取字符串形式的字段类型
		//! @return	符串形式的字段类型
		virtual const char*			GetTypeName() const = 0;
	
		//! @brief	获取字段长度
		//! @return	字段长度
		virtual long				GetLength() const = 0;	
		//! @brief	获取字段精度
		//! @return	字段精度
		virtual	int					GetPrecision() const = 0;
		virtual	double				GetScale() const = 0;
		//! @brief	获取字段默认值
		//! @return	字段默认值
		virtual GValue*				GetDefaultValue() = 0;
	
		//! @brief	字段是否可以为空
		//! @return	true	可以为空
		//! @return	fale	不可以为空
		virtual bool				IsNullable() = 0;
		//! @brief	字段是否是主键
		//! @return	true	是
		//! @return	fale	否
		virtual bool				IsPrimary() = 0;
		//! @brief	字段是否是系统字段
		//! @return	true	是
		//! @return	fale	否
		virtual bool				IsSystem() = 0;

		//! @brief	字段是否是几何字段
		//! @return	true	是
		//! @return	fale	否
		virtual bool				IsGeometryField() const = 0;	
		//! @brief	获取几何字段元数据
		//! @return	GeometryDef指针
		virtual GeometryDef*		GetGeometryDef() = 0;
	
		//! @brief	复制字段
		//! @return	复制后的Field指针
		virtual GField*				Clone() const = 0;

		//! @brief	获取该字段的可编辑对象指针
		//! @return	可编辑对象指针
		virtual GField_2*			Field_2() = 0;
	};
	//-----------------------------------------------------------------------------
	/**
	 * GField_2
	 *
	 * 可写Field类
	 */
	class GField_2
	{
	protected:
		GField_2(){}
		virtual ~GField_2(){}
	public:

		//! @brief	设置字段类型
		//! @param	type [IN]	字段类型
		virtual void				SetType(augeFieldType type) = 0;
		//! @brief	设置字段名
		//! @param	szName [IN]	字段名
		virtual void				SetName(const char* szName) = 0;
		//! @brief	设置字段别名
		//! @param	szAlias [IN]	字段别名
		virtual void				SetAlias(const char* szAlias) = 0;

		//! @brief	设置字段长度
		//! @param	len [IN]		字段长度
		virtual void				SetLength(long len) = 0;	
		//! @brief	设置字段精度
		//! @param	nPrecision [IN]	字段精度
		virtual	void				SetPrecision(int nPrecision) = 0;
		virtual	void				SetScale(double scale) = 0;

		//! @brief	设置字段是否可以为NULL
		//! @param	flag [IN]
		virtual void				SetNullable(bool flag) = 0;
		//! @param	设置字段是否是主键
		//! @brief	flag [IN]
		virtual void				SetPrimary(bool flag) = 0;

		//! @param	设置字段的默认值
		//! @brief	flag [IN]
		virtual bool				SetDefaultValue(GValue* pValue) = 0;
		//! @brief	获取几何字段元数据类、
		//! @return	几何字段元数据对象指针
		//			[注]：无须释放
		virtual	GeometryDef*		GetGeometryDef() = 0;

		//! @brief	获取只读字段对象指针
		//! @return	只读字段对象指针
		virtual GField*				Field() = 0;
	};

	//------------------------------------------------------------------------------
	/**s
	 * GeometryDef
	 *
	 * 只读几何字段元数据类
	 */
	class GeometryDef
	{
	protected:
		GeometryDef(){}
		virtual ~GeometryDef(){}
	public:
		//! @brief	获取空间参考ID
		//! @return	SRID
		virtual	int				 GetSRID()		const = 0;
		//! @brief	判断是否包含Z值
		//! @return	true/false
		virtual	bool			 HasZ()			const = 0;
		//! @brief	判断是否包含M值
		//! @return	true/false
		virtual	bool			 HasM()			const = 0;
		//! @brief	判断是否是默认几何字段
		//! @return	true/false
		virtual	bool			 IsDefault()	const = 0;
		//! @brief	获取几何类型
		//! @return	几何类型
		virtual augeGeometryType GeometryType()	const = 0;
		//! @brief	获取空间范围
		//! @param	extent	[OUT]	空间范围
		//! @return	true/false
		virtual bool			 GetExtent(GEnvelope& extent) const = 0;

		virtual g_uint			 GetDimension() = 0;
			
		//! @brief	获取可写GeometryDef指针
		//! @return	可写GeometryDef指针
		virtual GeometryDef_2*	GetGeometryDef_2() = 0;
	};

	//------------------------------------------------------------------------------
	/**
	 * GeometryDef_2
	 *
	 * 可写几何字段元数据类
	 */
	class GeometryDef_2 : GObject
	{
	protected:
		GeometryDef_2(){}
		virtual ~GeometryDef_2(){}
	public:	
		//! @brief	设置SRID
		//! @param	SRID
		virtual	void			SetSRID(int nSRID)		= 0;
		//! @brief	设置是否含有Z
		//! @param	bHanZ
		virtual	void			SetZ(bool bHasZ)		= 0;
		//! @brief	设置是否含有M
		//! @param	bHasM
		virtual	void			SetM(bool bHasM)		= 0;
		//! @brief	设置是否是默认几何字段
		//! @param	bIsDefault
		virtual	void			SetDefault(bool bIsDefault)	= 0;
		//! @brief	设置集合类型
		//! @param	type
		virtual void			SetGeometryType(augeGeometryType type) = 0;	
		//! @brief	设置空间范围
		//! @param	extent
		virtual bool			SetExtent(const GEnvelope& extent) = 0;

		virtual void			SetDimension(g_uint value) = 0;

		//! @brief	获取只读类型空间数据元数据指针
		//virtual GeometryDef*	GeometryDef() = 0;
	};

	//------------------------------------------------------------------------------
	/**
	 * GFields
	 *
	 * 字段集合类
	 */
	class GFields : public GObject
	{
	protected:
		GFields(){}
		virtual ~GFields(){}
	public:
		virtual	int				Count() const =0;
		virtual GField*			GetField(int index) = 0;
		virtual GField*			GetField(const char* szName) = 0;
		virtual	int				FindField(const char* szName,int iFieldType=augeFieldTypeNone) = 0;

		virtual GField*			GetGeometryField() = 0;
	
		virtual RESULTCODE		Add(GField* pField) = 0;	
		virtual RESULTCODE		Remove(const char* szName) = 0;
		virtual void			Clear() = 0;
	
		virtual bool			HasGeometryField() const = 0;
	};
	

	class FieldFactory
	{
	protected:
		FieldFactory(){}
		virtual ~FieldFactory(){}
	public:
		virtual GField*			CreateField() = 0;
		virtual GFields*		CreateFields() = 0;
		virtual const char*		Encode(augeFieldType type) = 0;
		virtual augeFieldType	Decode(const char* type) = 0;
	};

	extern "C"
	{
		AUGE_FIELD_API FieldFactory*	augeGetFieldFactoryInstance();
	}

}

#endif //__AUGE_FILED_H__
