#include "CanvasImpl.h"
#include "AugeField.h"
#include "RendererCairo.h"
#include "LabelSet.h"
#include "PointLabel.h"
#include "LineLabel.h"
#include "PolygonLabel.h"

namespace auge
{
	CanvasImpl::CanvasImpl(g_uint width, g_uint height) 
	{ 
		m_width = width;
		m_height = height;
		m_transform.SetCanvas(width, height);
		m_viewer.Set(-180.0f, -90.0f, 180.0f, 90.0f);
		m_pRenderer = new RendererCairo(width, height);
		m_maplex.SetExtent(width, height);
		m_maplex.SetRenderer(m_pRenderer);
		m_maplex.SetTransformation(&m_transform);
	}

	CanvasImpl::~CanvasImpl()
	{
		if(m_pRenderer!=NULL)
		{
			m_pRenderer->Release();
			m_pRenderer = NULL;
		}
	}

	void CanvasImpl::SetViewer(GEnvelope& viewer)
	{
		m_transform.SetViewer(viewer);
		m_viewer = m_transform.GetViewer();
	}

	void CanvasImpl::Draw(Map* pMap)
	{
		if(pMap==NULL)
		{
			return;
		}
		Layer* pLayer = NULL;
		g_uint count = pMap->GetLayerCount();
		for(g_int i=count-1; i>=0; i--)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				if(pLayer->IsVisiable())
				{
					DrawLayer(pLayer);
				}
			}
		}
		m_maplex.Draw();
	}

	void CanvasImpl::DrawLayer(Layer* pLayer)
	{
		augeLayerType type = pLayer->GetType();
		switch(type)
		{
		case augeLayerFeature:
			{
				FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
				DrawLayer(pFLayer);
			}
			break;
		case augeLayerRaster:
			{
				RasterLayer* pRLayer = static_cast<RasterLayer*>(pLayer);
				DrawLayer(pRLayer);
			}
			break;
		}
		//DrawLayer(pLayer, pLayer->GetStyle());
	}

	void CanvasImpl::DrawLayer(FeatureLayer* pLayer)
	{
		DrawLayer(pLayer, static_cast<FeatureStyle*>(pLayer->GetStyle()));
	}

	void CanvasImpl::DrawLayer(RasterLayer* pLayer)
	{

	}

	void CanvasImpl::DrawLayer(Layer* pLayer, Style* pStyle)
	{
		augeLayerType type = pLayer->GetType();
		switch(type)
		{
		case augeLayerFeature:
			{
				FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
				if(pStyle->GetType()==augeStyleFeature)
				{
					DrawLayer(pFLayer,static_cast<FeatureStyle*>(pStyle));
				}
				
			}
			break;
		case augeLayerRaster:
			{
				RasterLayer* pRLayer = static_cast<RasterLayer*>(pLayer);
				DrawLayer(pRLayer);
			}
			break;
		}
	}

	void CanvasImpl::DrawLayer(FeatureLayer* pLayer, FeatureStyle* pStyle)
	{
		if(pStyle==NULL)
		{
			return;
		}
		Rule* pRule = NULL;
		g_uint count = pStyle->GetRuleCount();
		for(g_uint i=0; i<count; i++)
		{
			pRule = pStyle->GetRule(i);
			DrawLayer(pLayer, pRule);
		}
	}

	void CanvasImpl::DrawLayer(FeatureLayer* pLayer, Rule* pRule)
	{	
		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pLayer->GetFeatureClass();

		GFilter* pFilter = CreateViewFilter(pRule->GetFilter(), pFeatureClass);

		Symbolizer* pSymbolizer = NULL;
		pSymbolizer = pRule->GetSymbolizer();
		if(pSymbolizer!=NULL)
		{
			DrawLayer(pLayer, pSymbolizer, pFilter);
		}
		
		TextSymbolizer* pTextSymbolzer = NULL;
		pTextSymbolzer = pRule->GetTextSymbolizer();
		if(pTextSymbolzer!=NULL)
		{
			LabelSet* pLabelSet = m_maplex.GetLabelSet(pLayer->GetName());
			LabelLayer(pLabelSet, pLayer, pTextSymbolzer, pFilter);
		}
		pFilter->Release();
	}

	//GFilter* CanvasImpl::CreateViewFilter(Rule* pRule, FeatureClass* pFeatureClass)
	//{
	//	GFilter* pFilter = NULL;
	//	GFilter* pRuleFilter = NULL;
	//	FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
	//	pRuleFilter = pRule->GetFilter();
	//	if(pRuleFilter==NULL)
	//	{
	//		PropertyName* pPropName = pFilterFactory->CreatePropertyName(pFeatureClass->GetFields()->GetGeometryField()->GetName());
	//		pFilter = pFilterFactory->CreateBBoxFilter(pPropName, m_viewer);
	//	}
	//	else
	//	{
	//		GFilter* pBoxFilter = NULL;
	//		PropertyName* pPropName = pFilterFactory->CreatePropertyName(pFeatureClass->GetFields()->GetGeometryField()->GetName());
	//		pBoxFilter = pFilterFactory->CreateBBoxFilter(pPropName, m_viewer);

	//		BinaryLogicFilter* pLogFilter = pFilterFactory->CreateBinaryLogicFilter("and");
	//		pLogFilter->AddFilter(pBoxFilter);
	//		pLogFilter->AddFilter(pRuleFilter);
	//		pRuleFilter->AddRef();
	//		pFilter = pLogFilter;
	//	}

	//	return pFilter;
	//}

	GFilter* CanvasImpl::CreateViewFilter(GFilter* pSubFilter,FeatureClass* pFeatureClass)
	{
		GFilter* pFilter = NULL;
		FilterFactory* pFilterFactory = augeGetFilterFactoryInstance();
		if(pSubFilter==NULL)
		{
			PropertyName* pPropName = pFilterFactory->CreatePropertyName(pFeatureClass->GetFields()->GetGeometryField()->GetName());
			pFilter = pFilterFactory->CreateBBoxFilter(pPropName, m_viewer);
		}
		else
		{
			GFilter* pBoxFilter = NULL;
			PropertyName* pPropName = pFilterFactory->CreatePropertyName(pFeatureClass->GetFields()->GetGeometryField()->GetName());
			pBoxFilter = pFilterFactory->CreateBBoxFilter(pPropName, m_viewer);

			BinaryLogicFilter* pLogFilter = pFilterFactory->CreateBinaryLogicFilter("and");
			pLogFilter->AddFilter(pBoxFilter);
			pLogFilter->AddFilter(pSubFilter);
			pSubFilter->AddRef();
			pFilter = pLogFilter;
		}

		return pFilter;
	}

	void CanvasImpl::DrawLayer(FeatureLayer* pLayer, Symbolizer* pSymbolizer, GFilter* pFilter)
	{	
		FeatureCursor	*pCursor = NULL;
		FeatureClass	*pFeatureClass = pLayer->GetFeatureClass();
		
		pCursor = pFeatureClass->Query(pFilter);

		//GField* pField = pFeatureClass->GetFields()->GetGeometryField();
		//switch(pField->GetType())
		//{
		//case augeGTPoint:
		//case augeGTMultiPoint:
		//	DrawFeatures(static_cast<PointSymbolizer*>(pSymbolizer), pCursor);
		//	break;
		//case augeGTLineString:
		//case augeGTMultiLineString:
		//	DrawFeatures(static_cast<LineSymbolizer*>(pSymbolizer), pCursor);
		//	break;
		//case augeGTPolygon:
		//case augeGTMultiPolygon:
		//	DrawFeatures(static_cast<PolygonSymbolizer*>(pSymbolizer), pCursor);
		//	break;
		//}

		g_uchar* wkb = NULL;
		auge::Geometry	*pGeometry = NULL;
		auge::Feature	*pFeature = NULL;
		while((pFeature=pCursor->NextFeature())!=NULL)
		{	
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				wkb = pGeometry->AsBinary();
				m_pRenderer->Draw(wkb, pSymbolizer, &m_transform);
			}
			pFeature->Release();
		}
		pCursor->Release();
	}

	void CanvasImpl::DrawFeatures(PointSymbolizer* pSymbolizer, FeatureCursor* pCursor)
	{
		Graphic* pGraphic = pSymbolizer->GetGraphic();
		if(pGraphic!=NULL)
		{
			DrawGraphics(pSymbolizer, pCursor);
		}
		else
		{
			DrawPoints(pSymbolizer, pCursor);
		}
	}

	void CanvasImpl::DrawGraphics(PointSymbolizer* pSymbolizer, FeatureCursor* pCursor)
	{

	}

	void CanvasImpl::DrawPoints(PointSymbolizer* pSymbolizer, FeatureCursor* pCursor)
	{
		g_uchar* wkb = NULL;
		auge::Geometry	*pGeometry = NULL;
		auge::Feature	*pFeature = NULL;
		while((pFeature=pCursor->NextFeature())!=NULL)
		{	
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				wkb = pGeometry->AsBinary();
				m_pRenderer->Draw(wkb, pSymbolizer, &m_transform);
			}
			pFeature->Release();
		}
	}

	void CanvasImpl::DrawFeatures(LineSymbolizer* pSymbolizer, FeatureCursor* pCursor)
	{
		g_uchar* wkb = NULL;
		auge::Geometry	*pGeometry = NULL;
		auge::Feature	*pFeature = NULL;
		while((pFeature=pCursor->NextFeature())!=NULL)
		{	
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				wkb = pGeometry->AsBinary();
				m_pRenderer->Draw(wkb, pSymbolizer, &m_transform);
			}
			pFeature->Release();
		}
	}

	void CanvasImpl::DrawFeatures(PolygonSymbolizer* pSymbolizer, FeatureCursor* pCursor)
	{
		g_uchar* wkb = NULL;
		auge::Geometry	*pGeometry = NULL;
		auge::Feature	*pFeature = NULL;
		while((pFeature=pCursor->NextFeature())!=NULL)
		{	
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				wkb = pGeometry->AsBinary();
				m_pRenderer->Draw(wkb, pSymbolizer, &m_transform);
			}
			pFeature->Release();
		}
	}

	void CanvasImpl::LabelLayer(LabelSet* pLabelSet, FeatureLayer* pLayer, TextSymbolizer* pSymbolizer, GFilter* pFilter)
	{
		const char		*name = pLayer->GetName();

		FeatureCursor	*pCursor = NULL;
		FeatureClass	*pFeatureClass = pLayer->GetFeatureClass();

		GFilter* pViewFilter = CreateViewFilter(pFilter, pFeatureClass);
		
		const char* label_text = NULL;
		g_uchar* wkb = NULL;
		auge::Geometry	*pGeometry = NULL;
		auge::Feature	*pFeature = NULL;
		auge::GLabel	*pLabel = NULL;

		Font* pFont = pSymbolizer->GetFont();
		m_pRenderer->Save();
		m_pRenderer->SetFont(pFont->GetFamily(),
							pFont->GetSize(),
							pFont->GetStyle(),
							pFont->GetWeight());

		pCursor = pFeatureClass->Query(pViewFilter);
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{	
				pLabel = CreateLabel(pGeometry->GeometryType());
				pLabel->SetGeometry(pGeometry);
				pLabel->SetSymbolizer(pSymbolizer);
				label_text = pSymbolizer->GetLabelText();
				if((label_text==NULL)||(strlen(label_text)==0))
				{
					const char* fname = pSymbolizer->GetLabel();
					label_text = pFeature->GetString(fname);
					if(label_text!=NULL)
					{						
						pLabel->SetText(label_text);
					}
				}
				else
				{
					pLabel->SetText(label_text);
				}
				pLabel->ComputePosition(m_pRenderer, &m_transform);				
				//if(pLabel->IsVisible(m_width, m_height))
				{
					pLabel->AdjustPosition(m_width, m_height);
					if(!m_maplex.IsCollision(pLabel))
					{
						m_maplex.AddLabel(name, pLabel);
					}
					else
					{
						AUGE_SAFE_RELEASE(pLabel);
					}
				}
				/*else
				{
					AUGE_SAFE_RELEASE(pLabel);
				}*/
			}

			pFeature->Release();
		}
		pCursor->Release();
		m_pRenderer->Resotre();
		if(pViewFilter!=NULL)
		{
			AUGE_SAFE_RELEASE(pViewFilter);
		}
	}

	GLabel* CanvasImpl::CreateLabel(augeGeometryType type)
	{
		GLabel* pLabel = NULL;
		switch(type)
		{
		case augeGTPoint:
		case augeGTMultiPoint:
			pLabel = new PointLabel();
			break;
		case augeGTLineString:
		case augeGTMultiLineString:
			pLabel = new LineLabel();
			break;
		case augeGTPolygon:
		case augeGTMultiPolygon:
			pLabel = new PolygonLabel();
			break;
		}
		return pLabel;
	}

	void CanvasImpl::LabelLayer(FeatureLayer* pLayer, TextSymbolizer* pSymbolizer, GFilter* pFilter)
	{
		FeatureCursor	*pCursor = NULL;
		FeatureClass	*pFeatureClass = pLayer->GetFeatureClass();

		pCursor = pFeatureClass->Query();

		const char* label_text = NULL;
		g_uchar* wkb = NULL;
		auge::Geometry	*pGeometry = NULL;
		auge::Feature	*pFeature = NULL;
		while((pFeature=pCursor->NextFeature())!=NULL)
		{
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				wkb = pGeometry->AsBinary();

				label_text = pSymbolizer->GetLabelText();
				if((label_text==NULL)||(strlen(label_text)==0))
				{
					//label_text = pSymbolizer->GetLabel();
					const char* fname = pSymbolizer->GetLabel();
					label_text = pFeature->GetString(fname);
					if(label_text!=NULL)
					{
						LabelGeometry(wkb, pSymbolizer, label_text);
					}
				}
				else
				{
					LabelGeometry(wkb, pSymbolizer, label_text);
				}
			}

			pFeature->Release();
		}
		pCursor->Release();
	}


	void CanvasImpl::LabelGeometry(g_uchar* wkb, TextSymbolizer* pSymbolizer, const char* text)
	{
		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			LabelPoint((WKBPoint*)wkb, pSymbolizer,text);
			break;
		case wkbMultiPoint:
			break;
		case wkbLineString:
			break;
		case wkbMultiLineString:
			break;
		case wkbPolygon:
			break;
		case wkbMultiPolygon:
			break;
		}
	}

	void CanvasImpl::LabelPoint(WKBPoint* pWKBPoint, TextSymbolizer* pSymbolizer,const char* text)
	{
		m_pRenderer->Label(text, pWKBPoint, pSymbolizer, &m_transform);
	}

	RESULTCODE	CanvasImpl::Save(const char* path)
	{
		m_pRenderer->SaveAsImage(path);
		return AG_SUCCESS;
	}

	void CanvasImpl::DrawBackground(GColor& color)
	{
		m_pRenderer->FillRectangle(m_width, m_height, color);
	}

	Transformation* CanvasImpl::GetTransform()
	{
		return &m_transform;
	}

	Renderer* CanvasImpl::GetRenderer()
	{
		return m_pRenderer;
	}
	
	void CanvasImpl::Label()
	{
		m_maplex.Draw();
	}
}