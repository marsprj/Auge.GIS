#ifndef __AUGE_FIELDS_IMPL_H__
#define __AUGE_FIELDS_IMPL_H__

#include "AugeField.h"
#include <vector>

namespace auge
{

	class GFieldsImpl : public GFields
	{
	public:
		GFieldsImpl();
		virtual ~GFieldsImpl();
	public:
		virtual	int				Count() const;
		virtual GField*			GetField(int index);
		virtual GField*			GetField(const char* szName);
		virtual	int				FindField(const char* szName,int iFieldType=augeFieldTypeNone);

		virtual GField*			GetGeometryField();

		virtual RESULTCODE		Add(GField* pField);

		virtual RESULTCODE		Remove(const char* szName);
		virtual void			Clear();

		virtual bool			HasGeometryField() const;

		virtual void			Release();

	public:
		RESULTCODE	AddRef(GField* pField);
	protected:
		bool		IsValidIndex(int index);
	private:
		typedef std::vector<GField*>	FIELD_VECTOR;
		FIELD_VECTOR			m_fields;
	};

}

#endif  //__AUGE_FIELDS_IMPL_H__
