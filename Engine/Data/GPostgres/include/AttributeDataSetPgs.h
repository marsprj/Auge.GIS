#ifndef __AUGE_ATTRIBUTE_DATASET_PGS_H__
#define __AUGE_ATTRIBUTE_DATASET_PGS_H__

#include "AugeData.h"
#include <string>
#include <libpq-fe.h>

namespace auge
{
	class GField;
	class GFields;
	class WorkspacePgs;

	class AttributeDataSetPgs : public AttributeDataSet
	{
	public:
		AttributeDataSetPgs();
		virtual ~AttributeDataSetPgs();
	public:
		virtual const char*			GetName();
		virtual augeDataSetType		GetType();

		virtual GFields*			GetFields();
		virtual GField*				GetField(const char* name);
		virtual Cursor*				GetRows();

		virtual void				Release();

	public:
		bool	Create(const char* name, WorkspacePgs* pWorkspace, PGresult* pgResult);

	private:
		
		GField*	CreateField(int col, PGresult* pgResult);
		bool	CreateFields(PGresult* pgResult);

	private:
		std::string			m_name;
		std::string			m_schema;
		WorkspacePgs*		m_pWorkspace;
		GFields*			m_pFields;
	};
}

#endif //__AUGE_ATTRIBUTE_DATASET_PGS_H__
