#ifndef __AUGE_WFS_TRANSACTION_RESPONSE_H__
#define __AUGE_WFS_TRANSACTION_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class TransactionRequest;
	class FeatureCursor;

	class TransactionResponse : public WebResponse
	{
	public:
		TransactionResponse(TransactionRequest* pRequest);
		virtual ~TransactionResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetFeatureCursor(FeatureCursor* pCursor);

	private:
		TransactionRequest	*m_pRequest;
		FeatureCursor		*m_pCursor;

		std::string			m_encoding;
	};
}

#endif //__AUGE_WFS_TRANSACTION_RESPONSE_H__
