#ifndef __AUGE_DBSCAN_H__
#define __AUGE_DBSCAN_H__

#include "AugeTile.h"
#include "wkb.h"

namespace auge
{
	typedef struct dbscan_point_t
	{
		float	x;
		float	y;
		g_int	id;
		g_byte	category;
		g_byte	visited;
		g_byte	included;
		g_byte	noise;		
	}dbscan_point_t;
	
	class DBScan
	{	
	public:
		DBScan();
		virtual ~DBScan();

	public:
		bool	SetPoints(g_uint count);
		bool	SetPoint(g_uint i, double x, double y);
		g_uint	GetPointCount();

		bool	Execute(float eps, g_uint min_pts);

	private:
		g_uint	FindNeighbours(dbscan_point_t* ptr);

	private:
		float			m_eps;
		g_uint			m_min_pts;

		g_uint			m_point_count;
		dbscan_point_t*	m_points;
	};
}


#endif //__AUGE_DBSCAN_H__
