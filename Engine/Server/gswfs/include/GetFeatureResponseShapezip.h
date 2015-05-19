#ifndef __AUGE_WFS_GET_FEATURE_SHAPE_ZIP_RESPONSE_H__
#define __AUGE_WFS_GET_FEATURE_SHAPE_ZIP_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class GetFeatureRequest;
	class FeatureCursor;
	class WebContext;

	class GetFeatureShapeZipResponse : public WebResponse
	{
	public:
		GetFeatureShapeZipResponse(GetFeatureRequest* pRequest);
		virtual ~GetFeatureShapeZipResponse();
	public:
		virtual	RESULTCODE		Write();
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void		SetWebContenxt(WebContext* pWebContext);
		void		SetFeatureCursor(FeatureCursor* pCursor);

	private:
		RESULTCODE	Export(const char* path, const char* className);
		RESULTCODE	WriteFile(WebWriter* pWriter, const char* fpath);

	private:
		GetFeatureRequest	*m_pRequest;
		WebContext			*m_pWebContext;
		FeatureCursor		*m_pCursor;
	};
}

#endif //__AUGE_WFS_GET_FEATURE_SHAPE_ZIP_RESPONSE_H__
