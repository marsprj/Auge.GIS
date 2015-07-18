#ifndef __AUGE_RASTER_REVERSE_IMPL_H__
#define __AUGE_RASTER_REVERSE_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class RasterReverseProcessorImpl : public RasterReverseProcessor
	{
	public:
		RasterReverseProcessorImpl();
		virtual ~RasterReverseProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
				
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();

	private:
		Raster*				Reverse(Raster* pinRaster);
		RESULTCODE			Reverse(RasterBand* pBand);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		
		g_uint				m_user;
	};

}


#endif //__AUGE_RASTER_REVERSE_IMPL_H__
