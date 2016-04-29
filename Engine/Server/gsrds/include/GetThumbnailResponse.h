#ifndef __AUGE_RDS_GET_THUMBNAIL_RESPONSE_H__
#define __AUGE_RDS_GET_THUMBNAIL_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>


namespace auge
{	
	class Raster;
	class EnumRaster;
	class GetThumbnailRequest;

	class GetThumbnailResponse : public WebResponse
	{
	public:
		GetThumbnailResponse(GetThumbnailRequest* pRequest);
		virtual ~GetThumbnailResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void SetRaster(Raster* pRaster);
		void SetRasters(EnumRaster* pRasters);

	private:
		void WriteRaster(XElement* pxParent);
		void WriteRasters(XElement* pxParent);
		void AddRasterNode(XElement* pxRaster, Raster* pRaster);

	private:
		GetThumbnailRequest	*m_pRequest;
		Raster				*m_pRaster;
		EnumRaster			*m_pRasters;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
