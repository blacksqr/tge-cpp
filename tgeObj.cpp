#include "tgeObj.h"
#include "tgeInterpreter.h"
#include "tgeEngine.h"

// Bottlenecks are Tcl_NewString and Tcl_NewListObj
namespace tge
{
	Obj::Obj(void) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		 mTclObj = Tcl_NewObj();
		 addRef();
	}
	Obj::Obj(bool sb) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		mTclObj = Tcl_NewBooleanObj(sb);
		addRef();
	}
	Obj::Obj(char *sz) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		int32 len = strlen(sz);
		mTclObj = Tcl_NewStringObj((const char* )sz, (int)len);
		addRef();
	}
	Obj::Obj(const char *scz) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		int32 len = strlen(scz);
		mTclObj = Tcl_NewStringObj(scz, len);
		addRef();
	}
	Obj::Obj(const tge::String &ss) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		mTclObj = Tcl_NewStringObj(ss.c_str(), ss.length());
		addRef();
	}
	Obj::Obj(size_t st) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		mTclObj = Tcl_NewLongObj((long)st);
		addRef();
	}
	Obj::Obj(tge::int32 si) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		mTclObj = Tcl_NewLongObj((long)si);
		addRef();
	}
	Obj::Obj(tge::Real sr) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		/** Have to use double to avoid loss.... */
		mTclObj = Tcl_NewDoubleObj((double)sr);
		addRef();
	}

	Obj::Obj(const Vector2& vec) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)vec.x),
			Tcl_NewDoubleObj((double)vec.y)
		};
		mTclObj = Tcl_NewListObj(2, objv);
		addRef();
	}
	Obj::Obj(const Vector3& vec) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)vec.x),
			Tcl_NewDoubleObj((double)vec.y),
			Tcl_NewDoubleObj((double)vec.z)
		};
		mTclObj = Tcl_NewListObj(3, objv);
		addRef();
	}
	Obj::Obj(const Vector4& vec) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)vec.x),
			Tcl_NewDoubleObj((double)vec.y),
			Tcl_NewDoubleObj((double)vec.z),
			Tcl_NewDoubleObj((double)vec.w)
		};
		mTclObj = Tcl_NewListObj(4, objv);
		addRef();
	}
	Obj::Obj(const Quaternion& q) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)q.w),
			Tcl_NewDoubleObj((double)q.x),
			Tcl_NewDoubleObj((double)q.y),
			Tcl_NewDoubleObj((double)q.z)
		};
		mTclObj = Tcl_NewListObj(4, objv);
		addRef();
	}

	Obj::Obj(const tge::Obj &rhs) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		mInterp = rhs.mInterp;
		_setTclObj(rhs.mTclObj);
	}
	
	// Special, protected contructor
	Obj::Obj(Tcl_Obj* const tobj) :
	mTclObj(NULL), mInterp(Engine::getSingleton().getDefaultObjContext())
	{
		_setTclObj(tobj);
	}

	Obj::~Obj(void)
	{
		if (!isNull())
		{
			release();
		}
	}

	Obj& Obj::share(const tge::Obj &rhs)
	{
		mTclObj = rhs.mTclObj;
		mInterp = rhs.mInterp;

		addRef();
		return *this;
	}

	Interpreter* Obj::getInterp(void) const
	{
		return mInterp;
	}
	void Obj::setInterp(Interpreter *interp)
	{
		mInterp = interp;
	}

	Tcl_Obj* Obj::_getTclObj(void) const
	{
		return mTclObj;
	}
	void Obj::_setTclObj(Tcl_Obj* const obj)
	{
		if (NULL != obj)
		{
			mTclObj = Tcl_DuplicateObj(obj);
		}
		else
		{
			mTclObj = NULL;
		}

	}
	bool Obj::isShared(void) const
	{
		return Tcl_IsShared(mTclObj);
	}

	bool Obj::isNull(void) const
	{
		return mTclObj==NULL;
	}
	Obj& Obj::operator=(const Obj& o)
	{
		mTclObj = Tcl_DuplicateObj(o.mTclObj);
		mInterp = o.mInterp;
		return *this;
	}

	Obj& Obj::operator=(bool sb)
	{
		Tcl_SetBooleanObj(mTclObj, sb);
		return *this;
	}
	Obj& Obj::operator=(Real sr)
	{
		Tcl_SetDoubleObj(mTclObj, (double)sr);
		return *this;
	}
	Obj& Obj::operator=(int32 si)
	{
		Tcl_SetLongObj(mTclObj, (long)si);
		return *this;
	}
	Obj& Obj::operator=(size_t st)
	{
		Tcl_SetLongObj(mTclObj, (long)st);
		return *this;
	}
	Obj& Obj::operator=(const char* scz)
	{
		Tcl_SetStringObj(mTclObj, scz, (int)strlen(scz));
		return *this;
	}
	Obj& Obj::operator=(char* sz)
	{
		Tcl_SetStringObj(mTclObj, sz, (int)strlen(sz));
		return *this;
	}
	Obj& Obj::operator=(const String& s)
	{
		Tcl_SetStringObj(mTclObj, s.c_str(), (int)s.length());
		return *this;
	}
	/** Vector2 as list: {x y} */
	Obj& Obj::operator=(const Vector2& vec)
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)vec.x),
			Tcl_NewDoubleObj((double)vec.y)
		};
		Tcl_SetListObj(mTclObj, 2, objv);
		return *this;
	}
	/** Vector3 as list: {x y z} */
	Obj& Obj::operator=(const Vector3& vec)
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)vec.x),
			Tcl_NewDoubleObj((double)vec.y),
			Tcl_NewDoubleObj((double)vec.z)
		};
		Tcl_SetListObj(mTclObj, 3, objv);
		return *this;
	}
	/** Vector4 as list: {x y z w} */
	Obj& Obj::operator=(const Vector4& vec)
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)vec.x),
			Tcl_NewDoubleObj((double)vec.y),
			Tcl_NewDoubleObj((double)vec.z),
			Tcl_NewDoubleObj((double)vec.w)
		};
		Tcl_SetListObj(mTclObj, 4, objv);
		return *this;
	}
	/** Quaternion as list: {w x y z} */
	Obj& Obj::operator=(const Quaternion& vec)
	{
		Tcl_Obj* objv[] = {
			Tcl_NewDoubleObj((double)vec.w),
			Tcl_NewDoubleObj((double)vec.x),
			Tcl_NewDoubleObj((double)vec.y),
			Tcl_NewDoubleObj((double)vec.z)
		};
		Tcl_SetListObj(mTclObj, 4, objv);
		return *this;
	}

	
	//
	Obj::operator bool(void) const
	{
		int ret = 0;
		Tcl_GetBooleanFromObj(mInterp->_interp(), mTclObj, &ret);

		return (ret==0?false : true);
	}
	Obj::operator Real(void) const
	{
		double r;
		Tcl_GetDoubleFromObj(mInterp->_interp(), mTclObj, &r);
		return static_cast<Real>(r);
	}
	Obj::operator int32(void) const
	{
		long d;
		Tcl_GetLongFromObj(mInterp->_interp(), mTclObj, &d);
		return static_cast<int32>(d);
	}
	Obj::operator size_t(void) const
	{
		long d;
		Tcl_GetLongFromObj(mInterp->_interp(), mTclObj, &d);
		return static_cast<size_t>(d);
	}
	Obj::operator const char* (void) const
	{
		char* sz = Tcl_GetString(mTclObj);
		return static_cast<const char*>(sz);
	}
	Obj::operator char*  (void) const
	{
		int len = 0;
		char* sz = Tcl_GetStringFromObj(mTclObj, &len);
		char* r = new char[len+1];
		memcpy(r, sz, (size_t)len);
		r[len] = '\0';

		return r;
	}
	Obj::operator String (void) const
	{
		return String(Tcl_GetString(mTclObj));
	}

	Obj::operator Vector2 (void) const
	{
		int n;
		Tcl_Obj** objv;
		Tcl_ListObjGetElements(mInterp->_interp(), mTclObj, &n, &objv);
		assert ( n > 1 );
		static double x=0, y=0;
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[0], &x);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[1], &y);

		return Vector2((Real)x, (Real)y);
	}
	Obj::operator Vector3 (void) const
	{
		int n;
		Tcl_Obj** objv;
		Tcl_ListObjGetElements(mInterp->_interp(), mTclObj, &n, &objv);
		assert ( n > 2 );
		static double x = 0, y = 0, z = 0;
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[0], &x);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[1], &y);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[2], &z);
		return Vector3((Real)x, (Real)y, (Real)z);
	}
	Obj::operator Vector4 (void) const
	{
		int n;
		Tcl_Obj** objv;
		Tcl_ListObjGetElements(mInterp->_interp(), mTclObj, &n, &objv);
		assert ( n > 3 );
		static double x=0, y=0, z=0, w=0;
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[0], &x);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[1], &y);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[2], &z);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[3], &w);
		return Vector4((Real)x, (Real)y, (Real)z, (Real)w);
	}
	Obj::operator Quaternion (void) const
	{	
		int n;
		Tcl_Obj** objv;
		Tcl_ListObjGetElements(mInterp->_interp(), mTclObj, &n, &objv);
		assert ( n > 3 );
		static double w=0, x=0, y=0, z=0;
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[0], &w);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[1], &x);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[2], &y);
		Tcl_GetDoubleFromObj(mInterp->_interp(), objv[3], &z);
		return Quaternion((Real)w, (Real)x, (Real)y, (Real)z);
	}


	/// List handling 
	Obj& Obj::lappend(const Obj& newObj)
	{
		if (mTclObj==NULL)
		{
			mTclObj = Tcl_NewObj();
			addRef();
		}
		Tcl_ListObjAppendElement(mInterp->_interp(), mTclObj, Tcl_DuplicateObj(newObj.mTclObj));
		
		//newObj.setInterp(mInterp);
		//newObj.addRef();

		return *this;
	}

	Obj& Obj::lappendList(const Obj& newObj)
	{
		if (mTclObj==NULL)
		{
			mTclObj = Tcl_NewObj();
			addRef();
		}
		Tcl_ListObjAppendList(mInterp->_interp(), mTclObj, Tcl_DuplicateObj(newObj.mTclObj));
		
		//newObj.setInterp(mInterp);
		//newObj.addRef();

		return *this;
	}

	Obj& Obj::lindex(size_t index) const
	{
		static Obj lob;
		if (mTclObj==NULL)
		{
			return lob;
		}
		Tcl_Obj* ptr = NULL;
		Tcl_ListObjIndex(mInterp->_interp(), mTclObj, index, &ptr);
		
		lob._setTclObj(ptr);
		return lob;
	}

	Obj& Obj::linsert(size_t index, Obj& newObj)
	{
		if (mTclObj==NULL)
		{
			mTclObj = Tcl_NewObj();
			addRef();
		}
		Tcl_Obj *const objv[] = {newObj.mTclObj};
		Tcl_ListObjReplace(mInterp->_interp(), mTclObj, index, 0, 1, objv);
		return *this;
	}

	Obj& Obj::linsertList(size_t index, const Obj& newObj)
	{
		if (mTclObj==NULL)
		{
			mTclObj = Tcl_NewObj();
			addRef();
		}
		int n;
		Tcl_Obj** objv;
		Tcl_ListObjGetElements(mInterp->_interp(), newObj.mTclObj, &n, &objv);
		if (n>0)
		{
			Tcl_ListObjReplace(mInterp->_interp(), mTclObj, index, 0, n, objv);
		}
		return *this;
	}

	size_t Obj::llength(void) const
	{
		if (mTclObj==NULL)
		{
			return 0;
		}
		int len = 0;
		Tcl_ListObjLength(mInterp->_interp(), mTclObj, &len);
		return (size_t)len;
	}

	void Obj::lremove(size_t index)
	{
		if (mTclObj!=NULL)
		{
			Tcl_ListObjReplace(mInterp->_interp(), mTclObj, index, 1, 0, NULL);
		}
	}


	/// NOT IMPLEMENTED YET!
	Obj& Obj::lrange(size_t startIndex, size_t endIndex)
	{
		return *this;
	}

	uint32 Obj::addRef(void)
	{
		return (uint32)(Tcl_IncrRefCount(mTclObj));
	}
	uint32 Obj::release(void)
	{
		uint32 rc = --(mTclObj)->refCount;
		if ( rc<= 0) 
		{
			TclFreeObj(mTclObj);
			mTclObj = NULL;
			rc = 0;
		}
		return rc;
	}
	void Obj::setNull(void)
	{
		if (mTclObj!=NULL)
		{
			TclFreeObj(mTclObj);
		}
		mTclObj = NULL;
	}
}