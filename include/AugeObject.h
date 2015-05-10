#ifndef __AUGE_OBJECT_H__
#define __AUGE_OBJECT_H__

namespace auge
{
	class GObject
	{
	public:
		GObject()
		{
			m_ref = 1;
			m_id = -1;
			m_version = 0;
		}

		virtual ~GObject(){}

	public:
		virtual int AddRef()
		{
			return (++m_ref);
		}

		virtual int ReleaseRef()
		{
			if(m_ref==0)
			{
				return m_ref;
			}
			return --m_ref;
		}

		virtual void Release()
		{
			if(ReleaseRef()==0)
			{
				delete this;
			}
		}

	public:
		int GetID()
		{
			return m_id;
		}

		void SetID(int id)
		{
			m_id = id;
		}

		int GetVersion()
		{
			return m_version;
		}

		void SetVersion(int version)
		{
			m_version = version;
		}

	protected:
		int  m_id;
		int	 m_ref;
		int	 m_version;
	};

}

#endif //__AUGE_OBJECT_H__
