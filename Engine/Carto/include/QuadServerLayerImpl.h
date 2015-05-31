#ifndef __AUGE_QUAD_SERVER_LAYER_IMPL_H__
#define __AUGE_QUAD_SERVER_LAYER_IMPL_H__

#include "AugeCarto.h"
#include <string>

namespace auge
{
	class QuadServerLayerImpl : public QuadServerLayer
	{
	public:
		QuadServerLayerImpl();
		virtual ~QuadServerLayerImpl();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* szName);
		virtual augeLayerType	GetType();

		virtual g_int			GetSRID();
		virtual GEnvelope&		GetExtent();

		virtual bool			IsQueryable();
		virtual void			SetQueryable(bool flag);

		virtual bool			IsVisiable();
		virtual void			SetVisiable(bool flag);

		virtual const char*		GetURL();
		virtual RESULTCODE		SetURL(const char* url);

	private:
		std::string m_name;
		std::string m_url;

		GEnvelope	m_extent;
		g_int		m_srid;
		bool		m_visible;
	};
}

#endif //__AUGE_QUAD_SERVER_LAYER_IMPL_H__
