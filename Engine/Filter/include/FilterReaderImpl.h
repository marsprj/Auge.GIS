#ifndef __AUGE_FILER_READER_IMPL_H__
#define __AUGE_FILER_READER_IMPL_H__

#include "AugeFilter.h"

namespace auge
{
	class GFields;

	class FilterReaderImpl : public FilterReader
	{
	public:
		FilterReaderImpl(GFields* pFields);
		virtual ~FilterReaderImpl();
	public:
		virtual GFilter*	Read(XElement* pxFilter);
		virtual void		Release();

	private:
		GFilter*			ReadFilter(XNode* pxFilter);

		// Filter
		GFilter*			ReadBBox(XNode* pxBBox);
		GFilter*			ReadBinaryComparison(XNode* pxComparison);
		GFilter*			ReadBinarySpatial(XNode* pxSpatial,augeSpatialOperator oper);
		GFilter*			ReadBinaryLogical(XNode* pxLogical);
		GFilter*			ReadUnaryLogic(XNode* pxLogical);
		GFilter*			ReadIdFilter(XNode* pxIdFilter);
		GFilter*			ReadDistanceSpatial(XNode* pxSpatial, augeSpatialOperator oper);
		
		// Expression
		Expression*			ReadExpression(XNode* pxExpression);
		PropertyName*		ReadPropertyName(XNode* pxPropertyName);
		Literal*			ReadLiteral(XNode* pxNode);
		bool				ReadEnvelope(XNode* pxEnvelope, GEnvelope& extent);
		bool				ReadBox(XNode* pxBox, GEnvelope& extent);

		augeComparisonOperator	ParseComparisonOperator(const char* oper);

	private:
		GFields		*m_pFields;
		std::string	m_prop_name;
	};
}

#endif //__AUGE_FILER_READER_IMPL_H__
