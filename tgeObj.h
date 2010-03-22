#pragma once
#include "tgeCommon.h"
#include "tgeEngine.h"
#include "tgeInterpreter.h"

namespace tge
{
	/** We should let the interpreter create the objects for us. 
		do not call these ctors directly.
		@note Interpreter pointer COULD be explicitly set if Obj not retrieved
			from following methods:
			
			Interpreter::getVar()
			Result::obj()
			Result::objptr()

			If not Interpreter not set nor the Obj is returned from the methods above.
			The default context interpreter will be set.

			Special note on Obj copying, if two underlying Tcl_Obj are not bound to the 
			same Tcl_Interp, the copy will fail.
	*/

	class Obj
	{
	public:
		/** A default contructed Obj will return false if tests with isNull().
			Since it will create a Tcl_Obj by defult. Obj::setNull() is used to
			clear out Obj.
		*/
		Obj(void);

		explicit Obj(bool sb);
		explicit Obj(Real sr);
		explicit Obj(int32 si);
		explicit Obj(size_t st);
		/** @param scz A Null-terminated string. */
		explicit Obj(const char* scz);
		/** @param sz A Null-terminated string. */
		explicit Obj(char* sz);
		explicit Obj(const String& ss);
		/** For storing pointers. */
		// explicit Obj(void* vptr);
		/** Vector2 as list: {x y} */
		explicit Obj(const Vector2& vec);
		/** Vector3 as list: {x y z} */
		explicit Obj(const Vector3& vec);
		/** Vector4 as list: {x y z w} */
		explicit Obj(const Vector4& vec);
		/** Quaternion as list: {w x y z} */
		explicit Obj(const Quaternion& vec);
		/** This is somehow not recommended. */
		Obj(Tcl_Obj* const tobj);

		template<typename T> Obj(T* tptr) :
		mTclObj(NULL),mInterp(Engine::getSingleton().getDefaultObjContext())
		{
			mTclObj = Tcl_NewIntObj((int)tptr);
			addRef();
		}

		virtual ~Obj();

		/** Copy ctor
			@note Always uses duplicate thus avoiding 
			unnoticed shared use of Tcl_Obj* . 
		*/
		Obj(const Obj& rhs);

		/** Explicite share with another Obj. This 
			will also imply an interp switch to rhs's interpreter.
			Use with care :-)
		*/
		Obj& share(const Obj& rhs);
		
		/**
		@note Always uses duplicate thus avoiding 
				unnoticed shared use of Tcl_Obj* . 
		*/
		Obj& operator=(const Obj& o);

		Obj& operator=(bool sb);
		Obj& operator=(Real sr);
		Obj& operator=(int32 si);
		Obj& operator=(size_t st);
		Obj& operator=(const char* scz);
		Obj& operator=(char* sz);
		Obj& operator=(const String &s);
		/** Vector2 as list: {x y} */
		Obj& operator=(const Vector2& vec);
		/** Vector3 as list: {x y z} */
		Obj& operator=(const Vector3& vec);
		/** Vector4 as list: {x y z w} */
		Obj& operator=(const Vector4& vec);
		/** Quaternion as list: {w x y z} */
		Obj& operator=(const Quaternion& vec);
		
		template <typename T> Obj& operator=(T* vptr)
		{
			Tcl_SetIntObj(mTclObj, (int)vptr);
			return *this;
		}




		Interpreter*	getInterp(void) const;
		void setInterp(Interpreter* interp);

		/** @note Just simulate COM interfaces. This value should never
			be less than zero. So use uint32.	
		*/
		uint32 addRef(void);
		uint32 release(void);
		bool isShared(void) const;
		bool isNull(void) const;
		void setNull(void);

		/** Static conversions below. */
		operator bool(void) const;
		operator Real(void) const;
		operator int32(void) const;
		operator size_t(void) const;
		operator const char* (void) const;
		operator char*  (void) const;
		operator String (void) const;
		operator Vector2 (void) const;
		operator Vector3 (void) const;
		operator Vector4 (void) const;
		operator Quaternion (void) const;


		template <typename T> operator T* (void)
		{
			int32 ii = 0;
			Tcl_GetIntFromObj(mInterp->_interp(), mTclObj, &ii);
			return reinterpret_cast<T*>(ii);
		}

		template <typename T> operator T* (void) const
		{
			int32 ii = 0;
			Tcl_GetIntFromObj(mInterp->_interp(), mTclObj, &ii);
			return reinterpret_cast<T*>(ii);
		}



		/** Appends an Obj instance. 
			@note The incoming element is directly appened, no copy generated.
			for performance. This op changes current Obj.
			@return The Obj ref after appending.
		*/
		Obj& lappend(const Obj& newObj);
		
		/** Appends an Obj trying to convert it into a TclList
			@note The incoming elements are directly appened, no copy generated.
			for performance. This op changes current Obj.
			@return The Obj ref after appending.
		*/
		Obj& lappendList(const Obj& newObjlist);
		
		/** Returns a COPY of the Obj instance at index. Should check if returned
			Obj instance isNull(). */
		Obj& lindex(size_t index) const;

		/** Inserts a single Obj before the index. */
		Obj& linsert(size_t index, Obj& newObj);

		/** Try to insert the list stored in newObj */
		Obj& linsertList(size_t index, const Obj& newObj);

		size_t llength(void) const;

		/** No support for chain'd style lremove operations, since they are 
			dangerous.
			@return Nothing....
		*/

		void lremove(size_t index);

		/** Returns an Obj list which contains COPY of the list elements 
			from start to end (exlusive). 
			@note Not implemented yet.
		*/
		Obj& lrange(size_t startIndex, size_t endIndex);

		/** @brief Returns the enclosed Tcl_Obj.
			@details Manipulating the Tcl_Obj directly might result in unpredictable result.
		*/
		Tcl_Obj* _getTclObj(void) const;
		
		/** @brief Directly change current Tcl_Obj content. 
			
			@details 
			Do not use this method unless you know what you are doing. 
			Make sure to check and assert if this obj is currently being shared!
			Also if given obj prt is NULL, this method will release current
			Tcl_Obj ptr and set it to NULL.
			Use Obj::isNull() to check null after this method is called.
		*/
		void _setTclObj(Tcl_Obj* const obj);
	protected:
		
	protected:
		Tcl_Obj*		mTclObj;
		Interpreter*	mInterp;
	};

}