#ifndef __AUGE_CLUSTER_KMEAN_H__
#define __AUGE_CLUSTER_KMEAN_H__

#include "AugeType.h"
#include "wkb.h"

namespace auge
{
	class KMean
	{
	private:
		typedef struct
		{
			double	x;
			double	y;
			g_int	cluster;
			g_int	changed;
		}kmean_point_t;

		typedef struct
		{
			double	x;
			double	y;
			g_uint	count;
		}kmean_cluster_t;
	public:
		KMean();
		virtual ~KMean();
	public:
		bool			SetPoints(g_uint count);
		bool			SetPoint(g_uint i, double x, double y);
		g_uint			GetPointCount();
		bool			GetPoint(g_uint i, double& x, double& y, g_uint& cluster);
		
		bool			SetClusters(g_uint count);
		bool			GetCluster(g_uint i, double& x, double& y);
		g_uint			GetClusterCount();

		bool			Execute();

	private:
		void			InitCluster();
		void			CleanupCluster();
		void			UpdateCluster();

		void			GetPointBound(double& xmin, double& ymin, double& xmax, double& ymax);
		g_uint			FindPointCluster(double ptx, double pty);

		double			Distance(double x0, double y0, double x1, double y1);

		void			PrintCluster();
	private:
		g_uint			m_cluster_count;
		kmean_cluster_t* m_clusters;
		double*			m_distances;

		g_uint			m_point_count;
		kmean_point_t*	m_points;
		
		g_uint			m_max_iteration;
	};
}

#endif //__AUGE_CLUSTER_KMEAN_H__
