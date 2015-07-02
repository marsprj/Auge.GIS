#ifndef __AUGE_RDS_GET_VALUE_RESPONSE_H__
#define __AUGE_RDS_GET_VALUE_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <vector>
#include <string>


namespace auge
{	
	class GetValueResponse : public WebResponse
	{
		typedef struct  
		{
			short	band;
			char	value[16];
		}pixel_value_t;
	public:
		GetValueResponse(GetValueRequest* pRequest);
		virtual ~GetValueResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
	
	public:
		void		Create(int bands);
		void		SetValue(int index, short band, const char* value);

	private:
		GetValueRequest		*m_pRequest;
		pixel_value_t		*m_values;
		g_uint				m_bands;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
