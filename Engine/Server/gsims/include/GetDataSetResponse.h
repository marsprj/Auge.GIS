#ifndef __AUGE_IMS_GET_DATASET_RESPONSE_H__
#define __AUGE_IMS_GET_DATASET_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Service;
	class Workspace;
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
		void	SetDataSets(EnumDataSet* pDataSets);

	private:
		EnumDataSet* m_pDataSets;
		GetDataSetRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
