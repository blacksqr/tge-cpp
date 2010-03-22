#include "tgeScriptIntfFactory.h"

#include "tgeInterpreter.h"
#include "tgeScriptIntf.h"

namespace tge
{
	ScriptIntfFactory::ScriptIntfFactory(void)
	{
	}
	ScriptIntfFactory::~ScriptIntfFactory(void)
	{
	}
	void ScriptIntfFactory::setInterp(Interpreter* interp)
	{
		mInterp = interp;
	}
	const StringVector& ScriptIntfFactory::getCapabilities(void) const
	{
		return mCapabilities;
	}
	ScriptIntf* ScriptIntfFactory::create(const String& name, const String& className)
	{
		ScriptIntf* si = _createImp(name, className);
		si->init(name, className);
		mInterp->registerCmd(name, si);
		mInterp->setVar(name, Obj(si));
		mSIIMap.insert(std::make_pair(name, si));
		return si;
	}
	bool ScriptIntfFactory::destroy(const String &name)
	{
		bool ret = true;
		ScriptIntfInstanceMap::iterator it = mSIIMap.find(name);
		if (it!=mSIIMap.end())
		{
			ScriptIntf* si = it->second;
			mSIIMap.erase(it);
			mInterp->unregisterCmd(name);
			mInterp->removeVar(name);
			
			ret = _destroyImp(si);
			
		}
		else
		{
			// On destroy exception, just silently returns a boolean.
			// Don have to be harsh
			ret = false;
		}
		return ret;
	}
	
	ScriptIntfFactory::ScriptIntfInstanceList& ScriptIntfFactory::listInstances(const String& className)
	{
		static ScriptIntfInstanceList _localList;
		static Obj _scriptResult;

		_localList.clear();
		_scriptResult.setNull();

		for (ScriptIntfInstanceMap::iterator it = mSIIMap.begin() ; it!=mSIIMap.end();++it)
		{
			if (it->second!=NULL && it->second->getScriptingClassName()==className)
			{
				String s(it->first);	
				_localList.push_back(it->second);
				_scriptResult.lappend(Obj(s));
			}
		}
		_setObjResult(_scriptResult);
		return _localList;
	}

	CmdStatus ScriptIntfFactory::run(
				const String& cmdName, 
				const Obj& arg1, 
				const Obj& arg2, 
				const Obj& argRest, 
				size_t argRestC)
	{
		for (StringVector::const_iterator it = mCapabilities.begin(); 
			it!=mCapabilities.end();
			++it)
		{
			// Matches class object to create
			if ( (*it).compare(cmdName)==0 )
			{
				if (arg1.isNull())
				{
					mInterp->_panic(
						"For factory commands, there must be at least 1 argument.",
						cmdName + " Create"
						);
				}
				String sub = (String)arg1;
				// Switch sub command
				if ( 0==sub.compare("create"))
				{
					if (arg2.isNull())
					{
						mInterp->_panic("A scripting name must be given for 'create'.", cmdName);
					}
					String scriptingName = (String)arg2;
					return  ( create(scriptingName, cmdName)!=NULL ? TGE_OK : TGE_ERROR );
				}
				else if ( 0==sub.compare("destroy"))
				{
					if (arg2.isNull())
					{
						mInterp->_panic("A scripting name must be given for 'destroy'.", cmdName);
					}
					String scriptingName = (String)arg2;
					return (true == destroy(scriptingName) ? TGE_OK : TGE_ERROR);
				}
				else if ( 0==sub.compare("list") )
				{
					
					listInstances(static_cast<String>(cmdName));
					
					return TGE_OK;
				}
				else
				{
					mInterp->_panic("Unknown sub command '" + sub + "'. Must be 'create', 'destroy' or 'list'. ", cmdName);
				}

				break;
			}
		}
		
		// Factory command not found
		mInterp->_panic("The command named \"" + cmdName + "\" not found.");
		return TGE_OK;
	}

}