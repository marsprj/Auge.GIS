#include "AttributeDataSetPgs.h"
#include "WorkspacePgs.h"

#include "AugeField.h"

namespace auge
{
	AttributeDataSetPgs::AttributeDataSetPgs():
		m_pWorkspace(NULL),
		m_pFields(NULL),
		m_schema("public")
	{
		FieldFactory* pFactory = augeGetFieldFactoryInstance();
		m_pFields = pFactory->CreateFields();
	}

	AttributeDataSetPgs::~AttributeDataSetPgs()
	{
		AUGE_SAFE_RELEASE(m_pFields);
	}

	const char*	AttributeDataSetPgs::GetName()
	{
		return m_name.c_str();
	}

	augeDataSetType	AttributeDataSetPgs::GetType()
	{
		return augeDataSetAttribute;
	}

	GFields* AttributeDataSetPgs::GetFields()
	{
		return m_pFields;
	}

	GField*	AttributeDataSetPgs::GetField(const char* name)
	{
		return m_pFields->GetField(name);
	}

	Cursor*	AttributeDataSetPgs::GetRows()
	{
		return NULL;
	}

	void AttributeDataSetPgs::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

	bool AttributeDataSetPgs::Create(const char* name, WorkspacePgs* pWorkspace, PGresult* pgResult)
	{
		m_name = name;
		m_pWorkspace = pWorkspace;
		
		if(!CreateFields(pgResult))
		{
			return false;
		}

		return true;
		
	}

	bool AttributeDataSetPgs::CreateFields(PGresult* pgResult)
	{	
		m_pFields->Clear();

		GField* pField = NULL;
		int nfields = PQnfields(pgResult);
		for(int i=0; i<nfields; i++)
		{
			pField = CreateField(i, pgResult);
			if(pField!=NULL)
			{
				m_pFields->Add(pField);
			}
		}

		return true;
	}


	GField*	AttributeDataSetPgs::CreateField(int col, PGresult* pgResult)
	{
		Oid pgType		 = PQftype(pgResult, col);
		const char* fname= PQfname(pgResult, col);

		augeFieldType type = augeFieldTypeNone;
		type = m_pWorkspace->m_pgConnection_r.GetFieldType(pgType);
		if(type==augeFieldTypeNone)
		{
			return NULL;
		}

		GField	 *pField = NULL;
		GField_2 *pField_2 = NULL;
		FieldFactory* pFactory = augeGetFieldFactoryInstance();

		pField = pFactory->CreateField();
		pField_2 = pField->Field_2();

		pField_2->SetName(fname);
		pField_2->SetAlias(fname);
		pField_2->SetType(type);


		g_int fsize = 0;
		if(pField->GetType()==augeFieldTypeString)
			fsize = PQfmod(pgResult, col) -4;
		else
			fsize = PQfsize(pgResult, col);

		pField_2->SetLength(fsize);


		return pField;
	}
}