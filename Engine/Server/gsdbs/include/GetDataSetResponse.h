#ifndef __AUGE_IMS_GET_DATASET_RESPONSE_H__
#define __AUGE_IMS_GET_DATASET_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Service;
	class FeatureClass;
	class Workspace;
	class DataSet;
	class GetDataSetRequest;
	class EnumDataSet;

	class GetDataSetResponse : public WebResponse
	{
	public:
		GetDataSetResponse(GetDataSetRequest* pRequest);
		virtual ~GetDataSetResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	public:
		void		SetDataSets(EnumDataSet* pDataSets);
		void		SetDataSet(DataSet* pDataSet);
		void		SetWebContext(WebContext* pWebContext);

		RESULTCODE	Write(WebWriter* pWriter, DataSet* pDataSet);
		RESULTCODE	Write(WebWriter* pWriter, EnumDataSet* pDataSets);

	private:
		void		AddFeatureClassNode(XElement*pxClass, FeatureClass* pFeatureClass);

	private:
		DataSet* m_pDataSet;
		EnumDataSet* m_pDataSets;
		GetDataSetRequest *m_pRequest;
		WebContext* m_pWebContext;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
