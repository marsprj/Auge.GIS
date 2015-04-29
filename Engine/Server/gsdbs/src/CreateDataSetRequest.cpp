#include "CreateDataSetRequest.h"f
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	CreateDataSetRequest::CreateDataSetRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = "GBK";
		m_pxDoc = NULL;
		m_pFields = NULL;
	}

	CreateDataSetRequest::~CreateDataSetRequest()
	{
		if(m_pxDoc)
		{
			AUGE_SAFE_RELEASE(m_pxDoc);
		}
		if(m_pFields)
		{
			AUGE_SAFE_RELEASE(m_pFields);
		}
	}

	const char*	CreateDataSetRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	CreateDataSetRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CreateDataSetRequest::GetRequest()
	{
		return "CreateDataSet";
	}

	const char*	CreateDataSetRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	CreateDataSetRequest::GetSourceName()
	{
		return m_source_name.c_str();
	}

	const char*	CreateDataSetRequest::GetDataSetName()
	{
		return m_dataset_name.c_str();
	}

	void CreateDataSetRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void CreateDataSetRequest::SetSourceName(const char* name)
	{
		if(name==NULL)
		{
			m_source_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = name;
		}
	}

	void CreateDataSetRequest::SetDataSetName(const char* name)
	{
		if(name==NULL||(strlen(name)==0))
		{
			m_dataset_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_dataset_name.clear();
		}
		else
		{
			m_dataset_name = name;
		}
	}

	bool CreateDataSetRequest::Create(XDocument* pxDoc)
	{
		if(m_pxDoc)
		{
			AUGE_SAFE_RELEASE(m_pxDoc);
		}
		m_pxDoc = pxDoc;

		XElement* pxRoot = pxDoc->GetRootNode();
		XAttribute* pxAttr = pxRoot->GetAttribute("sourceName");
		if(pxAttr!=NULL)
		{
			SetSourceName(pxAttr->GetValue());
		}

		pxAttr = pxRoot->GetAttribute("dataSetName");
		if(pxAttr!=NULL)
		{
			SetDataSetName(pxAttr->GetValue());
		}

		return true;
	}

	GFields* CreateDataSetRequest::GetFields()
	{
		if(m_pFields!=NULL)
		{
			return m_pFields;
		}
		m_pFields = CreateFields();
		return m_pFields;
	}

	GFields* CreateDataSetRequest::CreateFields()
	{
		if(m_pxDoc==NULL)
		{
			return NULL;
		}

		XElement *pxRoot = m_pxDoc->GetRootNode();
		XNode	 *pxFields = pxRoot->GetFirstChild();
		if(pxFields==NULL)
		{
			return NULL;
		}

		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		GFields* pFields = pFieldFactory->CreateFields();

		XNode		*pxNode = NULL;
		XNodeSet	*pxNodeSet = pxFields->GetChildren();
		if(pxNodeSet!=NULL)
		{
			pxNodeSet->Reset();
			while((pxNode = pxNodeSet->Next())!=NULL)
			{
				XElement* pxField = (XElement*)(pxNode);
				GField* pField = CreateField(pxField);
				if(pField==NULL)
				{
					pFields->Release();
					pFields = NULL;
					break;
				}
				else
				{
					pFields->Add(pField);
				}
			}
			pxNodeSet->Release();
		}

		return pFields;
	}

	//<Field>
	//	<Name>ID</Name>
	//	<Type>Int</Type>
	//	<DefaultValue>0</DefaultValue>
	//</Field>
	//<Field>
	//	<Name>Name</Name>
	//	<Type>String</Type>
	//	<Length>32</Length>
	//	<Nullable>true</Nullable>
	//	<DefaultValue>china</DefaultValue>
	//</Field>
	//<Field>
	//	<Name>shape</Name>
	//	<Type>Geometry</Type>
	//	<Nullable>true</Nullable>
	//	<GeometryDef>
	//		<Type>Point</Type>
	//		<SRID>4326</SRID>
	//	</GeometryDef>
	//</Field>
	GField* CreateDataSetRequest::CreateField(XElement* pxField)
	{	
		XNodeSet* pxNodeSet = pxField->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		GField* pField = pFieldFactory->CreateField();
		GField_2 *pField_2 = dynamic_cast<GField_2*>(pField);

		XNode *pxNode = NULL;
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next())!=NULL)
		{
			XElement* pxElement = static_cast<XElement*>(pxNode);
			const char* name = pxElement->GetName();
			if(g_stricmp(name, "Name")==0)
			{
				pField_2->SetName(pxNode->GetContent());
			}
			else if(g_stricmp(name, "Type")==0)
			{
				augeFieldType type = pFieldFactory->Decode(pxNode->GetContent());
				pField_2->SetType(type);
			}
			else if(g_stricmp(name, "GeometryDef")==0)
			{
				GeometryDef* pGeometryDef = pField->GetGeometryDef();	
				if(pGeometryDef!=NULL)
				{
					SetGeometryDef(pGeometryDef, pxNode);
				}
			}
			else if(g_stricmp(name, "Length")==0)
			{
				int length = atoi(pxNode->GetContent());
				pField_2->SetLength(length);
			}
			else if(g_stricmp(name, "Nullable")==0)
			{
				const char* text = pxNode->GetContent();
				if(g_stricmp(text,"true")==0)
					pField_2->SetNullable(true);
				else
					pField_2->SetNullable(false);
			}
		}
		pxNodeSet->Release();

		return pField;
	}

	void CreateDataSetRequest::SetGeometryDef(GeometryDef* pGeometryDef, XNode* pxGeometryDef)
	{
		XNode* pxNode = NULL;
		XNodeSet* pxNodeSet = pxGeometryDef->GetChildren();

		GEnvelope extent(-180.0f,-90.0f,180.0f,90.0f);
		GeometryDef_2* pGeometryDef_2 = dynamic_cast<GeometryDef_2*>(pGeometryDef);
		pGeometryDef_2->SetDefault(true);
		pGeometryDef_2->SetDimension(2);
		pGeometryDef_2->SetExtent(extent);

		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next())!=NULL)
		{
			const char* name = pxNode->GetName();
			if(!g_stricmp(name, "Type"))
			{
				const char* text = pxNode->GetContent();
				GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
				augeGeometryType type = pGeometryFactory->DecodeGeometryType(text);
				pGeometryDef_2->SetGeometryType(type);
			}
			else if(!g_stricmp(name, "SRID"))
			{
				const char* srid = pxNode->GetContent();
				pGeometryDef_2->SetSRID(atoi(srid));
			}
			else if(!g_stricmp(name, "Dimension"))
			{
				const char* dimension = pxNode->GetContent();
				pGeometryDef_2->SetDimension(atoi(dimension));
			}
			else if(!g_stricmp(name, "Extent"))
			{
				XNode* pxXMin = pxNode->GetFirstChild("XMin");
				XNode* pxXMax = pxNode->GetFirstChild("XMax");
				XNode* pxYMin = pxNode->GetFirstChild("YMin");
				XNode* pxYMax = pxNode->GetFirstChild("YMax");
				if(pxXMin!=NULL)
					extent.m_xmin = atof(pxXMin->GetContent());
				if(pxXMax!=NULL)
					extent.m_xmax = atof(pxXMax->GetContent());
				if(pxYMin!=NULL)
					extent.m_ymin = atof(pxYMin->GetContent());
				if(pxYMax!=NULL)
					extent.m_ymax = atof(pxYMax->GetContent());
				pGeometryDef_2->SetExtent(extent);
			}
		}

		pxNodeSet->Release();
	}
}