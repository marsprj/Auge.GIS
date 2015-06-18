#include "FilterReaderImpl.h"
#include "AugeCore.h"
#include "AugeField.h"

#include "BinarySpatialFilterImpl.h"
#include "DistanceBufferFilterImpl.h"

namespace auge
{
	FilterReaderImpl::FilterReaderImpl(GFields* pFields):
	m_pFields(pFields)
	{

	}

	FilterReaderImpl::~FilterReaderImpl()
	{

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
		else if((!g_stricmp(type, "GmlObjectId"))||(!g_stricmp(type, "FeatureId")))
		{
			pFilter = ReadIdFilter(pxFilter);
		}
		else if(!g_stricmp(type, "Equals"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpEquals);
		}
		else if(!g_stricmp(type, "Disjoint"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpDisjoint);
		}
		else if(!g_stricmp(type, "Touches"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpTouchs);
		}
		else if(!g_stricmp(type, "Within"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpWithin);
		}
		else if(!g_stricmp(type, "Overlaps"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpOverlaps);
		}
		else if(!g_stricmp(type, "Crosses"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpCrosses);
		}
		else if(!g_stricmp(type, "Intersects"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpIntersects);
		}
		else if(!g_stricmp(type, "Contains"))
		{
			pFilter = ReadBinarySpatial(pxFilter, augeSpContains);
		}
		else if(!g_stricmp(type, "DWithin"))
		{
			pFilter = ReadDistanceSpatial(pxFilter, augeSpDWithin);
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
			else if(g_stricmp(nodeName,"Box")==0)
			{
				GEnvelope extent;
				if(ReadBox(pxNode, extent))
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

	/*
	<?xml version="1.0" encoding="UTF-8"?>
	<ogc:Filter xmlns:ogc="http://www.opengis.net/ogc" xmlns:gml="http://www.opengis.net/gml">
		<Within>
			<PropertyName>shape</PropertyName>
			<gml:Point name="1" srsName="EPSG:63266405">
				<gml:coordinates xmlns:gml="http://www.opengis.net/gml" decimal="." cs="," ts=" ">33.0860405,68.96354675</gml:coordinates>
			</gml:Point>
		</Within>
	</ogc:Filter>
	*/
	GFilter* FilterReaderImpl::ReadBinarySpatial(XNode* pxSpatial, augeSpatialOperator oper)
	{
		BinarySpatialFilterImpl* pFilter = NULL;
		pFilter = new BinarySpatialFilterImpl();
		pFilter->SetOperator(oper);

		const char* name = pxSpatial->GetName();

		XNodeSet* pxNodes = pxSpatial->GetChildren();
		if(pxNodes==NULL)
		{
			return NULL;
		}

		XNode* pxNode = NULL;
		const char* nodeName = NULL;
		g_uint count = pxNodes->Count();
		for(g_uint i=0; i<count; i++)
		{
			pxNode = pxNodes->Item(i);
			nodeName = pxNode->GetName();
			if(g_stricmp(nodeName,"Envelope")==0)
			{
				GEnvelope extent;
				if(ReadEnvelope(pxNode, extent))
				{
					pFilter->SetExtent(extent);
					continue;
				}
			}

			Expression* pPropName = ReadExpression(pxNode);
			if(pPropName)
			{
				pFilter->SetPropertyName(static_cast<PropertyName*>(pPropName));
				continue;
			}

			Geometry* pGeometry = NULL;
			GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
			GMLReader* reader = pGeometryFactory->CreateGMLReader();
			pGeometry = reader->Read((XElement*)pxNode);
			reader->Release();
			if(pGeometry!=NULL)
			{
				pFilter->SetGeometry(pGeometry);
				continue;
			}
		}
		pxNodes->Release();

		return pFilter;
	}
	
	GFilter* FilterReaderImpl::ReadDistanceSpatial(XNode* pxSpatial, augeSpatialOperator oper)
	{
		DistanceBufferFilterImpl* pFilter = NULL;
		pFilter = new DistanceBufferFilterImpl();
		pFilter->SetOperator(oper);

		const char* name = pxSpatial->GetName();

		XNodeSet* pxNodes = pxSpatial->GetChildren();
		if(pxNodes==NULL)
		{
			return NULL;
		}

		XNode* pxNode = NULL;
		const char* nodeName = NULL;
		g_uint count = pxNodes->Count();
		for(g_uint i=0; i<count; i++)
		{
			pxNode = pxNodes->Item(i);
			nodeName = pxNode->GetName();
			if(g_stricmp(nodeName,"Envelope")==0)
			{
				GEnvelope extent;
				if(ReadEnvelope(pxNode, extent))
				{
					pFilter->SetExtent(extent);
					continue;
				}
			}
			else if(g_stricmp(nodeName,"Distance")==0)
			{
				double distance = atof(pxNode->GetContent());
				pFilter->SetDistance(distance);

				XElement* pxElem = static_cast<XElement*>(pxNode);
				XAttribute* pxAttr = pxElem->GetAttribute("unit");
				if(pxAttr!=NULL)
				{
					const char* unit = pxAttr->GetValue();
					pFilter->SetUnit(auge_uint_type_decode(unit));
				}
			}

			Expression* pPropName = ReadExpression(pxNode);
			if(pPropName)
			{
				pFilter->SetPropertyName(static_cast<PropertyName*>(pPropName));
				continue;
			}

			Geometry* pGeometry = NULL;
			GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
			GMLReader* reader = pGeometryFactory->CreateGMLReader();
			pGeometry = reader->Read((XElement*)pxNode);
			reader->Release();
			if(pGeometry!=NULL)
			{
				pFilter->SetGeometry(pGeometry);
				continue;
			}
		}
		pxNodes->Release();

		return pFilter;
	}

	GFilter* FilterReaderImpl::ReadBinaryComparison(XNode* pxComparison)
	{
		GFilter* pFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();

		const char* type = pxComparison->GetName();
		augeComparisonOperator oper = ParseComparisonOperator(type);
		
		switch (oper)
		{
		case augeComOprIsBetween:
			{
				pFilter = ReadBetweenFilter(pxComparison);
			}
			break;
		case augeComOprIsLike:
			{
				pFilter = ReadLikeFilter(pxComparison);
			}
			break;
		default:
			{
				pFilter = ReadComparison(pxComparison, oper);
				//BinaryComparisonFilter* pBinaryFilter = NULL;
				//pBinaryFilter = pFactory->CreateBinaryComparisonFilter();
				//pBinaryFilter->SetOperator(oper);
				//XNode *pxNode = NULL;
				//XNodeSet* pxNodeSet = pxComparison->GetChildren();
				//if(pxNodeSet==NULL)
				//{
				//	pBinaryFilter->Release();
				//	pxNodeSet->Release();
				//	return NULL;
				//}

				//Expression* pExpression = NULL;

				//const char* nodeName = NULL;
				//pxNodeSet->Reset();
				//pxNode = pxNodeSet->Next();
				//if(pxNode==NULL)
				//{
				//	pxNodeSet->Release();
				//	return NULL;
				//}
				//pExpression = ReadExpression(pxNode);
				//if(pExpression==NULL)
				//{
				//	pxNodeSet->Release();
				//	return NULL;
				//}
				//pBinaryFilter->SetExpression1(pExpression);

				//pxNode = pxNodeSet->Next();
				//if(pxNode==NULL)
				//{
				//	pBinaryFilter->Release();
				//	pxNodeSet->Release();
				//	return NULL;
				//}
				//pExpression = ReadExpression(pxNode);
				//if(pExpression==NULL)
				//{
				//	pBinaryFilter->Release();
				//	pxNodeSet->Release();
				//	return NULL;
				//}
				//pBinaryFilter->SetExpression2(pExpression);
				//pFilter = pBinaryFilter;

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
				//pxNodeSet->Release();
			}
		}
		return pFilter;
	}

	IsBetweenFilter* FilterReaderImpl::ReadBetweenFilter(XNode* pxComparison)
	{
		IsBetweenFilter* pFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		
		XNode* pxPropName = pxComparison->GetFirstChild("PropertyName");
		if(pxPropName==NULL)
		{
			pxPropName->Release();
			return NULL;
		}
		
		PropertyName* pPropName = static_cast<PropertyName*>(ReadExpression(pxPropName));
		if(pPropName==NULL)
		{
			return NULL;
		}
		m_prop_name = pPropName->GetName();
		pFilter = pFactory->CreateIsBetweenFilter();
		pFilter->SetExpression(pPropName);
		//pPropName->Release();

		XNode *pxNode = NULL;
		XNodeSet* pxNodeSet = pxComparison->GetChildren();
		if(pxNodeSet==NULL)
		{
			pFilter->Release();
			pxNodeSet->Release();
			return NULL;
		}

		const char* nodeName = NULL;
		
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next())!=NULL)
		{
			nodeName = pxNode->GetName();
			if(!g_stricmp(nodeName,"UpperBoundary"))
			{
				XNode* pxLiteral = pxNode->GetFirstChild();
				if(pxLiteral==NULL)
				{
					pFilter->Release();
					break;
				}
				Expression* pExpression = ReadExpression(pxLiteral);
				//pxLiteral->Release();
				if(pExpression==NULL)
				{
					pFilter->Release();
					break;
				}
				pFilter->SetUpperBound(pExpression);
				pExpression->Release();
			}
			else if(!g_stricmp(nodeName,"LowerBoundary"))
			{
				XNode* pxLiteral = pxNode->GetFirstChild();
				if(pxLiteral==NULL)
				{
					pFilter->Release();
					break;
				}
				Expression* pExpression = ReadExpression(pxLiteral);
				//pxLiteral->Release();
				if(pExpression==NULL)
				{
					pFilter->Release();
					break;
				}
				pFilter->SetLowerBound(pExpression);
				pExpression->Release();
			}
			
		}

		pxNodeSet->Release();

		return pFilter;
	}

	IsLikeFilter* FilterReaderImpl::ReadLikeFilter(XNode* pxLike)
	{
		IsLikeFilter* pFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();

		XNode* pxPropName = pxLike->GetFirstChild("PropertyName");
		if(pxPropName==NULL)
		{	
			return NULL;
		}

		XNode* pxLiteral = pxLike->GetFirstChild("Literal");
		if(pxLiteral==NULL)
		{
			return NULL;
		}

		PropertyName* pPropName = static_cast<PropertyName*>(ReadExpression(pxPropName));
		if(pPropName==NULL)
		{	
			return NULL;
		}
		const char* literal = pxLiteral->GetContent();
		Literal* pLiteral = pFactory->CreateLiteral(new GValue(literal));
		
		m_prop_name = pPropName->GetName();
		pFilter = pFactory->CreateIsLikeFilter(pPropName, pLiteral);
		pPropName->Release();
		pLiteral->Release();

		return pFilter;
	}

	GFilter* FilterReaderImpl::ReadComparison(XNode* pxComparison,augeComparisonOperator oper)
	{
		BinaryComparisonFilter* pBinaryFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		pBinaryFilter = pFactory->CreateBinaryComparisonFilter();
		pBinaryFilter->SetOperator(oper);
		XNode *pxNode = NULL;
		XNodeSet* pxNodeSet = pxComparison->GetChildren();
		if(pxNodeSet==NULL)
		{
			pBinaryFilter->Release();
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
		pBinaryFilter->SetExpression1(pExpression);

		pxNode = pxNodeSet->Next();
		if(pxNode==NULL)
		{
			pBinaryFilter->Release();
			pxNodeSet->Release();
			return NULL;
		}
		pExpression = ReadExpression(pxNode);
		if(pExpression==NULL)
		{
			pBinaryFilter->Release();
			pxNodeSet->Release();
			return NULL;
		}
		pxNodeSet->Release();
		pBinaryFilter->SetExpression2(pExpression);
		return pBinaryFilter;
	}

	GFilter* FilterReaderImpl::ReadBinaryLogical(XNode* pxLogical)
	{
		BinaryLogicFilter* pBinaryFilter = NULL;
		FilterFactory* pFactory = augeGetFilterFactoryInstance();
		pBinaryFilter = pFactory->CreateBinaryLogicFilter(pxLogical->GetName());
		if(pBinaryFilter==NULL)
		{
			return NULL;
		}

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
			if(pxAttr==NULL)
			{
				pxAttr = ((XElement*)pxNode)->GetAttribute("fid",NULL);
			}
			if(pxAttr!=NULL)
			{
				str = pxAttr->GetValue();
				//sscanf(str,"%s.%d",typeName,&id);
				pos = auge_find_last_char(str,'.');
				if(pos>0)
				{
					pFilter->Add(atoi(str+pos+1));
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
		else if(!g_stricmp(type, "Function"))
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
						pValue = new GValue(content==NULL?"":content);
						//pValue = new GValue(content);
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

	//<gml:Envelope srsName="http://www.opengis.net/gml/srs/epsg.xml#4326">
	//	<gml:lowerCorner>-75.102613 40.212597</gml:lowerCorner>
	//	<gml:upperCorner>-72.361859 41.512517</gml:upperCorner>
	//</gml:Envelope>
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

	//<gml:Box srsName="http://www.opengis.net/gml/srs/epsg.xml#4326">
	//	<gml:coordinates>-75.102613,40.212597 -72.361859,41.512517</gml:coordinates>
	//</gml:Box>
	bool FilterReaderImpl::ReadBox(XNode* pxBox, GEnvelope& extent)
	{
		XNode* pxNode = NULL;
		const char* coordinate = NULL;

		pxNode = pxBox->GetFirstChild("coordinates");
		if(pxNode==NULL)
		{
			return false;
		}
		coordinate = pxNode->GetContent();
		sscanf(coordinate, "%lf,%lf %lf,%lf", &extent.m_xmin, &extent.m_ymin, &extent.m_xmax, &extent.m_ymax);

		return true;
	}
	

	//typedef enum augeComparisonOperator
	//{
	//	augeComOprEqual					= 0,	// 相等
	//	augeComOprNotEqual				= 1,	// 不等
	//	augeComOprLessThan				= 2,	// 小于
	//	augeComOprGreaterThan			= 3,	// 大于
	//	augeComOprLessThanOrEqualTo		= 4,	// 小于等于
	//	augeComOprGreaterThanOrEqualTo	= 5,	// 大于等于
	//	augeComOprIsLike				= 6,
	//	augecomOprIsNull				= 7,
	//	augeComOprIsBetween				= 8
	//}augeComparisonOperator;
	augeComparisonOperator FilterReaderImpl::ParseComparisonOperator(const char* oper)
	{
		if(!g_stricmp(oper, "PropertyIsEqualTo"))
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
		else if(!g_stricmp(oper, "PropertyIsLessThanOrEqualTo"))
		{
			return augeComOprLessThanOrEqualTo;
		}
		else if(!g_stricmp(oper, "PropertyIsGreaterThanOrEqualTo"))
		{
			return augeComOprGreaterThanOrEqualTo;
		}
		else if(!g_stricmp(oper, "PropertyIsIsLike"))
		{
			return augeComOprIsLike;
		}
		else if(!g_stricmp(oper, "PropertyIsIsNull"))
		{
			return augecomOprIsNull;
		}
		else if(!g_stricmp(oper, "PropertyIsBetween"))
		{
			return augeComOprIsBetween;
		}

		return augeComOprEqual;
	}

	void FilterReaderImpl::Release()
	{
		delete this;
	}
}