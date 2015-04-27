#include "GetDataSetRequest.h"
#include "GetDataSetResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{
	GetDataSetResponse::GetDataSetResponse(GetDataSetRequest* pRequest)
	{
		m_pDataSet = NULL;
		m_pDataSets = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetDataSetResponse::~GetDataSetResponse()
	{
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
		if(m_pDataSet!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSet);
		}
	}

	RESULTCODE GetDataSetResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		if(m_pDataSet!=NULL)
		{
			Write(pWriter, m_pDataSet);
		}
		else
		{
			Write(pWriter, m_pDataSets);
		}
	}

	RESULTCODE GetDataSetResponse::Write(WebWriter* pWriter, EnumDataSet* pDataSets)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("DataSets", NULL, NULL);

		XElement	*pxDataSet = NULL;
		XElement	*pxService  = NULL;
		pDataSets->Reset();
		DataSet* pDataSet = NULL;
		while((pDataSet=pDataSets->Next())!=NULL)
		{ 
			pxDataSet = pxRoot->AddChild("DataSet");				
			XElement* pxElement = pxDataSet->AddChild("Name");
			pxElement->SetChildText(pDataSet->GetName());
			pxElement = pxDataSet->AddChild("Type");
			pxElement->SetChildText(augeGetDataSetType(pDataSet->GetType()));
			
			switch(pDataSet->GetType())
			{
			case augeDataSetFeature:
				{
					FeatureClass* pFeatureClass = static_cast<FeatureClass*>(pDataSet);
					GField* pField = pFeatureClass->GetFields()->GetGeometryField();
					XElement* pxGeometry = pxDataSet->AddChild("Gometry");
					if(pField!=NULL)
					{
						GeometryDef* pGeometryDef = pField->GetGeometryDef();
						if(pGeometryDef!=NULL)
						{
							augeGeometryType type = pGeometryDef->GeometryType();
							GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
							const char* geomType = pGeometryFactory->Encode(type);

							XElement* pxElem = pxGeometry->AddChild("Type");
							pxElem->AddChildText(geomType);

							char str[AUGE_NAME_MAX] = {0};
							g_sprintf(str, "%d", pGeometryDef->GetSRID());
							pxElem = pxGeometry->AddChild("SRID");
							pxElem->AddChildText(str);

						}
					}
				}
				break;
			case augeDataSetRaster:
				{

				}
				break;
			}
		}

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	RESULTCODE GetDataSetResponse::Write(WebWriter* pWriter, DataSet* pDataSet)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxDataSet = pxDoc->CreateRootNode("DataSet", NULL, NULL);
		XElement	*pxService  = NULL;
				
		XElement* pxElement = pxDataSet->AddChild("Name");
		pxElement->SetChildText(pDataSet->GetName());
		pxElement = pxDataSet->AddChild("Type");
		pxElement->SetChildText(augeGetDataSetType(pDataSet->GetType()));

		switch(pDataSet->GetType())
		{
		case augeDataSetFeature:
			{
				FeatureClass* pFeatureClass = static_cast<FeatureClass*>(pDataSet);
				GField* pField = pFeatureClass->GetFields()->GetGeometryField();
				XElement* pxGeometry = pxDataSet->AddChild("Gometry");
				if(pField!=NULL)
				{
					GeometryDef* pGeometryDef = pField->GetGeometryDef();
					if(pGeometryDef!=NULL)
					{
						augeGeometryType type = pGeometryDef->GeometryType();
						GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
						const char* geomType = pGeometryFactory->Encode(type);

						XElement* pxElem = pxGeometry->AddChild("Type");
						pxElem->AddChildText(geomType);

						char str[AUGE_NAME_MAX] = {0};
						g_sprintf(str, "%d", pGeometryDef->GetSRID());
						pxElem = pxGeometry->AddChild("SRID");
						pxElem->AddChildText(str);

					}
				}
			}
			break;
		case augeDataSetRaster:
			{

			}
			break;
		}

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	void GetDataSetResponse::SetDataSets(EnumDataSet* pDataSets)
	{
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
		m_pDataSets = pDataSets;
		if(m_pDataSet!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSet);
		}
	}

	void GetDataSetResponse::SetDataSet(DataSet* pDataSet)
	{
		if(m_pDataSet!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSet);
		}
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
		m_pDataSet = pDataSet;
	}
}