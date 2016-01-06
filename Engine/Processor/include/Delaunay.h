#ifndef __AUGE_DELAUNAY_H__
#define __AUGE_DELAUNAY_H__

#include "AugeType.h"

namespace auge
{
	typedef struct  
	{
		g_uint	id;
		double	x;
		double	y;
		double	z;
	}delaunay_node_t;

	typedef struct  
	{
		g_int	v1;
		g_int	v2;
	}delaunay_edge_t;

	typedef struct
	{
		g_int	v1;
		g_int	v2;
		g_int	v3;
	}delaunay_triangle_t;

	class Delaunay
	{
	public:
		Delaunay();
		virtual ~Delaunay();
	public:
		void	SetVertex(delaunay_node_t* vertexes, g_uint count);
		g_uint	GetVertexCount();
		g_uint	GetEdgeCount();
		g_uint	GetTriangleCount();
		delaunay_node_t*	 GetVertex();
		delaunay_edge_t*	 GetEdge();
		delaunay_triangle_t* GetTriangle();

		bool	Triangulate();

	private:
		void	ComputeExtent(double& xmin, double& ymin, double& xmax, double& ymax);
		void	GenerateSuperTriangle(double cx, double cy, double size);
		bool	CircumCircle(double xp,double yp, double x1,double y1,double x2,double y2,double x3,double y3, double *xc,double *yc,double *rsqr);
		bool	FindEdge(g_int v1, g_int v2);

	private:
		g_uint				m_vertex_count;
		delaunay_node_t*	m_vertexes;

		g_uint				m_edge_count;
		delaunay_edge_t*	m_edges;

		g_uint				m_triangle_count;
		delaunay_triangle_t* m_triangles;
	};
}

#endif //__AUGE_DELAUNAY_H__
