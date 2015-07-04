#include "Kmean.h"
#include "math.h"

namespace auge
{
	KMean::KMean()
	{
		m_cluster_count = 0;
		m_clusters = NULL;
		m_distances = NULL;

		m_point_count = 0;
		m_points = NULL;
		
		m_max_iteration = 100;
	}

	KMean::~KMean()
	{
		if(m_clusters!=NULL)
		{
			free(m_clusters);
			m_clusters = NULL;
		}
		if(m_points!=NULL)
		{
			free(m_points);
			m_points = NULL;
		}
		if(m_distances!=NULL)
		{
			free(m_distances);
			m_distances = NULL;
		}
	}

	bool KMean::SetPoints(g_uint count)
	{
		if(m_points!=NULL)
		{
			free(m_points);
			m_points = NULL;
		}
		m_point_count = count;
		m_points = (kmean_point_t*)malloc(sizeof(kmean_point_t)*m_point_count);
		memset(m_points,0,sizeof(kmean_point_t)*m_point_count);

		return true;
	}

	bool KMean::SetPoint(g_uint i, double x, double y)
	{
		if(i<m_point_count)
		{
			m_points[i].x = x;
			m_points[i].y = y;
		}
		return true;
	}

	g_uint KMean::GetPointCount()
	{
		return m_point_count;
	}

	bool KMean::GetPoint(g_uint i, double& x, double& y, g_uint& cluster)
	{
		if(i>=m_point_count)
		{
			return false;
		}
		x = m_points[i].x;
		y = m_points[i].y;
		cluster = m_points[i].cluster;
		return true;
	}

	bool KMean::SetClusters(g_uint count)
	{
		if(m_clusters!=NULL)
		{
			free(m_clusters);
			m_clusters = NULL;
		}
		m_cluster_count = count;
		m_clusters = (kmean_cluster_t*)malloc(sizeof(kmean_cluster_t) * m_cluster_count);
		memset(m_points, 0, sizeof(kmean_cluster_t) * m_cluster_count);

		m_distances = (double*)malloc(sizeof(double) * m_cluster_count);
		memset(m_distances, 0, sizeof(double) * m_cluster_count);

		return true;
	}

	g_uint KMean::GetClusterCount()
	{
		return m_cluster_count;
	}

	bool KMean::GetCluster(g_uint i, double& x, double& y)
	{
		if(i>=m_cluster_count)
		{
			return false;
		}
		x = m_clusters[i].x;
		y = m_clusters[i].y;
		return true;
	}

	bool KMean::Execute()
	{
		InitCluster();

		for(g_uint iter=0; iter<m_max_iteration; iter++)
		{
			bool exit = true;
			g_uint cluster_id = 0;
			kmean_point_t* pt = m_points;
			for(g_uint i=0; i<m_point_count; i++, pt++)
			{
				cluster_id = FindPointCluster(pt->x, pt->y);
				if(cluster_id != pt->cluster)
				{
					pt->cluster = cluster_id;
					exit = false;
				}
				//printf("%d\n", cluster_id);
			}

			if(exit)
			{
				break;
			}
			else
			{
				UpdateCluster();
			}
		}
		
#ifdef DEBUG

#endif
	return true;
	}

	g_uint KMean::FindPointCluster(double ptx, double pty)
	{
		//printf("------------------------------------------\n");

		g_uint cluster_id = 0;
		double min_distance = AUGE_DOUBLE_MAX;
		double distance=0.0f;
		kmean_cluster_t* c = m_clusters;
		for(g_uint j=0; j<m_cluster_count; j++, c++)
		{
			distance = Distance(ptx, pty, c->x, c->y);
			if(distance < min_distance)
			{
				min_distance = distance;
				cluster_id = j;
			}

			//printf("[%d]:%f\n",j,distance);
		}
		return cluster_id;
	}

	double KMean::Distance(double x0, double y0, double x1, double y1)
	{
		return fabs(x0-x1) + fabs(y0-y1);
	}

	void KMean::InitCluster()
	{
		double xmin=AUGE_DOUBLE_MAX;
		double ymin=AUGE_DOUBLE_MAX;
		double xmax=AUGE_DOUBLE_MIN;
		double ymax=AUGE_DOUBLE_MIN;

		// init cluter array
		GetPointBound(xmin, ymin, xmax, ymax);
		double w = xmax - xmin;
		double h = ymax - ymin;
		kmean_cluster_t* c = NULL;

		g_uint i=0;
		for(i=0; i<m_cluster_count; i++)
		{
			c = &(m_clusters[i]);
			c->x = xmin + ((double)(rand()) / RAND_MAX) * w;
			c->y = ymin + ((double)(rand()) / RAND_MAX) * h;

			printf("%f\t%f\n", c->x, c->y);
		}

		// init point cluster code
		for(i=0; i<m_point_count; i++)
		{
			m_points[i].changed = 0;
			m_points[i].cluster = -1;
		}
	}

	void KMean::CleanupCluster()
	{
		kmean_cluster_t* c = m_clusters;
		for(g_uint i=0; i<m_cluster_count; i++, c++)
		{
			c->x = 0.0f;
			c->y = 0.0f;
			c->count = 0;
		}
	}

	void KMean::UpdateCluster()
	{
		CleanupCluster();

		kmean_point_t* pt = m_points;
		for(g_uint i=0; i<m_point_count; i++, pt++)
		{
			m_clusters[pt->cluster].x += pt->x;
			m_clusters[pt->cluster].y += pt->y;
			m_clusters[pt->cluster].count++;
		}

		printf("//////////////////////////////////////////////////////////////////////////\n");
		kmean_cluster_t* c  = m_clusters;
		for(g_uint i=0; i<m_cluster_count; i++, c++)
		{
			m_clusters[i].x /= m_clusters[i].count;
			m_clusters[i].y /= m_clusters[i].count;

			printf("[%d]:\t%f\t%f\t%d\n", i, c->x, c->y, c->count);
		}		
	}

	void KMean::GetPointBound(double& xmin, double& ymin, double& xmax, double& ymax)
	{
		for(g_uint i=0; i<m_point_count; i++)
		{
			if(m_points[i].x < xmin)
				xmin = m_points[i].x;
			else if(m_points[i].x > xmax)
				xmax = m_points[i].x;

			if(m_points[i].y < ymin)
				ymin = m_points[i].y;
			else if(m_points[i].y > ymax)
				ymax = m_points[i].y;
		}
	}

	void KMean::PrintCluster()
	{
		//kmean_cluster_t* c = m_clusters;
		//for(g_uint i=0; i<m_cluster_count; i++, c++)
		//{
		//	printf("[%d]:%f\t%f\n", i, c->x, c->y);
		//}
	}
}

