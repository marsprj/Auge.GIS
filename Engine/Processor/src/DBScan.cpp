#include "DBScan.h"

//http://blog.csdn.net/roger__wong/article/details/40017463

namespace auge
{
	DBScan::DBScan()
	{
		m_eps = 0;
		m_min_pts = 5; 
		m_point_count = 0;
		m_points = NULL;
	}

	DBScan::~DBScan()
	{
		if(m_points!=NULL)
		{
			free(m_points);
			m_points = NULL;
		}
	}

	bool DBScan::Execute(float eps, g_uint min_pts)
	{
		if(eps<=0.0f||min_pts==0)
		{
			return false;
		}

		if(m_point_count==0||m_points==NULL)
		{
			return false;
		}

		g_uint neighbours = 0;
		dbscan_point_t* ptr = m_points;
		for(g_uint i=0; i<m_point_count; i++, ptr++)
		{
			if(!ptr->visited)
			{
				ptr->visited = 1;
				neighbours = FindNeighbours(ptr);
				
				if(neighbours < m_min_pts)
				{

				}
			}
		}

		return true;
	}

	bool DBScan::SetPoints(g_uint count)
	{
		if(m_points!=NULL)
		{
			free(m_points);
			m_points = NULL;
		}
		m_point_count = count;
		m_points = (dbscan_point_t*)malloc(sizeof(dbscan_point_t)*m_point_count);
		memset(m_points,0,sizeof(dbscan_point_t)*m_point_count);
		return true;
	}

	bool DBScan::SetPoint(g_uint i, double x, double y)
	{
		if(i<m_point_count)
		{
			m_points[i].x = x;
			m_points[i].y = y;
		}
		return true;
	}

	g_uint	DBScan::GetPointCount()
	{
		return m_point_count;
	}

	g_uint DBScan::FindNeighbours(dbscan_point_t* ptr)
	{
		return 1;
	}
}