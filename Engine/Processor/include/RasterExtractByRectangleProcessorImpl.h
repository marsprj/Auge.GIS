#ifndef __AUGE_RASTER_EXTRACT_BY_RECT_IMPL_H__
#define __AUGE_RASTER_EXTRACT_BY_RECT_IMPL_H__

#include "AugeGeometry.h"
#include "AugeProcessor.h"
#include <string>

namespace auge
{
	class RasterExtractByRectangleProcessorImpl : public RasterExtractByRectangleProcessor
	{
	public:
		RasterExtractByRectangleProcessorImpl();
		virtual ~RasterExtractByRectangleProcessorImpl();
	public:
		virtual void		SetUser(User* pUser);

		virtual void		SetInputRectangle(GEnvelope& rect);

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
		GEnvelope	m_rect;

		std::string	m_in_source_name;
		std::string	m_in_raster_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;

		User*		m_pUser;
	};

}


#endif //__AUGE_RASTER_EXTRACT_BY_RECT_IMPL_H__
