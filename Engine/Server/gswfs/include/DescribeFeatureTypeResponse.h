#ifndef __AUGE_WFS_DESCRIBE_FEATURETYPE_RESPONSE_H__
#define __AUGE_WFS_DESCRIBE_FEATURETYPE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class DescribeFeatureTypeRequest;

	class DescribeFeatureTypeResponse : public WebResponse
	{
	public:
		DescribeFeatureTypeResponse(DescribeFeatureTypeRequest* pRequest);
		virtual ~DescribeFeatureTypeResponse();
	public:
		virtual	RESULTCODE		Write();
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetPath(const char* path);

	private:
		DescribeFeatureTypeRequest*	m_pRequest;
		std::string		m_path;
	};
}

#endif //__AUGE_WFS_DESCRIBE_FEATURETYPE_RESPONSE_H__
