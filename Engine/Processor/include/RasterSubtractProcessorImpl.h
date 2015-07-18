#ifndef __AUGE_RASTER_SUBTRACT_IMPL_H__
#define __AUGE_RASTER_SUBTRACT_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class RasterSubtractProcessorImpl : public RasterSubtractProcessor
	{
	public:
		RasterSubtractProcessorImpl();
		virtual ~RasterSubtractProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual void		SetInputDataSource_1(const char* sourceName);
		virtual void		SetInputRaster_1(const char* rasterName);

		virtual void		SetInputDataSource_2(const char* sourceName);
		virtual void		SetInputRaster_2(const char* rasterName);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
				
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource_1();
		const char*			GetInputRaster_1();

		const char*			GetInputDataSource_2();
		const char*			GetInputRaster_2();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();

	private:
		Raster*				Subtract(Raster* pinRaster_1, Raster* pinRaster_2);
		RESULTCODE			Subtract(RasterBand* pinBand_1, RasterBand* pinBand_2);

	private:
		std::string	m_in_source_name_1;
		std::string	m_in_raster_name_1;

		std::string	m_in_source_name_2;
		std::string	m_in_raster_name_2;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		
		g_uint				m_user;
	};

}


#endif //__AUGE_RASTER_SUBTRACT_IMPL_H__
