#include "ShpUtil.h"
#include "shapefil.h"
#include "string.h"

#define	SizeOfLinearRing(ptr)	((ptr)->numPoints - 1) * sizeof(Point) + sizeof(LinearRing)

namespace auge
{

ShpUtil::ShpUtil()
{

}

ShpUtil::~ShpUtil()
{

}

//////////////////////////////////////////////////////////////////////////
//
//augeFieldType ShpUtil::type_shp_2_gbsde(int iType)
//{
//	augeFieldType irsaType = augeFieldTypeInt;
//
//	switch(iType)
//	{
//	case FTString:
//		irsaType = augeFieldTypeString;
//		break;
//	case FTInteger:
//		irsaType = augeFieldTypeInt;
//		break;
//	case FTLong:
//		irsaType = augeFieldTypeLong;
//		break;
//	case FTDouble:
//		irsaType = augeFieldTypeDouble;
//		break;
//	case FTLogical:
//		irsaType = augeFieldTypeBool;
//		break;
//	case FTDate://add by tjz 201001121234
//		irsaType = augeFieldTypeTime;
//		break;
//	case FTInvalid:
//		irsaType = augeFieldTypeNone;
//		break;
//	}
//
//	return irsaType;
//}
//
//augeGeometryType ShpUtil::type_geom_shp_2_gbsde(int iType)
//{
//	augeGeometryType irsaType = augeGTPoint;
//
//	switch(iType)
//	{
//	case 1:
//		irsaType = augeGTPoint;
//		break;
//	case 3:
//		//irsaType = augeGTLineString;
//		irsaType = augeGTMultiLineString;
//		break;		
//	case 5:
//		//irsaType = augeGTPolygon;
//		irsaType = augeGTMultiPolygon;
//		break;
//	case 8:
//		irsaType = augeGTMultiPoint;
//		break;
//	default:
//		irsaType = augeGTPoint;
//		break;
//	}
//
//	return irsaType;
//}
//
//DBFFieldType ShpUtil::type_gbsde_2_shp(augeFieldType irsaTye)
//{
//	DBFFieldType iType = FTInteger;
//	switch(irsaTye)
//	{
//	case augeFieldTypeString:
//		iType = FTString;
//		break;
//	case augeFieldTypeInt:
//		iType = FTInteger;
//		break;
//	case augeFieldTypeLong:
//		iType = FTLong;
//		break;
//	case augeFieldTypeDouble:
//		iType = FTDouble;
//		break;
//	case augeFieldTypeBool:
//		iType = FTLogical;
//		break;
//	case augeFieldTypeTime:// add by tjz 201001121140
//		iType = FTDate;
//		break;
//	case augeFieldTypeNone:
//		iType = FTInvalid;
//		break;
//	case augeFieldTypeChar:
//	case augeFieldTypeShort:
//	case augeFieldTypeInt64:
//	case augeFieldTypeFloat:
////	case augeFieldTypeTime:
//	case augeFieldTypeBLOB:
//	case augeFieldTypeGeometry:
//		break;
//	}
//	
//	return iType;
//}

int	ShpUtil::type_geom_gbsde_2_shp(augeGeometryType irsaType)
{
	int iType = 1;
	
	switch(irsaType)
	{
	case augeGTPoint:
		iType = 1;
		break;
	case augeGTMultiPoint:
		iType = 8;
		break;
	case augeGTLineString:
	case augeGTMultiLineString:
		iType = 3;
		break;		
	case augeGTPolygon:
	case augeGTMultiPolygon:
		iType = 5;
		break;
	default:
		iType = 1;
		break;
	}

	return iType;
}

//////////////////////////////////////////////////////////////////////////
int	ShpUtil::GetWKBLength(SHPObject* pSHPObject)
{
	int type = pSHPObject->nSHPType;
	switch(type)
	{
	case SHPT_POINT:
		return GetWKBLenofPoint(pSHPObject);
		break;
	case SHPT_ARC:
	case SHPT_ARCZ:
		{
			if(pSHPObject->nParts>1)
			//if(pSHPObject->nParts>0)
			{
				return GetWKBLenofMultiLine(pSHPObject);
			}
			else
			{
				return GetWKBLenofLine(pSHPObject);
			}
		}
		break;
	case SHPT_POLYGON:
	case SHPT_POLYGONZ:
		{	
			//return GetWKBLenofMultiPolygon(pSHPObject);
			if(IsSimplePolygon(pSHPObject))
			{
				return GetWKBLenofPolygon(pSHPObject);
			}	
			else
			{
				return GetWKBLenofMultiPolygon(pSHPObject);
			}
		}
		break;
	case SHPT_MULTIPOINT:
		{
			return GetWKBLenofMPoint(pSHPObject);
		}
		break;
	default:
		{
			return 0;
		}
		break;
	}
	
	return 0;
}


int ShpUtil::GetWKBLenofPoint(SHPObject *pSHPObject)
{
	int len = 5;
    len += 2*sizeof(double);
	return len;
}

int ShpUtil::GetWKBLenofLine(SHPObject *pSHPObject)
{
	int numofPoints = pSHPObject->nVertices;
	int len = 9;
    len += 2*numofPoints*sizeof(double);
	return len;
}

int ShpUtil::GetWKBLenofMultiLine(SHPObject *pSHPObject)
{
	//int numofPoints = pSHPObject->nVertices;
	//int len = 9;
	//len += 2*numofPoints*sizeof(double);
	//return len;
	
	int i;
	int iWKBLen = 0;
	
	iWKBLen += 9;
	
	for(i=0; i<pSHPObject->nParts; ++i)
	{
		iWKBLen += 9;
	}
	
	iWKBLen += 2*(pSHPObject->nVertices) * sizeof(double);
	
	return iWKBLen;
}

int ShpUtil::GetWKBLenofMultiPolygon(SHPObject *pSHPObject)
{
	//int numofPoints = pSHPObject->nVertices;
	//int len = 9;
	//len += 2*numofPoints*sizeof(double);
	//return len;
	
	int i;
	int iWKBLen = 0;
	
	iWKBLen += 9;
	
	for(i=0; i<pSHPObject->nParts; ++i)
	{
		iWKBLen += 9;
		iWKBLen += 4;
	}
	
	iWKBLen += 2*(pSHPObject->nVertices) * sizeof(double);
	
	return iWKBLen;
}


int ShpUtil::GetWKBLenofPolygon(SHPObject *pSHPObject)
{
	int numofLine = pSHPObject->nParts;
	int len = 9;
	len += 4*numofLine;
    int numofPoints = pSHPObject->nVertices;
	len += 2*numofPoints*sizeof(double);
	return len;
}

int ShpUtil::GetWKBLenofMPoint(SHPObject *pSHPObject)
{
    int len = 9;
	int num = pSHPObject->nVertices;
	len += num*(5+2*sizeof(double));
	return len;
}

//////////////////////////////////////////////////////////////////////////

int ShpUtil::SHPObject_2_WKB(SHPObject *pSHPObject, g_uchar* pWKB, int len)
{
    int type = pSHPObject->nSHPType;
	switch(type)
	{
	case SHPT_POINT:
		//return WritePoint(pSHPObject,pWKB,len);
		return WritePoint(pSHPObject, (WKBPoint*)pWKB);
		break;
	case SHPT_ARC:
	case SHPT_ARCZ:
		{
			//if(true)
			if(pSHPObject->nParts>1)	//只有Part大于1的线，才转为MultiLineString
			//if(pSHPObject->nParts>0)	//所有的Line都转换为MultiLineString
			{
				//int ret = WriteMultiLine(pSHPObject,pWKB,len);
				int ret = WriteMultiLine(pSHPObject,(WKBMultiLineString*)pWKB);
 				return ret;
			}
			else
			{
				//return WriteLine(pSHPObject,pWKB,len);
				return WriteLine(pSHPObject, (WKBLineString*)pWKB);
			}
		}
		break;
	case SHPT_POLYGON:
	case SHPT_POLYGONZ:
		{
			//return WritePolygon(pSHPObject,pWKB,len);
			//return WritePolygon(pSHPObject,(WKBPolygon*)pWKB);
			//return WriteMultiPolygon(pSHPObject,pWKB,len);
			//return WriteMultiPolygon(pSHPObject, (WKBMultiPolygon*)pWKB);
			if(IsSimplePolygon(pSHPObject))
			{
				return WritePolygon(pSHPObject, (WKBPolygon*)pWKB);
			}
			else
			{
				return WriteMultiPolygon(pSHPObject, (WKBMultiPolygon*)pWKB);
			}
		}
		break;
	case SHPT_MULTIPOINT:
		{
			return WriteMultiPoint(pSHPObject,(WKBMultiPoint*)pWKB);
		}
		break;
	case SHPT_POINTZ:
		{
			
		}
		break;
	case SHPT_MULTIPOINTZ:
		{
			
		}
		break;
	case SHPT_POINTM:
		{
			
		}
		break;
	case SHPT_ARCM:
		{
			
		}
		break;
	case SHPT_POLYGONM:
		{
			
		}
		break;
	case SHPT_MULTIPOINTM:
		{
			
		}
		break;
	case SHPT_MULTIPATCH:
		{
			
		}
		break;
	default:
		{
		}
		break;
	}
	
	return 0;
}

g_uchar* ShpUtil::SHPObject_2_WKB(SHPObject *pSHPObject)
{
	if(pSHPObject==NULL)
		return NULL;

	int	  iWKBLen = 0;
	g_uchar* pWKB = NULL;
	Geometry*	pGeometry  = NULL;

	iWKBLen = GetWKBLength(pSHPObject);
	if(iWKBLen==0)
	{
		::SHPDestroyObject(pSHPObject);
		return NULL;
	}
	
	pWKB = new g_uchar[iWKBLen];
	if(pWKB==NULL)
	{
		::SHPDestroyObject(pSHPObject);
		return NULL;
	}
	memset(pWKB, 0, iWKBLen);
	if(SHPObject_2_WKB(pSHPObject, pWKB, iWKBLen)==0)
	{
		delete[] pWKB;
		::SHPDestroyObject(pSHPObject);
		return NULL;
	}
	return pWKB;
}


int ShpUtil::WritePoint(SHPObject *pSHPObject, char* pWKB, int len)
{
    double x = pSHPObject->padfX[0];
	double y = pSHPObject->padfY[0];

	len = 0;
	char byteorder = 1;
	memcpy(pWKB,&byteorder,1);
	len += 1;
	int wkbType = 1;
	memcpy(pWKB+len,&wkbType,4);
	len += 4;
	memcpy(pWKB+len,&x,sizeof(double));
	len += sizeof(double);
	memcpy(pWKB+len,&y,sizeof(double));
	len += sizeof(double);
	return 1;

}

int ShpUtil::WriteLine(SHPObject *pSHPObject, char* pWKB, int len)
{
	int numofPoints = pSHPObject->nVertices;
	len = 0;

	char byteorder = 1;
	memcpy(pWKB,&byteorder,1);
	len += 1;
	int wkbType = 2;
	memcpy(pWKB+len,&wkbType,4);
	len += 4;
	memcpy(pWKB+len,&numofPoints,sizeof(int));
	len += 4;

	for(int i=0;i<numofPoints;i++)
	{
		double x = pSHPObject->padfX[i];
		double y = pSHPObject->padfY[i];
		memcpy(pWKB+len,&x,sizeof(double));
	    len += sizeof(double);
		memcpy(pWKB+len,&y,sizeof(double));
		len += sizeof(double);
	}
	return 1;

}

int ShpUtil::WritePolygon(SHPObject *pSHPObject, char* pWKB, int len)
{
	int numofLine = pSHPObject->nParts;
	len = 0;

	char byteorder = 1;
	memcpy(pWKB,&byteorder,1);
	len += 1;
	int wkbType = wkbPolygon;
	memcpy(pWKB+len,&wkbType,4);
	len += 4;
	memcpy(pWKB+len,&numofLine,4);
	len += 4;

	for(int i=0;i<numofLine;i++)
	{
        int j = pSHPObject->panPartStart[i];
		if(i<(numofLine-1))
		{
         int k = pSHPObject->panPartStart[i+1];
		 int numofP = k-j;
		 memcpy(pWKB+len,&numofP,4);
		 len += 4;
		 for(int t=j;t<k;t++)
		 {
           double x = pSHPObject->padfX[t];
           memcpy(pWKB+len,&x,sizeof(double));
		   len += sizeof(double);
		   double y = pSHPObject->padfY[t];
		   memcpy(pWKB+len,&y,sizeof(double));
		   len += sizeof(double);
		 }
		}else
		{
         int numofPoints = pSHPObject->nVertices;
		 int numofP = numofPoints-j;
		 memcpy(pWKB+len,&numofP,4);
		 len += 4;
		 for(int t=j;t<numofPoints;t++)
		 {
			 double x = pSHPObject->padfX[t];
             memcpy(pWKB+len,&x,sizeof(double));
			 len += sizeof(double);
		     double y = pSHPObject->padfY[t];
		     memcpy(pWKB+len,&y,sizeof(double));
		     len += sizeof(double);
		 }
		}


	}	return 1;
}

int ShpUtil::WritePolygon(SHPObject *pSHPObject, WKBPolygon* pWKBPolygon)
{
	if(pSHPObject==NULL||pWKBPolygon==NULL)
		return 0;

	int numParts = 0;
	int numPoints = 0;
	int i=0, j=0, k=0, t=0;
	double *pX = NULL;
	double *pY = NULL;
	auge::Point	*pt = NULL;
	LinearRing	*pLinearRing = NULL;

	numParts = pSHPObject->nParts;

	pWKBPolygon->byteOrder = coDefaultByteOrder;
	pWKBPolygon->wkbType = wkbPolygon;
	pWKBPolygon->numRings = numParts;

	pX = pSHPObject->padfX;
	pY = pSHPObject->padfY;

	pLinearRing = &(pWKBPolygon->rings[0]);
	for(i=0; i<numParts; i++)
	{
		j = pSHPObject->panPartStart[i];
		if(i<(numParts-1))
		{
			k = pSHPObject->panPartStart[i+1];
			numPoints = k-j;

			// WKBLiearRing
			pLinearRing->numPoints = numPoints;
			pt = &(pLinearRing->points[0]);

			for(t=0; t<numPoints; t++, pt++)
			{
				pt->x = *(pX++);
				pt->y = *(pY++);
			}


// 			int k = pSHPObject->panPartStart[i+1];
// 			int numofP = k-j;
// 			memcpy(pWKB+len,&numofP,4);
// 			len += 4;
// 			for(int t=j;t<k;t++)
// 			{
// 				double x = pSHPObject->padfX[t];
// 				memcpy(pWKB+len,&x,sizeof(double));
// 				len += sizeof(double);
// 				double y = pSHPObject->padfY[t];
// 				memcpy(pWKB+len,&y,sizeof(double));
// 				len += sizeof(double);
// 			}
		}
		else
		{	
			//最后一个LiearRing
			numPoints = pSHPObject->nVertices - j;

			// WKBLiearRing
			pLinearRing->numPoints = numPoints;
			pt = &(pLinearRing->points[0]);

			for(t=0; t<numPoints; t++, pt++)
			{
				pt->x = *(pX++);
				pt->y = *(pY++);
			}

// 			int numofPoints = pSHPObject->nVertices;
// 			int numofP = numofPoints-j;
// 			memcpy(pWKB+len,&numofP,4);
// 			len += 4;
// 			for(int t=j;t<numofPoints;t++)
// 			{
// 				double x = pSHPObject->padfX[t];
// 				memcpy(pWKB+len,&x,sizeof(double));
// 				len += sizeof(double);
// 				double y = pSHPObject->padfY[t];
// 				memcpy(pWKB+len,&y,sizeof(double));
// 				len += sizeof(double);
// 			}
		}

		pLinearRing = (LinearRing*)pt;
	}

	return 1;
}

int ShpUtil::WriteMultiPoint(SHPObject *pSHPObject, WKBMultiPoint* pWKBMultiPoint)
{
	if(pSHPObject==NULL||pWKBMultiPoint==NULL)
		return 0;

	WKBPoint		*pWKBPoint = NULL;

	pWKBMultiPoint->byteOrder = coDefaultByteOrder;
	pWKBMultiPoint->wkbType = wkbMultiPoint;
	pWKBMultiPoint->numPoints = pSHPObject->nVertices;

	pWKBPoint = &(pWKBMultiPoint->points[0]);	
	pWKBPoint->byteOrder = coDefaultByteOrder;
	pWKBPoint->wkbType = wkbPoint;

	int numofPoints = pSHPObject->nVertices;
	for(int i=0;i<numofPoints;i++)
	{
		pWKBPoint->byteOrder = coDefaultByteOrder;
		pWKBPoint->wkbType = wkbPoint;
		pWKBPoint->point.x = pSHPObject->padfX[i];
		pWKBPoint->point.y = pSHPObject->padfY[i];
		
		pWKBPoint++;
	}

	return 1;
}

int ShpUtil::WritePoint(SHPObject *pSHPObject, WKBPoint* pWKBPoint)
{
	if(pSHPObject==NULL||pWKBPoint==NULL)
		return 0;

	pWKBPoint->byteOrder = coDefaultByteOrder;
	pWKBPoint->wkbType = wkbPoint;
	pWKBPoint->point.x = pSHPObject->padfX[0];
	pWKBPoint->point.y = pSHPObject->padfY[0];

	return 1;
}

int ShpUtil::WriteLine(SHPObject *pSHPObject, WKBLineString* pWKBLineString)
{
	if(pSHPObject==NULL||pWKBLineString==NULL)
		return 0;

	auge::Point *pt = NULL;
	double* pX = NULL;
	double* pY = NULL;
	pWKBLineString->byteOrder = coDefaultByteOrder;
	pWKBLineString->wkbType = wkbLineString;
	pWKBLineString->numPoints = pSHPObject->nVertices;

	pt = &(pWKBLineString->points[0]);
	int numPoints = pSHPObject->nVertices;;
	pX = pSHPObject->padfX;
	pY = pSHPObject->padfY;
	for(int i=0; i<numPoints; i++, pt++)
	{
		pt->x = *(pX++);
		pt->y = *(pY++);
	}

	return 1;
}

int ShpUtil::WriteMultiLine(SHPObject *pSHPObject, WKBMultiLineString* pWKBMultiLineString)
{
	if(pSHPObject==NULL||pWKBMultiLineString==NULL)
		return 0;

	int	  numLines = 0;
	int	  i, j;
	int   numPoints;
	int	  i32start = 0;
	int	  i32end = 0;
	auge::Point		*pt = NULL;
	WKBLineString	*pWKBLineString = NULL;

	pWKBMultiLineString->byteOrder = coDefaultByteOrder;
	pWKBMultiLineString->wkbType = wkbMultiLineString;
	pWKBMultiLineString->numLineStrings = pSHPObject->nParts;

	pWKBLineString = &(pWKBMultiLineString->lineStrings[0]);

	numLines = pSHPObject->nParts;
	for(i=0, i32start = 0; i<numLines; i++)
	{
		if((i+1)<pSHPObject->nParts)
			i32end = (pSHPObject->panPartStart)[i+1];
		else
			i32end = pSHPObject->nVertices;
		//subline的起止点位置
		numPoints = i32end - i32start;
		
		pWKBLineString->byteOrder = coDefaultByteOrder;
		pWKBLineString->wkbType = wkbLineString;
		pWKBLineString->numPoints = numPoints;
		pt = &(pWKBLineString->points[0]);
		
		for(j=i32start; j<i32end; ++j, pt++)
		{
			pt->x = pSHPObject->padfX[j];
			pt->y = pSHPObject->padfY[j];
		}
		
		i32start = i32end;
		pWKBLineString = (WKBLineString*)(pt);
	}

	return 1;
}

// int ShpUtil::WriteMultiPolygon(SHPObject *pSHPObject, char* pWKB, int len)
// {
// 	if(pSHPObject==NULL||pWKB==NULL)
// 		return 0;
// 	
// 	char*	  pt = pWKB;
// 	char	  byteorder = 1;
// 	int	  i, j;
// 	int   wkbType = 6;
// 	int   wkbSubType = 3;
// 	int   numPoints;
// 	int	  i32start = 0;
// 	int	  i32end = 0;
// 	int   numRing = 1;
// 	size_t	  sizeof_double = sizeof(double);
// 	
// 	memset(pt, 0, len);
// 	
// 	memcpy(pt, &byteorder, 1);
// 	pt += 1;
// 	memcpy(pt, &wkbType,4);
// 	pt += 4;
// 	memcpy(pt, &(pSHPObject->nParts), 4);
// 	pt += 4;
// 	
// 	for(i=0, i32start = 0; i<pSHPObject->nParts; ++i)
// 	{
// 		if((i+1)<pSHPObject->nParts)
// 			i32end = (pSHPObject->panPartStart)[i+1];
// 		else
// 			i32end = pSHPObject->nVertices;
// 		
// 		numPoints = i32end - i32start;
// 		
// 		memcpy(pt, &byteorder, 1);
// 		pt += 1;
// 		memcpy(pt, &wkbSubType,4);
// 		pt += 4;
// 		memcpy(pt, &numRing, 4);
// 		pt += 4;
// 		memcpy(pt, &numPoints, 4);
// 		pt += 4;
// 		
// 		for(j=i32start; j<i32end; ++j)
// 		{
// 			memcpy(pt, pSHPObject->padfX+j, sizeof_double);
// 			pt += sizeof_double;
// 			memcpy(pt, pSHPObject->padfY+j, sizeof_double);
// 			pt += sizeof_double;
// 		}
// 		
// 		i32start = i32end;
// 	}
// 	
// 	return 1;
// }

int ShpUtil::WriteMultiPolygon(SHPObject *pSHPObject, WKBMultiPolygon* pWKBMultiPolygon)
{
	if(pSHPObject==NULL||pWKBMultiPolygon==NULL)
		return 0;

	
	int	i=0, j=0;
	int numPoints = 0;
	int numPolygons = 0;
	int numRing = 1;
 	int	i32start = 0;
 	int	i32end = 0;
	WKBPolygon	*pWKBPolygon = NULL;
	LinearRing	*pLinearRing = NULL;
	auge::Point *pt = NULL;
	double *pX = NULL;
	double *pY = NULL;

	pWKBMultiPolygon->byteOrder = coDefaultByteOrder;
	pWKBMultiPolygon->wkbType = wkbMultiPolygon;
	pWKBMultiPolygon->numPolygons = pSHPObject->nParts;
	pWKBPolygon = &(pWKBMultiPolygon->polygons[0]);

	pX = pSHPObject->padfX;
	pY = pSHPObject->padfY;

	for(i=0, i32start = 0; i<pSHPObject->nParts; ++i)
	{
		if((i+1)<pSHPObject->nParts)
			i32end = (pSHPObject->panPartStart)[i+1];
		else
			i32end = pSHPObject->nVertices;
		
		pWKBPolygon->byteOrder = coDefaultByteOrder;
		pWKBPolygon->wkbType = wkbPolygon;
		pWKBPolygon->numRings = numRing;
		
		numPoints = i32end - i32start;
		pLinearRing = &(pWKBPolygon->rings[0]);
		pLinearRing->numPoints = numPoints;
		pt = &(pLinearRing->points[0]);

		for(j=0; j<numPoints; j++, pt++)
		{
			pt->x = *(pX++);
			pt->y = *(pY++);				 
		}

		i32start = i32end;
		pWKBPolygon = (WKBPolygon*)(pt);

	}

	return 1;
}

int ShpUtil::WriteMultiLine(SHPObject *pSHPObject, char* pWKB, int len)
{
	if(pSHPObject==NULL||pWKB==NULL)
		return 0;

	char* pt = pWKB;
	char  byteorder = 1;
	int	  i, j;
	int   wkbType = 5;
	int   wkbSubType = 2;
	int   numPoints;
	int	  i32start = 0;
	int	  i32end = 0;
	size_t	  sizeof_double = sizeof(double);

	memset(pt, 0, len);

	memcpy(pt, &byteorder, 1);
	pt += 1;
	memcpy(pt, &wkbType,4);
	pt += 4;
	memcpy(pt, &(pSHPObject->nParts), 4);
	pt += 4;

	for(i=0, i32start = 0; i<pSHPObject->nParts; ++i)
	{
		if((i+1)<pSHPObject->nParts)
			i32end = (pSHPObject->panPartStart)[i+1];
		else
			i32end = pSHPObject->nVertices;

		numPoints = i32end - i32start;

		memcpy(pt, &byteorder, 1);
		pt += 1;
		memcpy(pt, &wkbSubType,4);
		pt += 4;
		memcpy(pt, &numPoints, 4);
		pt += 4;

		for(j=i32start; j<i32end; ++j)
		{
			memcpy(pt, pSHPObject->padfX+j, sizeof_double);
			pt += sizeof_double;
			memcpy(pt, pSHPObject->padfY+j, sizeof_double);
			pt += sizeof_double;
		}

		i32start = i32end;
	}

	return 1;
}

int ShpUtil::WriteMultiPolygon(SHPObject *pSHPObject, char* pWKB, int len)
{
	if(pSHPObject==NULL||pWKB==NULL)
		return 0;
	
	char*	  pt = pWKB;
	char	  byteorder = 1;
	int	  i, j;
	int   wkbType = 6;
	int   wkbSubType = 3;
	int   numPoints;
	int	  i32start = 0;
	int	  i32end = 0;
	int   numRing = 1;
	size_t	  sizeof_double = sizeof(double);
	
	memset(pt, 0, len);
	
	memcpy(pt, &byteorder, 1);
	pt += 1;
	memcpy(pt, &wkbType,4);
	pt += 4;
	memcpy(pt, &(pSHPObject->nParts), 4);
	pt += 4;
	
	for(i=0, i32start = 0; i<pSHPObject->nParts; ++i)
	{
		if((i+1)<pSHPObject->nParts)
			i32end = (pSHPObject->panPartStart)[i+1];
		else
			i32end = pSHPObject->nVertices;
		
		numPoints = i32end - i32start;
		
		memcpy(pt, &byteorder, 1);
		pt += 1;
		memcpy(pt, &wkbSubType,4);
		pt += 4;
		memcpy(pt, &numRing, 4);
		pt += 4;
		memcpy(pt, &numPoints, 4);
		pt += 4;
		
		for(j=i32start; j<i32end; ++j)
		{
			memcpy(pt, pSHPObject->padfX+j, sizeof_double);
			pt += sizeof_double;
			memcpy(pt, pSHPObject->padfY+j, sizeof_double);
			pt += sizeof_double;
		}
		
		i32start = i32end;
	}
	
	return 1;
}
//
//int ShpUtil::FindDefaultGeometryField(AgFeatureClassInfo* pFeatureClassInfo)
//{
//	if(pFeatureClassInfo==NULL)
//		return -1;
//
//	int index = -1;
//	int iGeometryCount = 0;
//	AgField*	 pField = NULL;
//	AgFields* pFields = (AgFields*)(pFeatureClassInfo->GetFields());
//	int iCount = pFields->Count();
//	for(int i=0; i<iCount; i++)
//	{
//		pField = pFields->GetField(i);
//		if(pField!=NULL)
//		{
//			if((augeFieldType)pField->GetType()==augeFieldTypeGeometry)
//			{
//				if(iGeometryCount==0)
//					index = i;
//				AgGeometryDef* pGeometryDef = pField->GetGeometryDef();
//				if(pGeometryDef->IsDefault())
//				{
//					index = i;
//					break;
//				}
//
//				++iGeometryCount;				
//			}
//		}
//	}
//
//	return index;
//}


SHPObject* ShpUtil::CreateSHPObjectFromWKB(const g_uchar* pWKB, const int iWKBLen)
{
	if(pWKB==NULL||iWKBLen<=0)
	{
		return NULL;
	}

	WKBGeometry *pWKBGeometry = NULL;
	SHPObject   *pSHPObject = NULL;

	pWKBGeometry = (WKBGeometry*)(pWKB);

	switch(WKBTYPE(pWKBGeometry))
	{
	case wkbPoint:
		pSHPObject = CreateSHPPointFromWKB((WKBPoint*)pWKB);
		break;
	case wkbLineString:
		pSHPObject = CreateSHPLineStringFromWKB((WKBLineString*)pWKB);
		break;
	case wkbPolygon:
		pSHPObject = CreateSHPPolygonFromWKB((WKBPolygon*)pWKB);
		break;
	case wkbMultiPoint:
		pSHPObject = CreateSHPMultiPointFromWKB(pWKB, iWKBLen);
		break;
	case wkbMultiLineString:
		pSHPObject = CreateSHPLineStringFromWKB((WKBMultiLineString*)pWKB);
	//	pSHPObject = CreateSHPMultiLineStringFromWKB(pWKB, iWKBLen);
		break;
	case wkbMultiPolygon:
		pSHPObject = CreateSHPMultiPolygonFromWKB(pWKB, iWKBLen);
		break;
	case wkbGeometryCollection:
		//pSHPObject = CreateSHPGeometryCollectionFromWKB(pWKB, iWKBLen);
		break;
	default:
		break;
	}


	return pSHPObject;
}

SHPObject* ShpUtil::CreateSHPPointFromWKB(WKBPoint* pPoint)
{
	if(pPoint==NULL)
	{
		return NULL;
	}

	SHPObject *pSHPObject = NULL;

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

	pSHPObject = ::SHPCreateObject(SHPT_POINT,
		                         -1,
								 1,
								 NULL,
								 NULL,
								 1,
								 &padfX,
								 &padfY,
								 &padfY,
								 &padfZ);

	return pSHPObject;
}

SHPObject* ShpUtil::CreateSHPLineStringFromWKB(WKBLineString* pWKBLineString)
{
	if(pWKBLineString==NULL)
	{
		return NULL;
	}

	SHPObject *pSHPObject = NULL;

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

	pSHPObject = SHPCreateObject(SHPT_ARC,
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

	return pSHPObject;
}


SHPObject* ShpUtil::CreateSHPLineStringFromWKB(WKBMultiLineString* pWKBMultiLineString)
{
	if(pWKBMultiLineString==NULL)
	{
		return NULL;
	}

	SHPObject *pSHPObject = NULL;

	int i = 0;
	int j = 0;
	int index = 0;
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

	for(i=0, index=0; i<nParts; ++i)
	{
		pPoints = (Point*)(&(ppPart[i]->points));
		nPtCount = ppPart[i]->numPoints;
		for(j=0; j<nPtCount; j++)
		{
			padfX[index] = pPoints[j].x;
			padfY[index] = pPoints[j].y;
			padfZ[index] = 0;
			padfM[index] = 0;

			++index;
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

	pSHPObject = SHPCreateObject(SHPT_ARC,
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

	return pSHPObject;
}

SHPObject* ShpUtil::CreateSHPPolygonFromWKB(WKBPolygon* pWKBPolygon)
{
	if(pWKBPolygon==NULL)
	{
		return NULL;
	}

	SHPObject *pSHPObject = NULL;

	int i = 0;
	int j = 0;
	int index = 0;
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

	for(i=0, index=0; i<nParts; ++i)
	{
		pPoints = (Point*)(&(ppPart[i]->points));
		nPtCount = ppPart[i]->numPoints;
		for(j=0; j<nPtCount; j++)
		{
			padfX[index] = pPoints[j].x;
			padfY[index] = pPoints[j].y;
			padfZ[index] = 0;
			padfM[index] = 0;

			++index;
		}
	}


	pSHPObject = SHPCreateObject(SHPT_POLYGON,
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

	return pSHPObject;
}

SHPObject* ShpUtil::CreateSHPMultiPointFromWKB(const g_uchar* pWKB, const int iWKBLen)
{
	if(pWKB==NULL||iWKBLen<=0)
	{
		return NULL;
	}

	SHPObject   *pSHPObject = NULL;
	WKBMultiPoint	*pWKBMultiPoint = (WKBMultiPoint*)pWKB;

	int nVertices = 0;
	int nParts    = 0;

	double		x,y;
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
		x = pPoints[i].point.x;
		y = pPoints[i].point.y;
		pPanPartStart[i]	= i;
		pPanPartType[i]		= SHPT_POINT;
		padfX[i]			= x;
		padfY[i]			= y;
		padfZ[i]			= 0;
		padfM[i]			= 0;
	}

	pSHPObject = SHPCreateObject(	SHPT_MULTIPOINT,
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

	return pSHPObject;
}

SHPObject* ShpUtil::CreateSHPMultiLineStringFromWKB(const g_uchar* pWKB, const int iWKBLen)
{
	if(pWKB==NULL||iWKBLen<=0)
	{
		return NULL;
	}

	SHPObject		*pSHPObject = NULL;
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

	pSHPObject = SHPCreateObject(	SHPT_ARC,
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

	return pSHPObject;
}

SHPObject* ShpUtil::CreateSHPMultiPolygonFromWKB(const g_uchar* pWKB, const int iWKBLen)
{
	if(pWKB==NULL||iWKBLen<=0)
	{
		return NULL;
	}
	
	SHPObject   *pSHPObject = NULL;
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

	pSHPObject = SHPCreateObject(SHPT_POLYGON,
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

	return pSHPObject;
}

SHPObject* ShpUtil::CreateSHPGeometryCollectionFromWKB(const char* pWKB, const int iWKBLen)
{
	if(pWKB==NULL||iWKBLen<=0)
	{
		return NULL;
	}

	//WKBGeometry *pWKBGeometry = NULL;
	SHPObject   *pSHPObject = NULL;

	pSHPObject = new SHPObject;
	if( NULL == pSHPObject)
	{
		return NULL;
	}

	return pSHPObject;
}


void ShpUtil::GetMultiPolygonMeta(WKBMultiPolygon* pWKBMultiPolygon, int* pnParts, int* pnVertices)
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


void ShpUtil::GetMultiLineStringMeta(WKBMultiLineString* pWKBMultiLineString, int* pnParts, int* pnVertices)
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

//////////////////////////////////////////////////////////////////////////
//AgField* ShpUtil::CreateField(const char* szName, const char* szAlias, augeFieldType type, int nLength/*=4*/, int nPrecision/*=0*/)
//{
//	if(szName==NULL)
//		return false;
//	
//	AgFieldImpl *pagField = new AgFieldImpl();
//
//	pagField->SetName(szName);
//	pagField->SetAlias(szAlias);
//	if(type==augeFieldTypeLong)
//		pagField->SetType(augeFieldTypeInt);
//	else
//		pagField->SetType(type);
//	pagField->SetLength(nLength);
//	pagField->SetPrecision(nPrecision);
//	
//	return pagField;
//}

//AgField* ShpUtil::CreateField(const char* szName, const char* szAlias, augeGeometryType iGeomType, AgEnvelope &extent, long lSrid/*=0*/, bool isDefault/*=false*/)
//{
//	if(szName==NULL)
//		return false;
//	
//	AgFieldImpl *pagField = new AgFieldImpl();
//
//	pagField->SetName(szName);
//	pagField->SetAlias(szAlias);
//	pagField->SetType(augeFieldTypeGeometry);
//	pagField->SetLength(4);
//	pagField->SetPrecision(0);
//	
//	AgGeometryDef* pGeometryDef = pagField->GetGeometryDef();
//	if(pGeometryDef!=NULL)
//	{
//		AgGeometryDef_2* pagGeomeryDef_2 = pGeometryDef->GeometryDef_2();
//		pagGeomeryDef_2->SetGeometryType(iGeomType);
//		pagGeomeryDef_2->SetZ(false);
//		pagGeomeryDef_2->SetM(false);
//		pagGeomeryDef_2->SetSRID(lSrid);
//		pagGeomeryDef_2->SetDefault(isDefault);
//		pagGeomeryDef_2->SetExtent(extent);
//	}
//
//	return pagField;
//}

/**
 * 判断多边形是Polygon还是MultiPolygon
 * 从第二个环开始，判断是否有顺时针方向的环，如果存在则说明该Polygon是一个MultiPolygon
 */
bool ShpUtil::IsSimplePolygon(SHPObject* pSHPObject)
{
	if(pSHPObject->nParts==1)
		return true;
	
	int numParts  = pSHPObject->nParts;
	int numPoints = 0;
	int i=0,j=0,k=0,t=0;
	float x0 = 0.0f, y0 = 0.0f;
	float x1 = 0.0f, y1 = 0.0f;
	float x2 = 0.0f, y2 = 0.0f;
	
	for(i=1; i<numParts; i++)
	{
		j = pSHPObject->panPartStart[i];
		if(i<(numParts-1))
		{
			k = pSHPObject->panPartStart[i+1];
			numPoints = k-j;
			
			x0 = (pSHPObject->padfX)[j];
			y0 = (pSHPObject->padfY)[j];
			
			x1 = (pSHPObject->padfX)[j+1];
			y1 = (pSHPObject->padfY)[j+1];
			
			x2 = (pSHPObject->padfX)[j+2];
			y2 = (pSHPObject->padfY)[j+2];
			
			if(IsClockwise(x0, y0, x1, y1, x2, y2))
			{
				return false;
			}
		}
		else
		{
			k = pSHPObject->nVertices;
			numPoints = k-j;
			
			x0 = (pSHPObject->padfX)[j];
			y0 = (pSHPObject->padfY)[j];
			
			x1 = (pSHPObject->padfX)[j+1];
			y1 = (pSHPObject->padfY)[j+1];
			
			x2 = (pSHPObject->padfX)[j+2];
			y2 = (pSHPObject->padfY)[j+2];
			
			if(IsClockwise(x0, y0, x1, y1, x2, y2))
			{
				return false;
			}
		}
	}
	return true;
}

bool ShpUtil::HasAntiClockRing(SHPObject* pSHPObject)
{
	if(pSHPObject->nParts==1)
		return true;
	
	int numParts  = pSHPObject->nParts;
	int numPoints = 0;
	int i=0,j=0,k=0,t=0;
	float x0 = 0.0f, y0 = 0.0f;
	float x1 = 0.0f, y1 = 0.0f;
	float x2 = 0.0f, y2 = 0.0f;
	
	for(i=0; i<numParts; i++)
	{
		j = pSHPObject->panPartStart[i];
		if(i<(numParts-1))
		{
			k = pSHPObject->panPartStart[i+1];
			numPoints = k-j;
			
			x0 = (pSHPObject->padfX)[j];
			y0 = (pSHPObject->padfY)[j];
			
			x1 = (pSHPObject->padfX)[j+1];
			y1 = (pSHPObject->padfY)[j+1];
			
			x2 = (pSHPObject->padfX)[j+2];
			y2 = (pSHPObject->padfY)[j+2];
			
			if(!IsClockwise(x0, y0, x1, y1, x2, y2))
			{
				return true;
			}
		}
		else
		{
			k = pSHPObject->nVertices;
			numPoints = k-j;
			
			x0 = (pSHPObject->padfX)[j];
			y0 = (pSHPObject->padfY)[j];		
			
			x1 = (pSHPObject->padfX)[j+1];
			y1 = (pSHPObject->padfY)[j+1];
			
			x2 = (pSHPObject->padfX)[j+2];
			y2 = (pSHPObject->padfY)[j+2];
			
			if(!IsClockwise(x0, y0, x1, y1, x2, y2))
			{
				return true;
			}
		}
	}
	return false;
}

int ShpUtil::FindNextClockwiseRing(SHPObject* pSHPObject, int cur_ring_id)
{
	if(cur_ring_id>=pSHPObject->nParts)
	{
		return -1;
	}

	int numParts  = pSHPObject->nParts;
	int numPoints = 0;
	int i=0,j=0,k=0,t=0;
	double x0 = 0.0f, y0 = 0.0f;
	double x1 = 0.0f, y1 = 0.0f;
	double x2 = 0.0f, y2 = 0.0f;
	
	for(i=cur_ring_id+1; i<numParts; i++)
	{
		j = pSHPObject->panPartStart[i];
		if(i<(numParts-1))
		{
			k = pSHPObject->panPartStart[i+1];
			numPoints = k-j;
			
			x0 = (pSHPObject->padfX)[j];
			y0 = (pSHPObject->padfY)[j];
			
			x1 = (pSHPObject->padfX)[j+1];
			y1 = (pSHPObject->padfY)[j+1];
			
			x2 = (pSHPObject->padfX)[j+2];
			y2 = (pSHPObject->padfY)[j+2];
			
			if(IsClockwise(x0, y0, x1, y1, x2, y2))
			{
				return i;
			}
		}
		else
		{
			k = pSHPObject->nVertices;
			numPoints = k-j;
			
			x0 = (pSHPObject->padfX)[j];
			y0 = (pSHPObject->padfY)[j];
			
			x1 = (pSHPObject->padfX)[j+1];
			y1 = (pSHPObject->padfY)[j+1];
			
			x2 = (pSHPObject->padfX)[j+2];
			y2 = (pSHPObject->padfY)[j+2];
			
			if(IsClockwise(x0, y0, x1, y1, x2, y2))
			{
				return i;
			}
		}
	}
	return -1;
}

bool ShpUtil::IsClockwise(float x0, float y0,float x1, float y1,float x2, float y2)
{
	float delta_x1 = x1-x0;
	float delta_y1 = y1-y0;
	float delta_x2 = x2-x1;
	float delta_y2 = y2-y1;

	float value = delta_x1*delta_y2 - delta_x2*delta_y1;
	return (value<0.0f);
}

}
