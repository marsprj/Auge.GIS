#ifndef __AUGE_ARCHIVE_IMPL_H__
#define __AUGE_ARCHIVE_IMPL_H__

#include "AugeType.h"
#include "AugeCore.h"
#include "AugeProcessor.h"
#include <vector>
#include <string>

namespace auge
{
	class ArchiveProcessorImpl : public ArchiveProcessor
	{
	public:
		ArchiveProcessorImpl();
		virtual ~ArchiveProcessorImpl();
	public:
		virtual void		SetUser(User* pUser);
		virtual void		AddFile(const char* fpath);
		virtual void		SetArchivePath(const char* apath);
		virtual void		Clear();

		virtual RESULTCODE	Compress();
		virtual RESULTCODE	Decompress();

		virtual RESULTCODE	Execute();

		virtual void		Release();

	public:
		const char*			GetArchivePath();

	private:
		std::vector<std::string>	m_paths;
		std::string m_archive_path;

		User*		m_pUser;
	};
}

#endif //__AUGE_ARCHIVE_IMPL_H__
