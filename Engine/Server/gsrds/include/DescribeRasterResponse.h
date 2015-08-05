#ifndef __AUGE_RDS_DESCRIBE_RASTER_RESPONSE_H__
#define __AUGE_RDS_DESCRIBE_RASTER_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>


namespace auge
{	
	class Raster;
	class DescribeRasterRequest;

	class DescribeRasterResponse : public WebResponse
	{
	public:
		DescribeRasterResponse(DescribeRasterRequest* pRequest);
		virtual ~DescribeRasterResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void SetRaster(Raster* praster);

	private:
		DescribeRasterRequest	*m_pRequest;
		Raster					*m_pRaster;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
