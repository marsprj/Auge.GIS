#ifndef __AUGE_IMS_DATASET_CREATE_REQUEST_H__
#define __AUGE_IMS_DATASET_CREATE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GField;
	class GFields;
	class GeometryDef;
	class XNode;
	class XElement;
	class XDocument;

	class CreateDataSetRequest : public WebRequest
	{
	public:
		CreateDataSetRequest();
		virtual ~CreateDataSetRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char*	GetSourceName();
		const char*	GetDataSetName();
		GFields*	GetFields();

	public:
		bool		Create(XDocument* pxDoc);

		void		SetVersion(const char* value);
		void		SetSourceName(const char* name);
		void		SetDataSetName(const char* name);

		GFields*	CreateFields();
		GField*		CreateField(XElement* pxField);
		void		SetGeometryDef(GeometryDef* pGeometryDef, XNode* pxGeometryDef);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_source_name;
		std::string m_dataset_name;
		std::string m_encoding;
		XDocument*	m_pxDoc;
		GFields*	m_pFields;
	};
}

#endif //__AUGE_IMS_DATASET_CREATE_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=dbs&version=1.0.0&request=GetDataSource&name=db1
http://127.0.0.1:8088/ows/user1/mgr?

<CreateDataSet 
	service="dbs"
	version="1.0.0"
	sourceName="db1"
	dataSetName="table1">
	<Fields>
		<Field>
			<Name>ID</Name>
			<Type>Int</Type>
			<DefaultValue>0</DefaultValue>
		</Field>
		<Field>
			<Name>Name</Name>
			<Type>String</Type>
			<Length>32</Length>
			<Nullable>true</Nullable>
			<DefaultValue>china</DefaultValue>
		</Field>
		<Field>
			<Name>shape</Name>
			<Type>Geometry</Type>
			<Nullable>true</Nullable>
			<GeometryDef>
				<Type>Point</Type>
				<SRID>4326</SRID>
				<Extent>
					<XMin>-180.0</XMin>
					<XMax>180.0</XMax>
					<YMin>-90.0</YMin>
					<YMax>90.0</YMax>
				</Extent>
			</GeometryDef>
		</Field>
	</Fields>
</CreateDataSet>

************************************************************************/