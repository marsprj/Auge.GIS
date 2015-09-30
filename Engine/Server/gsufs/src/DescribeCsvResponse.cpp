#include "DescribeCsvRequest.h"
#include "DescribeCsvResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{ 
	DescribeCsvResponse::DescribeCsvResponse(DescribeCsvRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pDataset = NULL;
	}

	DescribeCsvResponse::~DescribeCsvResponse()
	{
		Cleanup();
	}

	void DescribeCsvResponse::Cleanup()
	{
		if(m_pDataset!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataset);
		}
	}

	void DescribeCsvResponse::SetDataset(AttributeDataSet* pDataset)
	{
		Cleanup();
		m_pDataset = pDataset;
	}
	
	void DescribeCsvResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE DescribeCsvResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = new XDocument();
		WriteDocument(pxDoc);

		int len = 0;
		g_uchar* buffer = NULL;
		GLogger* pLogger = augeGetLoggerInstance();

		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);
		
		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		return AG_SUCCESS;
	}

	void DescribeCsvResponse::WriteDocument(XDocument* pxDoc)
	{
		const char* setName = m_pDataset->GetName();
		char str[AUGE_NAME_MAX];

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		pxRoot = pxDoc->CreateRootNode("schema", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
		pxRoot->SetAttribute("version", "1.1.0", NULL);

		// schema-->import
		XElement* pxImport = pxRoot->AddChild("import", "xsd");
		pxImport->SetAttribute("namespace","http://www.opengis.net/gml", NULL);
		// schema-->complexType
		g_sprintf(str,"%sType", setName);
		XElement* pxComplexType = pxRoot->AddChild("complexType", "xsd");
		pxComplexType->SetAttribute("name", str, NULL, true);
		// schema-->complexType-->complexContent
		XElement* pxComplexContent = pxComplexType->AddChild("complexContent","xsd");
		// schema-->complexType-->complexContent-->extentsion
		XElement* pxExtentsion = pxComplexContent->AddChild("extentsion", "xsd");
		pxExtentsion->SetAttribute("base","gml:AbstractFeatureType", NULL);
		// schema-->complexType-->complexContent-->extentsion-->sequence
		XElement* pxSequence = pxExtentsion->AddChild("sequence", "xsd");

		GField	*pField  = NULL;
		GFields	*pFields = m_pDataset->GetFields();
		g_uint count = pFields->Count();
		augeFieldType ftype = augeFieldTypeNone;

		// schema-->complexType-->complexContent-->extentsion-->sequence-->element
		XElement* pxElement = NULL;
		for(g_uint i=0; i<count; i++)
		{
			pField = pFields->GetField(i);
			pxElement = pxSequence->AddChild("element", "xsd");
			const char* fname = pField->GetName();
			const char* utf8_name = auge_encoding_convert("GBK","UTF-8",fname,strlen(fname));
			pxElement->SetAttribute("name",utf8_name, NULL);
			pxElement->SetAttribute("nillable",pField->IsNullable()?"true":"false", NULL);
			pxElement->SetAttribute("minOccurs","0", NULL);
			pxElement->SetAttribute("maxOccurs","1", NULL);
			pxElement->SetAttribute("type","xsd:string", NULL);
		}

		// schema-->element
		pxElement = pxRoot->AddChild("element", "xsd");
		g_sprintf(str, "%sType", setName);
		pxElement->SetAttribute("type",str, NULL, true);
		pxElement->SetAttribute("name",setName, NULL, true);
		pxElement->SetAttribute("substitutionGroup","gml:_Feature", NULL);
	}

}