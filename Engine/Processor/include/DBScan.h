#ifndef __AUGE_DBSCAN_H__
#define __AUGE_DBSCAN_H__

#include "AugeTile.h"
#include "wkb.h"

namespace auge
{
	class DBScan
	{
	public:
		DBScan();
		virtual ~DBScan();

	public:
		bool			Execute();
	};
}


#endif //__AUGE_DBSCAN_H__
