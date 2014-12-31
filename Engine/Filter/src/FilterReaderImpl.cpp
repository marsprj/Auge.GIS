#include "FilterReaderImpl.h"
#include "AugeCore.h"
#include "AugeField.h"

namespace auge
{
	FilterReaderImpl::FilterReaderImpl(GFields* pFields):
	m_pFields(pFields)
	{

	}

	FilterReaderImpl::~FilterReaderImpl()
	{

	}

	void FilterReaderImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	GFilter* FilterReaderImpl::Read(const char* xml)
	{
		if(xml==NULL)
		{
			return NULL;
		}

		XDocument* pxDoc = NULL;
		XParser parser;

		pxDoc = parser.ParseMemory(xml);
		if(pxDoc==NULL)
		{
			return NULL;
		}

		XElement* pxFilter = NULL;
		pxFilter = pxDoc->GetRootNode();

		GFilter* pFilter = Read(pxFilter);
		pxDoc->Close();
		pxDoc->Release();

		return pFilter;
	}

	GFilter* FilterReaderImpl::Read(XElement* pxFilter)
	{
		if(pxFilter==NULL)
		{
			return NULL;
		}
		if(g_stricmp(pxFilter->GetName(),"Filter"))
		{
			return NULL;
		}
		XNode* pxNode = pxFilter->GetFirstChild();
		if(pxNode==NULL)
		{
			return NULL;
		}
		return ReadFilter(pxNode);
	}

	GFilter* FilterReaderImpl::ReadFilter(XNode* pxFilter)
	{
		GFilter* pFilter = NULL;		
		const char* type = pxFilter->GetName();
		m_prop_name.clear();

		if(!g_stricmp(type, "BBox"))
		{
			pFilter = ReadBBox(pxFilter);
		}
		else if(!g_strnicmp(type, "PropertyIs",10))
		{
			pFilter = ReadBinaryComparison(pxFilter);
		}
		else if((!g_stricmp(type, "And")) || ((!g_stricmp(type, "Or"))))
		{
			pFilter = ReadBinaryLogical(pxFilter);
		}
		else if(!g_stricmp(type, "Not"))
		{
			pFilter = ReadUnaryLogic(pxFilter);
		}
		else if(!g_stricmp(type, "GmlObjectId"))
		{
			pFilter = ReadIdFilter(pxFilter);
		}

		return pFilter;
	}

	GFilter* FilterReaderImpl::ReadBBox(XNode* pxBBox)
	{
		XNodeSet* pxNodes = pxBBox->GetChildren();
		if(pxNodes==NULL)
		{
			return NULL;
		}

		BBoxFilter* pFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		pFilter = pFactory->CreateBBoxFilter();

		XNode* pxNode = NULL;
		const char* nodeName = NULL;
		g_uint count = pxNodes->Count();
		for(g_uint i=0; i<count; i++)
		{
			pxNode = pxNodes->Item(i);
			nodeName = pxNode->GetName();
			/*if(g_stricmp(nodeName,"PropertyName")==0)
			{
				Expression* pPropName = ReadExpression(pxNode);
				pFilter->SetPropertyName(pPropName);
			}*/
			if(g_stricmp(nodeName,"Envelope")==0)
			{
				GEnvelope extent;
				if(ReadEnvelope(pxNode, extent))
				{
					pFilter->SetExtent(extent);
				}
			}
			else
			{
				Expression* pPropName = ReadExpression(pxNode);
				pFilter->SetPropertyName(pPropName);
			}
		}
		pxNodes->Release();

		return pFilter;
	}

	GFilter* FilterReaderImpl::ReadBinaryComparison(XNode* pxComparison)
	{
		BinaryComparisonFilter* pFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();

		pFilter = pFactory->CreateBinaryComparisonFilter();

		const char* type = pxComparison->GetName();
		augeComparisonOperator oper = ParseComparisonOperator(type);
		pFilter->SetOperator(oper);

		XNode *pxNode = NULL;
		XNodeSet* pxNodeSet = pxComparison->GetChildren();
		if(pxNodeSet==NULL)
		{
			pFilter->Release();
			pxNodeSet->Release();
			return NULL;
		}

		Expression* pExpression = NULL;

		const char* nodeName = NULL;
		pxNodeSet->Reset();
		pxNode = pxNodeSet->Next();
		if(pxNode==NULL)
		{
			pxNodeSet->Release();
			return NULL;
		}
		pExpression = ReadExpression(pxNode);
		if(pExpression==NULL)
		{
			pxNodeSet->Release();
			return NULL;
		}
		pFilter->SetExpression1(pExpression);

		pxNode = pxNodeSet->Next();
		if(pxNode==NULL)
		{
			pFilter->Release();
			pxNodeSet->Release();
			return NULL;
		}
		pExpression = ReadExpression(pxNode);
		if(pExpression==NULL)
		{
			pFilter->Release();
			pxNodeSet->Release();
			return NULL;
		}
		pFilter->SetExpression2(pExpression);

		//while(!pxNodeSet->IsEOF())
		//{
		//	pxNode = pxNodeSet->Next();
		//	nodeName = pxNode->GetName();
		//	if(!g_stricmp(nodeName,"PropertyName"))
		//	{
		//		Expression* pPropName = ReadExpression(pxNode);
		//		pFilter->SetExpression1(pPropName);
		//	}
		//	else if(!g_stricmp(nodeName,"Literal"))
		//	{
		//		Literal* pLiteral = ReadLiteral(pxNode);
		//		pFilter->SetExpression2(pLiteral);
		//	}
		//}
		pxNodeSet->Release();

		return pFilter;
	}

	GFilter* FilterReaderImpl::ReadBinaryLogical(XNode* pxLogical)
	{
		BinaryLogicFilter* pBinaryFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		pBinaryFilter = pFactory->CreateBinaryLogicFilter();

		XNode* pxNode = NULL;
		XNodeSet* pxNodeSet = pxLogical->GetChildren();
		GFilter* pFilter = NULL;
		
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next())!=NULL)
		{
			pFilter = ReadFilter(pxNode);
			if(pFilter!=NULL)
			{
				pBinaryFilter->AddFilter(pFilter);
			}
		}
		pxNodeSet->Release();
		return pBinaryFilter;
	}

	GFilter* FilterReaderImpl::ReadUnaryLogic(XNode* pxLogical)
	{
		UnaryLogicFilter* pUnaryFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		pUnaryFilter = pFactory->CreateUnaryLogicFilter();

		XNode* pxNode = pxLogical->GetFirstChild();
		if(pxNode==NULL)
		{
			return NULL;
		}

		GFilter* pFilter = ReadFilter(pxNode);
		if(pFilter==NULL)
		{
			pUnaryFilter->Release();
			return NULL;
		}
		pUnaryFilter->SetFilter(pFilter);
	
		return pUnaryFilter;
	}

	GFilter* FilterReaderImpl::ReadIdFilter(XNode* pxIdFilter)
	{
		IDFilter* pFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		pFilter = pFactory->CreateIDFilter();

		g_int id;
		g_int pos;
		const char* str = NULL;
		char typeName[AUGE_NAME_MAX] = {0};
		
		XAttribute* pxAttr = NULL;
		XNode* pxNode = pxIdFilter;
		while(pxNode!=NULL)
		{
			pxAttr = ((XElement*)pxNode)->GetAttribute("id",NULL);
			if(pxAttr!=NULL)
			{
				str = pxAttr->GetValue();
				//sscanf(str,"%s.%d",typeName,&id);
				pos = auge_find_last_char(str,'.');
				if(pos>0)
				{
					pFilter->Add(atoi(str+pos));
				}
				
				pxNode = pxNode->GetSibling();
			}
		}
		return pFilter;
	}

	//////////////////////////////////////////////////////////////////////////
	// Read Expression
	//////////////////////////////////////////////////////////////////////////
	Expression*	FilterReaderImpl::ReadExpression(XNode* pxExpression)
	{
		Expression* pExpsion = NULL;

		const char* type = pxExpression->GetName();

		if(!g_stricmp(type, "PropertyName"))
		{
			pExpsion = ReadPropertyName(pxExpression);
		}
		else if(!g_stricmp(type, "Literal"))
		{
			pExpsion = ReadLiteral(pxExpression);
		}
		else if(!g_stricmp(type, "Arithmetic"))
		{

		}
		else if(!g_stricmp(type, "Function"))
		{

		}

		return pExpsion;
	}

	PropertyName* FilterReaderImpl::ReadPropertyName(XNode* pxPropertyName)
	{
		const char* content = pxPropertyName->GetContent();
		if(content==NULL)
		{
			return NULL;
		}

		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		PropertyName* pPropName = pFactory->CreatePropertyName(content);
		m_prop_name = content;

		return pPropName;
	}

	// Attention !!!
	// this function need be modified.
	// the type of GValue depends on the type of Field.
	// Field should pass to ReadLiteral as a parameter.
	Literal* FilterReaderImpl::ReadLiteral(XNode* pxNode)
	{
		const char* content = pxNode->GetContent();
		
		GValue* pValue = NULL;
		Literal* pLiteral = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();

		if(m_pFields==NULL)
		{
			pValue = new GValue(content);
		}
		else
		{
			GField* pField = m_pFields->GetField(m_prop_name.c_str());
			if(pField==NULL)
			{
				pValue = new GValue(content);
			}
			else
			{
				switch(pField->GetType())
				{					 
				case augeFieldTypeShort:
					{
						pValue = new GValue(atoi(content));
					}
					break;
				case augeFieldTypeInt:
					{
						pValue = new GValue(atoi(content));
					}
					break;
				case augeFieldTypeLong:
					{
						pValue = new GValue(atoi(content));
					}
					break;
				case augeFieldTypeInt64:
					{
						pValue = new GValue(atoi(content));
					}
					break;
				case augeFieldTypeFloat:
					{
						pValue = new GValue(atof(content));
					}
					break;
				case augeFieldTypeDouble:
					{
						pValue = new GValue(atof(content));
					}
					break;
				case augeFieldTypeChar:			 
					{
						pValue = new GValue(atoi(content));
					}
					break;
				case augeFieldTypeString:
					{
						pValue = new GValue(content);
					}
					break;
				case augeFieldTypeTime:	
					{

					}
					break;
				case augeFieldTypeBool:			 
					{

					}
					break;
				case augeFieldTypeBLOB:			 
					{

					}
					break;
				case augeFieldTypeGeometry:
					{
						//Geometry *pGeometry = pValue->GetGeometry();
						//if(pGeometry!=NULL)
						//{
						//	const char* wkt = pGeometry->AsText(true);
						//	if(wkt!=NULL)
						//	{	
						//		pValue = new GValue(content);
						//	}
						//}
					}
					break;
				}//switchss
			}
		}

		pLiteral = pFactory->CreateLiteral(pValue);

		return pLiteral;
	}

	bool FilterReaderImpl::ReadEnvelope(XNode* pxEnvelope, GEnvelope& extent)
	{
		XNode* pxNode = NULL;
		XNodeSet* pxNodeSet = NULL;
		const char* coordinate = NULL;

		pxNodeSet = pxEnvelope->GetChildren("lowerCorner");
		if(pxNodeSet==NULL)
		{
			return false;
		}
		pxNode = pxNodeSet->Item(0);
		if(pxNode==NULL)
		{
			return false;
		}
		coordinate = pxNode->GetContent();
		sscanf(coordinate, "%lf %lf", &extent.m_xmin, &extent.m_ymin);
		pxNodeSet->Release();
		
		pxNodeSet = pxEnvelope->GetChildren("upperCorner");
		if(pxNodeSet==NULL)
		{
			return false;
		}
		pxNode = pxNodeSet->Item(0);
		if(pxNode==NULL)
		{
			return false;
		}
		coordinate = pxNode->GetContent();
		sscanf(coordinate, "%lf %lf", &extent.m_xmax, &extent.m_ymax);
		pxNodeSet->Release();

		return true;
	}

	

	//typedef enum augeComparisonOperator
	//{
	//	augeComOprEqual					= 0,	// 相等
	//	augeComOprNotEqual				= 1,	// 不等
	//	augeComOprLessThan				= 2,	// 小于
	//	augeComOprGreaterThan			= 3,	// 大于
	//	augeComOprLessThanOrEqalTo		= 4,	// 小于等于
	//	augeComOprGreaterThanOrEqalTo	= 5,	// 大于等于
	//	augeComOprIsLike				= 6,
	//	augecomOprIsNull				= 7,
	//	augeComOprIsBetween				= 8
	//}augeComparisonOperator;
	augeComparisonOperator FilterReaderImpl::ParseComparisonOperator(const char* oper)
	{
		if(!g_stricmp(oper, "PropertyIsEqual"))
		{
			return augeComOprEqual;
		}
		else if(!g_stricmp(oper, "PropertyIsNotEqual"))
		{
			return augeComOprNotEqual;
		}
		else if(!g_stricmp(oper, "PropertyIsLessThan"))
		{
			return augeComOprLessThan;
		}
		else if(!g_stricmp(oper, "PropertyIsGreaterThan"))
		{
			return augeComOprGreaterThan;
		}
		else if(!g_stricmp(oper, "PropertyIsLessThanOrEqalTo"))
		{
			return augeComOprLessThanOrEqalTo;
		}
		else if(!g_stricmp(oper, "PropertyIsGreaterThanOrEqalTo"))
		{
			return augeComOprGreaterThanOrEqalTo;
		}
		else if(!g_stricmp(oper, "PropertyIsIsLike"))
		{
			return augeComOprIsLike;
		}
		else if(!g_stricmp(oper, "PropertyIsIsNull"))
		{
			return augecomOprIsNull;
		}
		else if(!g_stricmp(oper, "PropertyIsIsBetween"))
		{
			return augeComOprIsBetween;
		}

		return augeComOprEqual;
	}
}