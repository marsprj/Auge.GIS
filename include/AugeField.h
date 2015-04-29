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
	 * ֻ���ֶ���
	 */
	class GField : public GObject
	{
	protected:
		GField(){}
		virtual ~GField(){}
	public:
		//! @brief	��ȡ�ֶ���
		//! @return	�ֶ���
		virtual const char*			GetName() = 0;
		//! @brief	��ȡ�ֶα���
		//! @return	�ֶα���
		virtual const char*			GetAlias() = 0;

		//! @brief	��ȡ�ֶ�����
		//! @return	�ֶ�����
		virtual augeFieldType		GetType() const = 0;
		//! @brief	��ȡ�ַ�����ʽ���ֶ�����
		//! @return	������ʽ���ֶ�����
		virtual const char*			GetTypeName() const = 0;
	
		//! @brief	��ȡ�ֶγ���
		//! @return	�ֶγ���
		virtual long				GetLength() const = 0;	
		//! @brief	��ȡ�ֶξ���
		//! @return	�ֶξ���
		virtual	int					GetPrecision() const = 0;
		virtual	double				GetScale() const = 0;
		//! @brief	��ȡ�ֶ�Ĭ��ֵ
		//! @return	�ֶ�Ĭ��ֵ
		virtual GValue*				GetDefaultValue() = 0;
	
		//! @brief	�ֶ��Ƿ����Ϊ��
		//! @return	true	����Ϊ��
		//! @return	fale	������Ϊ��
		virtual bool				IsNullable() = 0;
		//! @brief	�ֶ��Ƿ�������
		//! @return	true	��
		//! @return	fale	��
		virtual bool				IsPrimary() = 0;
		//! @brief	�ֶ��Ƿ���ϵͳ�ֶ�
		//! @return	true	��
		//! @return	fale	��
		virtual bool				IsSystem() = 0;

		//! @brief	�ֶ��Ƿ��Ǽ����ֶ�
		//! @return	true	��
		//! @return	fale	��
		virtual bool				IsGeometryField() const = 0;	
		//! @brief	��ȡ�����ֶ�Ԫ����
		//! @return	GeometryDefָ��
		virtual GeometryDef*		GetGeometryDef() = 0;
	
		//! @brief	�����ֶ�
		//! @return	���ƺ��Fieldָ��
		virtual GField*				Clone() const = 0;

		//! @brief	��ȡ���ֶεĿɱ༭����ָ��
		//! @return	�ɱ༭����ָ��
		virtual GField_2*			Field_2() = 0;
	};
	//-----------------------------------------------------------------------------
	/**
	 * GField_2
	 *
	 * ��дField��
	 */
	class GField_2
	{
	protected:
		GField_2(){}
		virtual ~GField_2(){}
	public:

		//! @brief	�����ֶ�����
		//! @param	type [IN]	�ֶ�����
		virtual void				SetType(augeFieldType type) = 0;
		//! @brief	�����ֶ���
		//! @param	szName [IN]	�ֶ���
		virtual void				SetName(const char* szName) = 0;
		//! @brief	�����ֶα���
		//! @param	szAlias [IN]	�ֶα���
		virtual void				SetAlias(const char* szAlias) = 0;

		//! @brief	�����ֶγ���
		//! @param	len [IN]		�ֶγ���
		virtual void				SetLength(long len) = 0;	
		//! @brief	�����ֶξ���
		//! @param	nPrecision [IN]	�ֶξ���
		virtual	void				SetPrecision(int nPrecision) = 0;
		virtual	void				SetScale(double scale) = 0;

		//! @brief	�����ֶ��Ƿ����ΪNULL
		//! @param	flag [IN]
		virtual void				SetNullable(bool flag) = 0;
		//! @param	�����ֶ��Ƿ�������
		//! @brief	flag [IN]
		virtual void				SetPrimary(bool flag) = 0;

		//! @param	�����ֶε�Ĭ��ֵ
		//! @brief	flag [IN]
		virtual bool				SetDefaultValue(GValue* pValue) = 0;
		//! @brief	��ȡ�����ֶ�Ԫ�����ࡢ
		//! @return	�����ֶ�Ԫ���ݶ���ָ��
		//			[ע]�������ͷ�
		virtual	GeometryDef*		GetGeometryDef() = 0;

		//! @brief	��ȡֻ���ֶζ���ָ��
		//! @return	ֻ���ֶζ���ָ��
		virtual GField*				Field() = 0;
	};

	//------------------------------------------------------------------------------
	/**s
	 * GeometryDef
	 *
	 * ֻ�������ֶ�Ԫ������
	 */
	class GeometryDef
	{
	protected:
		GeometryDef(){}
		virtual ~GeometryDef(){}
	public:
		//! @brief	��ȡ�ռ�ο�ID
		//! @return	SRID
		virtual	int				 GetSRID()		const = 0;
		//! @brief	�ж��Ƿ����Zֵ
		//! @return	true/false
		virtual	bool			 HasZ()			const = 0;
		//! @brief	�ж��Ƿ����Mֵ
		//! @return	true/false
		virtual	bool			 HasM()			const = 0;
		//! @brief	�ж��Ƿ���Ĭ�ϼ����ֶ�
		//! @return	true/false
		virtual	bool			 IsDefault()	const = 0;
		//! @brief	��ȡ��������
		//! @return	��������
		virtual augeGeometryType GeometryType()	const = 0;
		//! @brief	��ȡ�ռ䷶Χ
		//! @param	extent	[OUT]	�ռ䷶Χ
		//! @return	true/false
		virtual bool			 GetExtent(GEnvelope& extent) const = 0;

		virtual g_uint			 GetDimension() = 0;
			
		//! @brief	��ȡ��дGeometryDefָ��
		//! @return	��дGeometryDefָ��
		virtual GeometryDef_2*	GetGeometryDef_2() = 0;
	};

	//------------------------------------------------------------------------------
	/**
	 * GeometryDef_2
	 *
	 * ��д�����ֶ�Ԫ������
	 */
	class GeometryDef_2 : GObject
	{
	protected:
		GeometryDef_2(){}
		virtual ~GeometryDef_2(){}
	public:	
		//! @brief	����SRID
		//! @param	SRID
		virtual	void			SetSRID(int nSRID)		= 0;
		//! @brief	�����Ƿ���Z
		//! @param	bHanZ
		virtual	void			SetZ(bool bHasZ)		= 0;
		//! @brief	�����Ƿ���M
		//! @param	bHasM
		virtual	void			SetM(bool bHasM)		= 0;
		//! @brief	�����Ƿ���Ĭ�ϼ����ֶ�
		//! @param	bIsDefault
		virtual	void			SetDefault(bool bIsDefault)	= 0;
		//! @brief	���ü�������
		//! @param	type
		virtual void			SetGeometryType(augeGeometryType type) = 0;	
		//! @brief	���ÿռ䷶Χ
		//! @param	extent
		virtual bool			SetExtent(const GEnvelope& extent) = 0;

		virtual void			SetDimension(g_uint value) = 0;

		//! @brief	��ȡֻ�����Ϳռ�����Ԫ����ָ��
		//virtual GeometryDef*	GeometryDef() = 0;
	};

	//------------------------------------------------------------------------------
	/**
	 * GFields
	 *
	 * �ֶμ�����
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
