#ifndef __AUGE_FIELD_IMPL_H__
#define __AUGE_FIELD_IMPL_H__

#include "AugeField.h"
#include <vector>
#include <string>

namespace auge
{
	class GeometryDefImpl;

	class GFieldImpl : public GField, virtual public GField_2
	{
	public:
		GFieldImpl();
		virtual ~GFieldImpl();
	public:
		virtual const char*			GetName();
		virtual void				SetName(const char* szName);

		virtual const char*			GetAlias();
		virtual void				SetAlias(const char* szAlias);

		virtual augeFieldType		GetType() const;
		virtual void				SetType(augeFieldType type);
		virtual const char*			GetTypeName() const;

		virtual long				GetLength() const;
		virtual void				SetLength(long len);

		virtual	int					GetPrecision() const;
		virtual	void				SetPrecision(int nPrecision);

		virtual	double				GetScale() const;
		virtual	void				SetScale(double scale);

		virtual bool				IsNullable();
		virtual void				SetNullable(bool flag);

		virtual bool				IsPrimary();
		virtual void				SetPrimary(bool flag);

		virtual bool				IsSystem();
		virtual void				SetSystem(bool flag);

		virtual bool				IsGeometryField() const;	
		virtual GeometryDef*		GetGeometryDef();

		virtual GValue*				GetDefaultValue();
		virtual bool				SetDefaultValue(GValue* pGValue);

		virtual GField*				Clone() const;

		virtual GField*				Field();
		virtual GField_2*			Field_2();

		virtual void				Release();

	private:
		augeFieldType		m_Type;							// �ֶ��������
		std::string			m_name;						// �ֶ�����
		std::string			m_alias;						// �ֶα���
		g_uint				m_tag;							// �ֶα�־���磺Nullable,Indexed,Visible etc
		long				m_length;						// �̶�����ʱ���ֶγ���	
		double				m_scale;						// ��������
		g_int				m_precision;					// ����
		GeometryDefImpl		*m_pGeometryDef;					// �ռ��ֶ���Ϣ����
		GValue				*m_pValue;
	};

}

#endif //__AUGE_FIELD_IMPL_H__
