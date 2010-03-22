#pragma once

#include "tgeCommon.h"
#include "tgeCmd.h"
#include "tgeObj.h"

namespace tge
{
	/** ScriptIntf supports only one interpreter for efficiency.
	*/
	class ScriptIntf : public Cmd
	{
		friend class ScriptIntfFactory;
	public:
		typedef std::map<String, Obj> PropBag;
	public:
		/** @brief A default ctor is a must. */
		ScriptIntf(void);

		/** @bug FIXED. Added this 'virtual' keyword to destructor on Sep 26th, 2009. 
			Without this method there have been BIG problems. 
		*/
		virtual ~ScriptIntf(void);
		
		/** @brief This init method MUST be called explicitly after consturction.
			@param scriptingName Name of this factory in TCL script.
			@param className Name of the class of this factory, used for 
		*/
		void init(const String& scriptingName, const String& className);

		/** ID for this object which could be accessed in script. */
		const String& getScriptingInstanceName(void) const;
	
		const String& getScriptingClassName(void) const;

		/** Should override this method. 
			@note For scriptAccess The arg1 should be the 2nd arg in the Cmd::run() call. E.g.,
				Jeff       tells      story
				~~~~       ~~~~~      ~~~~~
		        cmdName    action     arg1
		*/
		virtual CmdStatus scriptAccess(const String& action, const Obj& arg1, const Obj& argList, int argc) = 0; 
	


		/** @brief Forward method, do not override this in subclasses. */
		virtual CmdStatus run(const String& cmdName, const Obj& arg1, const Obj& arg2, const Obj& argRest, size_t argRestc);

		/** @note Getter and setter should not appear in critical paths. */
		/** Script access getter. Just return any Obj referernce and engine
			will be responsible to set it to Tcl result.
		*/
		virtual const Obj& get(const String& propName);

		/** Script access setter. */
		virtual void set(const String& propName, const Obj& propValue);
	

	protected:
		String mScriptingInstanceName;
		String mScriptingClassName;
		PropBag mProps;
	};


}