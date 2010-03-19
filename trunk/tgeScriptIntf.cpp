#include "tgeScriptIntf.h"
#include "tgeObj.h"
#include "tgeInterpreter.h"

namespace tge
{
	ScriptIntf::ScriptIntf(void)
	{
	}
	ScriptIntf::~ScriptIntf(void)
	{
	}
	void ScriptIntf::init(const String& scriptingName, const String& className)
	{
		mScriptingInstanceName = scriptingName;
		mScriptingClassName = className;
	}
	/** ID for this object which could be accessed in script. */
	const String& ScriptIntf::getScriptingInstanceName(void) const
	{
		return mScriptingInstanceName;
	}
	const String& ScriptIntf::getScriptingClassName(void) const
	{
		return mScriptingClassName;
	}

	const Obj& ScriptIntf::get(const String& propName)
	{
		static Obj obj;	// Empty

		PropBag::const_iterator it = mProps.find(propName);

		if (it==mProps.end())
		{
			// Issues a warning
			std::cerr<<String("Specified property \"")+ propName + String(" not found. - ["+mScriptingClassName+"]") << std::endl;

			return obj;
		}
		else
		{	obj = mProps[propName];
			return obj;
		}
	}

	void ScriptIntf::set(const String& propName, const Obj& value)
	{
		mProps[propName] = value;
	}

	CmdStatus ScriptIntf::run(const String& cmdName, const Obj& arg1, const Obj& arg2, const Obj& argRest, size_t argRestc)
	{
		if (!arg1.isNull())
		{
			String action = static_cast<String>(arg1);
			if ( action.compare("set") == 0)
			{
				if (arg2.isNull())
				{
					mInterp->_panic("Null args for 'setter'. Please specify prop name. ", cmdName);
				}
				else 
				{
					if (argRestc<1)
					{
						mInterp->_panic("Null args for 'set " + (String)arg2 + "'. Please specify prop value.", cmdName);
					}
					else
					{
						Obj obj = argRest.lindex(0);
						if (!obj.isNull())
						{
							set(static_cast<String>(arg2), obj);
						}
					}
				}
				return TGE_OK;
			}
			else if ( action.compare("get") == 0)
			{
				if (arg2.isNull())
				{
					mInterp->_panic("Null args  for 'getter'. Please specify prop name.", cmdName);
				}
				_setObjResult( get(static_cast<String>(arg2)) );
				return TGE_OK;
			}
			else
			{
				return scriptAccess(action, arg2, argRest, argRestc);
			}
		}
		else
		{
			mInterp->_panic("No actions given.", cmdName);
		}
		return TGE_OK;
	}
}