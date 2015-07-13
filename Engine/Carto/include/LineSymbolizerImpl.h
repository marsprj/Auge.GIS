#ifndef __AUGE_LINE_SYMBOLIZER_IMPL_H__
#define __AUGE_LINE_SYMBOLIZER_IMPL_H__

#include "AugeStyle.h"
#include "StrokeImpl.h"
#include <string>

namespace auge
{

class LineSymbolizerImpl : public LineSymbolizer
{
public:
	LineSymbolizerImpl();
	~LineSymbolizerImpl();
public:
	virtual const char* GetGeometry();
	virtual Stroke*		GetStroke();

	virtual bool		SetGeometry(const char* prop_name);
	virtual bool		SetStroke(Stroke* pStroke);

	virtual void		SetLineType(augeLineType type);
	virtual augeLineType GetLineType();

	virtual void		Release();

	virtual augeSymbolizerType	GetType();	
private:
	std::string		m_geometry_prop_name;
	StrokeImpl	m_stroke;
	/*
	PerpendicularOffset allows to draw lines in parallel to the original geometry. For 
	complex line strings these parallel lines have to be constructed so that the distance 
	between original geometry and drawn line stays equal. This construction can result in 
	drawn lines that are actually smaller or longer than the original geometry. 
	*/
	float		m_perpendicular_offset;

	augeLineType		m_line_type;
};

}

#endif //__AUGE_LINE_SYMBOLIZER_IMPL_H__

