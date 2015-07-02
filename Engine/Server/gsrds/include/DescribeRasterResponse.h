#ifndef __AUGE_RDS_DESCRIBE_RASTER_RESPONSE_H__
#define __AUGE_RDS_DESCRIBE_RASTER_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>


namespace auge
{	
	class RasterDataset;
	class DescribeRasterRequest;

	class DescribeRasterResponse : public WebResponse
	{
	public:
		DescribeRasterResponse(DescribeRasterRequest* pRequest);
		virtual ~DescribeRasterResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void SetRasterDataset(RasterDataset* prasterDataset);

	private:
		DescribeRasterRequest	*m_pRequest;
		RasterDataset			*m_pRasterDataset;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
