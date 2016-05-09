#ifndef __AUGE_IMS_JOB_STATISTICS_RESPONSE_H__
#define __AUGE_IMS_JOB_STATISTICS_RESPONSE_H__

#include "AugeWebEngine.h"
#include "AugeXML.h"
#include <string>

namespace auge
{
	class Job;
	class JobStatisticsRequest;

	class JobStatisticsResponse : public WebResponse
	{
	public:
		JobStatisticsResponse(JobStatisticsRequest* pRequest);
		virtual ~JobStatisticsResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	public:
		void	SetJob(Job* pJob);
		void	SetJobs(EnumJob* pJobs);
		void	SetWebContext(WebContext* pWebContext);
		void	SetStatistics(GStatistics* pStatistics);

	private:
		void	WriteStatistics(XDocument* pxDoc);
		
	private:
		JobStatisticsRequest	*m_pRequest;
		WebContext				*m_pWebContext;
		GStatistics				*m_pStatistics;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
