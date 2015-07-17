#ifndef __AUGE_RASTER_EDGE_DETECT_IMPL_H__
#define __AUGE_RASTER_EDGE_DETECT_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class RasterEdgeDetectProcessorImpl : public RasterEdgeDetectProcessor
	{
	public:
		RasterEdgeDetectProcessorImpl();
		virtual ~RasterEdgeDetectProcessorImpl();
	public:
		virtual void		SetUser(User* pUser);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);

		virtual void		SetEdgeDetector(augeEdgeDetector detector);
		
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();

	private:
		Raster*				Sobel(Raster* pRaster);
		Raster*				Sobel_1(Raster* pinRaster);
		Raster*				Sobel_3(Raster* pinRaster);

		RESULTCODE			Sobel(RasterBand* pBand);
		RESULTCODE			Sobel(RasterBand* pinBand, RasterBand* poutBand);
		RESULTCODE			Sobel_Short(RasterBand* pinBand, RasterBand* poutBand);

		Raster*				Sobel_Byte(Raster* pinRaster);

		void				Normalize(short* data, int width, int height, short v_min, short v_max);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;

		augeEdgeDetector m_detector;

		User*		m_pUser;
	};

}


#endif //__AUGE_RASTER_EDGE_DETECT_IMPL_H__
