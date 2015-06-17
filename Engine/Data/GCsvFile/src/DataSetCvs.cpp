#include "DataSetCvs.h"
#include "CursorImpl.h"
#include "AugeField.h"

namespace auge
{
	DataSetCsv::DataSetCsv()
	{
		m_pWorkspace = NULL;
		m_pFields = NULL;
	}

	DataSetCsv::~DataSetCsv()
	{
		m_pWorkspace = NULL;
		if(m_pFields!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFields);
		}
	}

	const char* DataSetCsv::GetName()
	{
		return m_name.c_str();
	}

	augeDataSetType	DataSetCsv::GetType()
	{
		return augeDataSetAttribute;
	}

	GFields* DataSetCsv::GetFields()
	{
		if(m_pFields==NULL)
		{
			m_pFields = CreateFields();
		}
		return m_pFields;
	}

	GField*	DataSetCsv::GetField(const char* name)
	{
		return NULL;
	}

	bool DataSetCsv::Create(const char* name, const char* path, WorkspaceCsv* pWorkspace)
	{
		m_name = name;
		m_path = path;
		m_pWorkspace = pWorkspace;

		std::ifstream is;
		is.open(m_path.c_str());
		bool ret = is.is_open();
		is.close();

		return ret;
	}

	GFields* DataSetCsv::CreateFields()
	{	
		
		std::ifstream is;
		is.open(m_path.c_str());
		if(!is.is_open())
		{
			return NULL;
		}
		
		std::string headers;
		if(!getline(is, headers))
		{
			is.close();
			return NULL;
		}

		GField		*pField  = NULL;
		GField_2	*pField_2= NULL;
		GFields		*pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		pFields = pFieldFactory->CreateFields();

		char* str = strdup(headers.c_str());
		char* ptr = strtok(str, ",");
		while(ptr!=NULL)
		{
			pField = pFieldFactory->CreateField();
			pField_2 = pField->Field_2();
			pField_2->SetName(str);
			pField_2->SetType(augeFieldTypeString);
			pField_2->SetLength(0);
			pFields->Add(pField);

			ptr = strtok(NULL, ",");
		}

		free(str);
		is.close();

		return pFields;
	}

	Cursor*	DataSetCsv::GetRows()
	{
		CursorCsv* pCursor = new CursorCsv();
		pCursor->Create(this);
		return pCursor;
	}
}
