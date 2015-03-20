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
		virtual	RESULTCODE		Write();
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		
		void	SetInsertCount(g_uint count);
		void	SetUpdateCount(g_uint count);
		void	SetDeleteCount(g_uint count);

	private:
		TransactionRequest	*m_pRequest;
		std::string			m_encoding;

		g_uint m_num_insert;
		g_uint m_num_update;
		g_uint m_num_delete;

	};
}

#endif //__AUGE_WFS_TRANSACTION_RESPONSE_H__
