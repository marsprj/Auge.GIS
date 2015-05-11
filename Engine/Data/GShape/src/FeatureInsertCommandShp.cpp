#include "FeatureInsertCommandShp.h"
#include "FeatureClassShp.h"
#include "AugeField.h"

namespace auge
{
	#define	SizeOfLinearRing(ptr)	((ptr)->numPoints - 1) * sizeof(Point) + sizeof(LinearRing)

	FeatureInsertCommandShp::FeatureInsertCommandShp(FeatureClassShp* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
	}

	FeatureInsertCommandShp::~FeatureInsertCommandShp()
	{

	}

	RESULTCODE FeatureInsertCommandShp::Prepare(FeatureClass* pFeatureClass)
	{
		return AG_SUCCESS;
	}

	RESULTCODE FeatureInsertCommandShp::Insert(Feature* pFeature)
	{
		if(pFeature==NULL)
		{
			return AG_FAILURE;
		}

		Geometry* pGeometry = NULL;
		pGeometry = pFeature->GetGeometry();
		SHPObject* pshpObject = NULL;
		pshpObject  = CreateSHPObjectFromWKB(pGeometry->AsBinary(), 1);
		if(pshpObject==NULL)
		{
			return AG_FAILURE;
		}

		SHPHandle pshpHande = m_pFeatureClass->m_pshpHandle;
		g_int shp_id = ::SHPWriteObject(pshpHande, -1, pshpObject);
		if(shp_id<0)
		{
			return AG_FAILURE;
		}

		const char* fname = NULL;
		augeFieldType ftype;
		GField*  pField  = NULL;
		GFields* pFields = m_pFeatureClass->GetFields();
		GValue*  pValue = NULL;
		
		g_int i_index = pFields->FindField("gid");
		g_int g_index = pFields->FindField(pFields->GetGeometryField()->GetName());
		g_int f_index = 0;
		g_int dbf_index = 0;

		g_uint count = pFields->Count();
		for(g_uint i=0; i<count; i++)
		{
			pField = pFields->GetField(i);
			ftype = pField->GetType();
			if(ftype==augeFieldTypeGeometry)
			{
				continue;
			}
			fname = pField->GetName();
			if(i==i_index)
			{
				continue;
			}

			f_index = pFields->FindField(fname);
			dbf_index = GetFieldInnerIndex(f_index, i_index, g_index);
			if(dbf_index<0)
			{
				continue;
			}

			pValue = pFeature->GetValue(fname);
			if(pValue!=NULL)
			{
				WriteFieldValue(shp_id, dbf_index, ftype, pValue);
				pValue->Release();
			}
		}

		return AG_SUCCESS;
	}

	SHPObject* FeatureInsertCommandShp::CreateSHPObjectFromWKB(const g_uchar* pWKB, const int iWKBLen)
	{
		if(pWKB==NULL||iWKBLen<=0)
		{
			return NULL;
		}

		WKBGeometry *pWKBGeometry = NULL;
		SHPObject   *pshpObject = NULL;

		pWKBGeometry = (WKBGeometry*)(pWKB);

		switch(WKBTYPE(pWKBGeometry))
		{
		case wkbPoint:
			pshpObject = CreateSHPPointFromWKB((WKBPoint*)pWKB);
			break;
		case wkbLineString:
			pshpObject = CreateSHPLineStringFromWKB((WKBLineString*)pWKB);
			break;
		case wkbPolygon:
			pshpObject = CreateSHPPolygonFromWKB((WKBPolygon*)pWKB);
			break;
		case wkbMultiPoint:
			pshpObject = CreateSHPMultiPointFromWKB(pWKB, iWKBLen);
			break;
		case wkbMultiLineString:
			pshpObject = CreateSHPLineStringFromWKB((WKBMultiLineString*)pWKB);
		//	pshpObject = CreateSHPMultiLineStringFromWKB(pWKB, iWKBLen);
			break;
		case wkbMultiPolygon:
			pshpObject = CreateSHPMultiPolygonFromWKB(pWKB, iWKBLen);
			break;
		case wkbGeometryCollection:
			pshpObject = CreateSHPGeometryCollectionFromWKB(pWKB, iWKBLen);
			break;
		default:
			break;
		}


		return pshpObject;
	}

	SHPObject* FeatureInsertCommandShp::CreateSHPGeometryCollectionFromWKB(const g_uchar* pWKB, const int iWKBLen)
	{
		if(pWKB==NULL||iWKBLen<=0)
		{
			return NULL;
		}

		//WKBGeometry *pWKBGeometry = NULL;
		SHPObject   *pshpObject = NULL;

		pshpObject = new SHPObject;
		if( NULL == pshpObject)
		{
			return NULL;
		}

		return pshpObject;
	}


	SHPObject* FeatureInsertCommandShp::CreateSHPPointFromWKB(WKBPoint* pPoint)
	{
		if(pPoint==NULL)
		{
			return NULL;
		}

		SHPObject *pshpObject = NULL;

		//int * panPartStart = NULL;
		//int * panPartType  = NULL;
		//int nVertices = 0;
		double padfX = 0;
		double padfY = 0;
		double padfZ = 0;
		double padfM = 0;

		padfX = pPoint->point.x;
		padfY = pPoint->point.y;
		padfZ = 0;
		padfM = 0;

		pshpObject = ::SHPCreateObject(SHPT_POINT,
									 -1,
									 1,
									 NULL,
									 NULL,
									 1,
									 &padfX,
									 &padfY,
									 &padfY,
									 &padfZ);

		return pshpObject;
	}

	SHPObject* FeatureInsertCommandShp::CreateSHPLineStringFromWKB(WKBLineString* pWKBLineString)
	{
		if(pWKBLineString==NULL)
		{
			return NULL;
		}

		SHPObject *pshpObject = NULL;

		int i=0;
		int panPartStart = 0;
		//int panPartType  = 0;
		int nVertices = 0;
		double *padfX = NULL;
		double *padfY = NULL;
		double *padfZ = NULL;
		double *padfM = NULL;
		Point  *pt = NULL;	

		nVertices = pWKBLineString->numPoints;

		padfX = new double[nVertices];
		padfY = new double[nVertices];
		padfZ = new double[nVertices];
		padfM = new double[nVertices];

		if((padfX==NULL)||(padfY==NULL)||(padfZ==NULL)||(padfM==NULL))
		{
			if(padfX!=NULL)
			{
				delete[] padfX;
				padfX = NULL;
			}
			if(padfY!=NULL)
			{
				delete[] padfY;
				padfY = NULL;
			}
			if(padfZ!=NULL)
			{
				delete[] padfZ;
				padfZ = NULL;
			}
			if(padfM!=NULL)
			{
				delete[] padfM;
				padfM = NULL;
			}
		}

		pt = (Point*)(&(pWKBLineString->points[0]));
		for(i=0; i<nVertices; ++i, pt++)
		{
			padfX[i] = pt->x;
			padfY[i] = pt->y;
			padfZ[i] = 0;
			padfM[i] = 0;
		}

		pshpObject = SHPCreateObject(SHPT_ARC,
									 -1,
									 1,
									 &panPartStart,
									 NULL,
									 nVertices,
									 padfX,
									 padfY,
									 padfY,
									 padfZ);

		delete[] padfX;		padfX = NULL;
		delete[] padfY;		padfY = NULL;
		delete[] padfZ;		padfZ = NULL;
		delete[] padfM;		padfM = NULL;

		return pshpObject;
	}


	SHPObject* FeatureInsertCommandShp::CreateSHPLineStringFromWKB(WKBMultiLineString* pWKBMultiLineString)
	{
		if(pWKBMultiLineString==NULL)
		{
			return NULL;
		}

		SHPObject *pshpObject = NULL;

		int i = 0;
		int j = 0;
		int dbf_index = 0;
		int offset = 0;
		//int panPartType  = 0;
		int nVertices = 0;
		int nParts    = 0;
		int nPtCount  = 0;
		double	*padfX = NULL;
		double	*padfY = NULL;
		double	*padfZ = NULL;
		double	*padfM = NULL;
		Point	*pPoints = NULL;
		int		*pPanPartType  = NULL;
		int		*pPanPartStart = NULL;
		WKBLineString **ppPart = NULL;
		char	*pt = NULL;

		nParts = pWKBMultiLineString->numLineStrings;

		ppPart = new WKBLineString *[nParts];
		pPanPartType  = new int[nParts];
		pPanPartStart = new int[nParts];

		memset(ppPart, 0, nParts*sizeof(int));

		nVertices = 0; offset = 0;
		pt = (char*)(pWKBMultiLineString->lineStrings);
		for(i=0; i<nParts; ++i)
		{
			pt += offset;
			ppPart[i] = (WKBLineString*)(pt);
			offset = SizeOfWKBLineString(ppPart[i]);

			pPanPartStart[i] = nVertices;
			pPanPartType[i] = SHPT_ARC;
			nVertices += ppPart[i]->numPoints;
		}

		padfX = new double[nVertices];
		padfY = new double[nVertices];
		padfZ = new double[nVertices];
		padfM = new double[nVertices];

		/*
		if((padfX==NULL)||(padfY==NULL)||(padfZ==NULL)||(padfM==NULL))
		{
			if(padfX!=NULL)
			{
				delete[] padfX;
				padfX = NULL;
			}
			if(padfY!=NULL)
			{
				delete[] padfY;
				padfY = NULL;
			}
			if(padfZ!=NULL)
			{
				delete[] padfZ;
				padfZ = NULL;
			}
			if(padfM!=NULL)
			{
				delete[] padfM;
				padfM = NULL;
			}
		}
		*/

		for(i=0, dbf_index=0; i<nParts; ++i)
		{
			pPoints = (Point*)(&(ppPart[i]->points));
			nPtCount = ppPart[i]->numPoints;
			for(j=0; j<nPtCount; j++)
			{
				padfX[dbf_index] = pPoints[j].x;
				padfY[dbf_index] = pPoints[j].y;
				padfZ[dbf_index] = 0;
				padfM[dbf_index] = 0;

				++dbf_index;
			}
		}

		/*
		pPoints = (Point*)(&(pWKBLineString->points));
		for(i=0; i<nVertices; ++i)
		{
			padfX[i] = pPoints[i].x;
			padfY[i] = pPoints[i].y;
			padfZ[i] = 0;
			padfM[i] = 0;
		}
		*/

		pshpObject = SHPCreateObject(SHPT_ARC,
									-1,
									nParts,
									pPanPartStart,
									pPanPartType,
									nVertices,
									padfX,
									padfY,
									padfY,
									padfZ);

		delete[] padfX;			padfX = NULL;
		delete[] padfY;			padfY = NULL;
		delete[] padfZ;			padfZ = NULL;
		delete[] padfM;			padfM = NULL;
		delete[] ppPart;		ppPart = NULL;
		delete[] pPanPartType;	pPanPartType = NULL;

		return pshpObject;
	}

	SHPObject* FeatureInsertCommandShp::CreateSHPPolygonFromWKB(WKBPolygon* pWKBPolygon)
	{
		if(pWKBPolygon==NULL)
		{
			return NULL;
		}

		SHPObject *pshpObject = NULL;

		int i = 0;
		int j = 0;
		int dbf_index = 0;
		int offset = 0;
		//int panPartType  = NULL;
		int nVertices = 0;
		int nParts    = 0;
		int nPtCount  = 0;
		double	*padfX = NULL;
		double	*padfY = NULL;
		double	*padfZ = NULL;
		double	*padfM = NULL;
		Point	*pPoints = NULL;
		int		*pPanPartType  = NULL;
		int		*pPanPartStart = NULL;
		LinearRing **ppPart = NULL;
		char	*pt = NULL;

		nParts = pWKBPolygon->numRings;

		ppPart = new LinearRing *[nParts];
		pPanPartType  = new int[nParts];
		pPanPartStart = new int[nParts];

		memset(ppPart, 0, nParts*sizeof(int));

		nVertices = 0; offset = 0;
		pt = (char*)(pWKBPolygon->rings);
		for(i=0; i<nParts; ++i)
		{
			pt += offset;
			ppPart[i] = (LinearRing*)(pt);
			offset = SizeOfLinearRing(ppPart[i]);

			pPanPartStart[i] = nVertices;
			pPanPartType[i] = SHPT_POLYGON;
			nVertices += ppPart[i]->numPoints;
		}

		padfX = new double[nVertices];
		padfY = new double[nVertices];
		padfZ = new double[nVertices];
		padfM = new double[nVertices];

		for(i=0, dbf_index=0; i<nParts; ++i)
		{
			pPoints = (Point*)(&(ppPart[i]->points));
			nPtCount = ppPart[i]->numPoints;
			for(j=0; j<nPtCount; j++)
			{
				padfX[dbf_index] = pPoints[j].x;
				padfY[dbf_index] = pPoints[j].y;
				padfZ[dbf_index] = 0;
				padfM[dbf_index] = 0;

				++dbf_index;
			}
		}


		pshpObject = SHPCreateObject(SHPT_POLYGON,
									 -1,
									 nParts,
									 pPanPartStart,
									 pPanPartType,
									 nVertices,
									 padfX,
									 padfY,
									 padfY,
									 padfZ);

		delete[] padfX;			padfX = NULL;
		delete[] padfY;			padfY = NULL;
		delete[] padfZ;			padfZ = NULL;
		delete[] padfM;			padfM = NULL;
		delete[] ppPart;		ppPart = NULL;
		delete[] pPanPartType;	pPanPartType = NULL;

		return pshpObject;
	}

	SHPObject* FeatureInsertCommandShp::CreateSHPMultiPointFromWKB(const g_uchar* pWKB, const int iWKBLen)
	{
		if(pWKB==NULL||iWKBLen<=0)
		{
			return NULL;
		}

		SHPObject   *pshpObject = NULL;
		WKBMultiPoint	*pWKBMultiPoint = (WKBMultiPoint*)pWKB;

		int nVertices = 0;
		int nParts    = 0;

		double		*padfX = NULL;
		double		*padfY = NULL;
		double		*padfZ = NULL;
		double		*padfM = NULL;
		WKBPoint	*pPoints = NULL;
		int			*pPanPartType  = NULL;
		int			*pPanPartStart = NULL;

		nParts	  = pWKBMultiPoint->numPoints;
		nVertices = pWKBMultiPoint->numPoints;

		pPanPartType  = new int[nParts];
		pPanPartStart = new int[nParts];

		padfX = new double[nVertices];
		padfY = new double[nVertices];
		padfZ = new double[nVertices];
		padfM = new double[nVertices];

		pPoints = (WKBPoint*)(&(pWKBMultiPoint->points));
		for(int i=0; i<pWKBMultiPoint->numPoints; ++i)
		{
			pPanPartStart[i]	= i;
			pPanPartType[i]		= SHPT_POINT;
			padfX[i]			= pPoints[i].point.x;
			padfY[i]			= pPoints[i].point.y;
			padfX[i]			= 0;
			padfY[i]			= 0;
		}

		pshpObject = SHPCreateObject(	SHPT_MULTIPOINT,
										-1,
										nParts,
										pPanPartStart,
										pPanPartType,
										nVertices,
										padfX,
										padfY,
										padfY,
										padfZ);


		delete[] padfX;			padfX = NULL;
		delete[] padfY;			padfY = NULL;
		delete[] padfZ;			padfZ = NULL;
		delete[] padfM;			padfM = NULL;
		delete[] pPanPartType;	pPanPartType = NULL;
		delete[] pPanPartStart;	pPanPartStart = NULL;

		return pshpObject;
	}

	SHPObject* FeatureInsertCommandShp::CreateSHPMultiLineStringFromWKB(const g_uchar* pWKB, const int iWKBLen)
	{
		if(pWKB==NULL||iWKBLen<=0)
		{
			return NULL;
		}

		SHPObject		*pshpObject = NULL;
		int j;
		unsigned int	i;
		unsigned int	iOffset = 0;
		unsigned int	numLineStrings = 0;
		unsigned char	*ptr			  = NULL;
		WKBLineString	*pWKBLineString	  = NULL;
		WKBMultiLineString *pWKBMultiLineString = (WKBMultiLineString *)(pWKB);

		int nVertices = 0;
		int nParts    = 0;
		int nPtCount  = 0;
		double	*padfX = NULL;
		double	*padfY = NULL;
		double	*padfZ = NULL;
		double	*padfM = NULL;
		Point	*pPoints = NULL;
		int		*pPanPartType  = NULL;
		int		*pPanPartStart = NULL;
		unsigned int nIndexPart = 0;
		unsigned int nIndexVertices = 0;

		GetMultiLineStringMeta(pWKBMultiLineString, &nParts, &nVertices);

		pPanPartType  = new int[nParts];
		pPanPartStart = new int[nParts];

		padfX = new double[nVertices];
		padfY = new double[nVertices];
		padfZ = new double[nVertices];
		padfM = new double[nVertices];

		iOffset = 0;
		nPtCount = 0;
		nIndexPart = 0;
		nIndexVertices = 0;
		numLineStrings = pWKBMultiLineString->numLineStrings;
		ptr = (unsigned char*)(pWKBMultiLineString->lineStrings);

		for(i=0; i<numLineStrings; i++)
		{
			pWKBLineString = (WKBLineString*)(ptr);		

			pPanPartStart[i] = nPtCount;
			pPanPartType[i] = SHPT_ARC;

			pPoints = (Point*)(&(pWKBLineString->points));
			for(j=0; j<pWKBLineString->numPoints; ++j)
			{
				padfX[nIndexVertices] = pPoints[j].x;
				padfY[nIndexVertices] = pPoints[j].y;
				padfZ[nIndexVertices] = 0;
				padfM[nIndexVertices] = 0;

				++nIndexVertices;
			}
			nPtCount += pWKBLineString->numPoints;
			ptr += SizeOfWKBLineString(pWKBLineString);	
		}

		pshpObject = SHPCreateObject(	SHPT_ARC,
										-1,
										nParts,
										pPanPartStart,
										pPanPartType,
										nVertices,
										padfX,
										padfY,
										padfY,
										padfZ);

		delete[] padfX;			padfX = NULL;
		delete[] padfY;			padfY = NULL;
		delete[] padfZ;			padfZ = NULL;
		delete[] padfM;			padfM = NULL;
		delete[] pPanPartType;	pPanPartType  = NULL;
		delete[] pPanPartStart;	pPanPartStart = NULL;

		return pshpObject;
	}

	SHPObject* FeatureInsertCommandShp::CreateSHPMultiPolygonFromWKB(const g_uchar* pWKB, const int iWKBLen)
	{
		if(pWKB==NULL||iWKBLen<=0)
		{
			return NULL;
		}
	
		SHPObject   *pshpObject = NULL;
		unsigned int i, j, k;
		unsigned int iOffset = 0;
		unsigned int numPoints   = 0;
		unsigned int numRings	 = 0;
		unsigned int numPolygons = 0;
		unsigned char	*ptr			  = NULL;
		LinearRing	*pLinearRing	  = NULL;
		WKBPolygon		*pWKBPolygon	  = NULL;
		WKBMultiPolygon *pWKBMultiPolygon = (WKBMultiPolygon *)(pWKB);

		int nVertices = 0;
		int nParts    = 0;
		int nPtCount  = 0;
		int numRing	  = 0;
		double	*padfX = NULL;
		double	*padfY = NULL;
		double	*padfZ = NULL;
		double	*padfM = NULL;
		Point	*pt = NULL;
		int		*pPanPartType  = NULL;
		int		*pPanPartStart = NULL;
		unsigned int nIndexPart = 0;
		unsigned int nIndexVertices = 0;

		GetMultiPolygonMeta(pWKBMultiPolygon, &nParts, &nVertices);

		pPanPartType  = new int[nParts];
		pPanPartStart = new int[nParts];

		padfX = new double[nVertices];
		padfY = new double[nVertices];
		padfZ = new double[nVertices];
		padfM = new double[nVertices];

		iOffset = 0;
		nPtCount = 0;
		nIndexPart = 0;
		nIndexVertices = 0;
		numPolygons = pWKBMultiPolygon->numPolygons;
		ptr = (unsigned char*)(pWKBMultiPolygon->polygons);
	
		pWKBPolygon = (WKBPolygon*)(ptr);	

		for(i=0, numRing; i<numPolygons; i++)
		{
			numRings = pWKBPolygon->numRings;
			pLinearRing = (LinearRing*)(&(pWKBPolygon->rings[0]));
			for(j=0; j<numRings; ++j)
			{
				pPanPartStart[numRing] = nPtCount;
				pPanPartType[numRing++] = SHPT_POLYGON;

				numPoints = pLinearRing->numPoints;
				pt = (Point*)(&(pLinearRing->points[0]));

				for(k=0; k<numPoints; ++k, pt++)
				{
					padfX[nIndexVertices] = pt->x;
					padfY[nIndexVertices] = pt->y;
					padfZ[nIndexVertices] = 0;
					padfM[nIndexVertices] = 0;

					++nIndexVertices;
				}

				nPtCount += numPoints;
			
				pLinearRing = (LinearRing*)(pt);
			}

			pWKBPolygon = (WKBPolygon*)(pt);	
		}

		pshpObject = SHPCreateObject(SHPT_POLYGON,
									-1,
									nParts,
									pPanPartStart,
									pPanPartType,
									nVertices,
									padfX,
									padfY,
									padfY,
									padfZ);

		delete[] padfX;			padfX = NULL;
		delete[] padfY;			padfY = NULL;
		delete[] padfZ;			padfZ = NULL;
		delete[] padfM;			padfM = NULL;
		delete[] pPanPartType;	pPanPartType = NULL;
		delete[] pPanPartStart;	pPanPartStart = NULL;

		return pshpObject;
	}

	void FeatureInsertCommandShp::GetMultiLineStringMeta(WKBMultiLineString* pWKBMultiLineString, int* pnParts, int* pnVertices)
	{
		if(pWKBMultiLineString==NULL)
			return ;

		unsigned int	numLineStrings = 0;
		unsigned char	*ptr = NULL;
		WKBLineString	*pWKBLineString	  = NULL;

		(*pnParts) = 0;
		(*pnVertices) = 0;
		numLineStrings = pWKBMultiLineString->numLineStrings;
		ptr = (unsigned char*)(pWKBMultiLineString->lineStrings);
		for(unsigned int i=0; i<numLineStrings; i++)
		{
			pWKBLineString = (WKBLineString*)(ptr);		

			(*pnVertices) += pWKBLineString->numPoints;
			++(*pnParts);

			ptr += SizeOfWKBLineString(pWKBLineString);
		}
	}

	void FeatureInsertCommandShp::GetMultiPolygonMeta(WKBMultiPolygon* pWKBMultiPolygon, int* pnParts, int* pnVertices)
	{
		if(pWKBMultiPolygon==NULL)
			return ;

		unsigned int i, j;
		unsigned int iOffset	 = 0;
		unsigned int numRings	 = 0;
		unsigned int numPolygons = 0;
		unsigned char	*ptr = NULL;
		LinearRing	*pLinearRing	  = NULL;
		WKBPolygon		*pWKBPolygon	  = NULL;

		iOffset = 0;
		(*pnParts) = 0;
		(*pnVertices) = 0;
		numPolygons = pWKBMultiPolygon->numPolygons;
		ptr = (unsigned char*)(pWKBMultiPolygon->polygons);
		for(i=0; i<numPolygons; i++)
		{
			pWKBPolygon = (WKBPolygon*)(ptr);		

			numRings = pWKBPolygon->numRings;
			ptr = (unsigned char*)(&(pWKBPolygon->rings));
			for(j=0; j<numRings; ++j)
			{
				pLinearRing = (LinearRing*)(ptr);
				(*pnVertices) += pLinearRing->numPoints ;
				++(*pnParts);

				ptr += SizeOfLinearRing(pLinearRing);
			}
		}
	}

	void FeatureInsertCommandShp::WriteFieldValue(int shp_id, int dbf_index, augeFieldType type, GValue* pValue, const char* szEncoding/*=NULL*/)
	{
		DBFFieldType iDBFieldTye = GetDbfType(type);
		DBFHandle pdbfHandle = m_pFeatureClass->m_pdbfHandle;
		int ret_val;
		switch(iDBFieldTye)
		{
		case FTString:
			{
				const char* val = pValue->GetString();
				ret_val = ::DBFWriteStringAttribute(pdbfHandle, shp_id, dbf_index, val);
			}
			break;
		case FTInteger:
			{
				int val; 
				val = pValue->GetInt();
				ret_val = ::DBFWriteIntegerAttribute(pdbfHandle, shp_id, dbf_index, val);
			}
			break;
		case FTLong:
			{
				long val; 
				val = pValue->GetLong();
				ret_val = ::DBFWriteLongAttribute(pdbfHandle, shp_id, dbf_index, val);
			}
			break;
		case FTDouble:
			{
				double val; 
				val = pValue->GetDouble();
				ret_val = ::DBFWriteDoubleAttribute(pdbfHandle, shp_id, dbf_index, val);
			}
			break;
		case FTLogical:
			{
				char val = pValue->GetBool() ? 'Y' : 'N';
				ret_val = ::DBFWriteLogicalAttribute(pdbfHandle, shp_id, dbf_index, val);
			}
			break;
		case FTInvalid:
			{

			}
			break;
		default:
			break;
		}
	}

	DBFFieldType FeatureInsertCommandShp::GetDbfType(augeFieldType type)
	{
		DBFFieldType dbf_type = FTInteger;
		switch(type)
		{
		case augeFieldTypeString:
			dbf_type = FTString;
			break;
		case augeFieldTypeInt:
			dbf_type = FTInteger;
			break;
		case augeFieldTypeLong:
			dbf_type = FTLong;
			break;
		case augeFieldTypeDouble:
			dbf_type = FTDouble;
			break;
		case augeFieldTypeBool:
			dbf_type = FTLogical;
			break;
		case augeFieldTypeTime:// add by tjz 201001121140
			dbf_type = FTDate;
			break;
		case augeFieldTypeNone:
			dbf_type = FTInvalid;
			break;
		case augeFieldTypeChar:
		case augeFieldTypeShort:
		case augeFieldTypeInt64:
		case augeFieldTypeFloat:
			//	case augeFieldTypeTime:
		case augeFieldTypeBLOB:
		case augeFieldTypeGeometry:
			break;
		}

		return dbf_type;
	}

	int	FeatureInsertCommandShp::GetFieldInnerIndex(int index,int fid, int gindex) const 
	{
		int min_index = fid;
		int max_index = gindex;
		int inner_index = index;

		if(min_index > max_index)
			Swap(min_index, max_index);

		if(index<min_index)
			inner_index = index;
		else if(index>min_index&&index<max_index)
			inner_index = index-1;
		else
			inner_index = index-2;

		return inner_index;
	}

	inline
	void FeatureInsertCommandShp::Swap(int& val1, int& val2) const
	{
		int temp = val1;
		val1 = val2;
		val2 = temp;
	}

}

		