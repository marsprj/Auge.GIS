#ifndef __AUGE_FEATURE_LAYER_H__
#define __AUGE_FEATURE_LAYER_H__

#include "Layer.h"

namespace auge
{
	class Layer;
	class FeatureType;
	class Style;

	class AUGE_API FeatureLayer : public Layer
	{
	public:
		FeatureLayer();
		virtual ~FeatureLayer();

	public:
		virtual LayerType	GetType();
		virtual Envelope&	GetExtent();

	public:		
		int					SetFeatureType(FeatureType* pFeatureType);
		FeatureType*		GetFeatureType();

		Style*				GetStyle();
		void				SetStyle(Style* pStyle);
		
	private:
		FeatureType	*m_pFeatureType;
		Style		*m_pStyle;
	};

}//namespace

#endif //__AUGE_FEATURE_LAYER_H__