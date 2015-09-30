#ifndef __AUGE_IMS_DESCRIBE_CSV_RESPONSE_H__
#define __AUGE_IMS_DESCRIBE_CSV_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class AttributeDataSet;
	class DescribeCsvRequest;

	class DescribeCsvResponse : public WebResponse
	{
	public:
		DescribeCsvResponse(DescribeCsvRequest* pRequest);
		virtual ~DescribeCsvResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetDataset(AttributeDataSet* pDataset);
		void	SetWebContext(WebContext* pWebContext);

	private:
		void	WriteDocument(XDocument* pxDoc);
		void	Cleanup();
		
	private:
		DescribeCsvRequest	*m_pRequest;
		WebContext			*m_pWebContext;
		AttributeDataSet	*m_pDataset;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
