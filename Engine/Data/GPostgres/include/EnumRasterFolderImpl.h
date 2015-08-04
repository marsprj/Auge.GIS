#ifndef __AUGE_ENUM_RASTER_FOLDER_IMPL_H__
#define __AUGE_ENUM_RASTER_FOLDER_IMPL_H__

#include "AugeData.h"
#include <vector>

namespace auge
{
	class EnumRasterFolderImpl : public EnumRasterFolder
	{
	public:
		EnumRasterFolderImpl();
		virtual ~EnumRasterFolderImpl();
	public:
		virtual void			Reset();
		virtual RasterFolder*	Next();
		virtual bool			Add(RasterFolder* pRasterFolder);
		virtual void			Release();

	private:
		std::vector<RasterFolder*>	m_RasterFolders;
		std::vector<RasterFolder*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_RASTER_FOLDER_IMPL_H__
