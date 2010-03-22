#include "tgeEngine.h"
#include "tgeScriptIntfFactory.h"
#include "tgeInterpreter.h"

tge::Engine* Ogre::Singleton<class tge::Engine>::ms_Singleton = NULL;
namespace tge
{
	Engine::Engine(void) :
	mDefContext(NULL)
	{
		createInterpreter("Default");
		setDefaultObjContext("Default");
	}
	Engine::~Engine(void)
	{
		/** Clear upon deletion. */
		if (mInterpMap.empty())
		{
			delete getDefaultObjContext();
		}
		else
		{
			// TODO do more cleaning here
		}
	}
	void Engine::init(void)
	{
	}

	void Engine::deInit(void)
	{
	}

	Interpreter* Engine::createInterpreter(const String& name) throw (tge::except)
	{
		Interpreter* interp = new Interpreter;
		if (interp==NULL)
		{
			throw (except("Cannot create interpreter."));
		}
		std::pair<InterpreterMap::iterator, bool> ret = 
		mInterpMap.insert(InterpreterMap::value_type(name, interp));
		if ( false==ret.second )
		{
			String s("Interpreter with name ");
			s.append(name).append(" already exists.");
			throw (except(s));
		}
		return interp;
	}

	Interpreter* Engine::getInterpreter(const String& name) const
	{
		Interpreter* ret = NULL;
		if (name.empty())
		{
			ret = getDefaultObjContext();
		}
		else
		{
			InterpreterMap::const_iterator it = mInterpMap.find(name);
			if (it!=mInterpMap.end())
			{
				ret = it->second;
			}
			else
			{
				ret = NULL;
			}
		}
		return ret;
	}

	bool Engine::registerScriptIntfFactory(ScriptIntfFactory* factory, const String& interpreterName)
	{
		bool ret = true;
		Interpreter* interp = getInterpreter(interpreterName);

		if (interp==NULL)
		{
			return false;
		}
		// Checks if this factory is already registerd.
		for (SIPtrInterpPtrMap::const_iterator it = mSItoInterpMap.begin();
			it!=mSItoInterpMap.end();
			++it)
		{
			ScriptIntfFactory* _f = static_cast<ScriptIntfFactory*>(it->first);
			if ( _f==factory || typeid(_f) == typeid(factory) ) 
			{
				if ( interp==static_cast<Interpreter*>(it->second) )
				{

				}
			}
			
		}

		mSItoInterpMap.insert(std::make_pair(factory, interp));


		factory->setInterp(interp);

		StringVector svec = factory->getCapabilities();

		for (StringVector::iterator i = svec.begin(); i!= svec.end(); ++i)
		{
			String cmd = (*i);
			interp->registerCmd(cmd, factory);
		}
		return ret;
	}

	Result Engine::eval(const String& str, const String& interpreterName)
	{
		return mDefContext->eval(str);
	}
	Result Engine::eval(Obj* obj, const String& interpreterName) 
	{
		return mDefContext->eval(obj);
	}

	void Engine::setVar(const String& name, const Obj& obj)
	{
		mDefContext->setVar(name, obj);
	}
	Obj& Engine::getVar(const String& name)
	{
		return mDefContext->getVar(name);
	}
	void Engine::removeVar(const String& name)
	{
		mDefContext->removeVar(name);
	}
	bool Engine::registerGlobalCmd(const String& cmdName, Cmd* cmd, const String& interpName)
	{
		return mDefContext->registerCmd(cmdName, cmd);
	}
	bool Engine::unregisterGlobalCmd(const String& cmdName, const String& interpName)
	{
		return mDefContext->unregisterCmd(cmdName);
	}
	void Engine::setDefaultObjContext(const String& interpName)
	{
		mDefContext = getInterpreter(interpName);
	}
	Interpreter* Engine::getDefaultObjContext(void) const
	{
		return mDefContext;
	}

	bool Engine::unregisterScriptIntfFactory(ScriptIntfFactory* factory, const String& interpName)
	{
		bool ret = true;
		StringVector svec = factory->getCapabilities();
		for (StringVector::iterator it = svec.begin();
			it!=svec.end();
			++it)  
		{
			mDefContext->unregisterCmd(*it);
		}
		return ret;
	}

	bool Engine::commandExists(const String& cmdName, const String& interpName)
	{
		return mDefContext->commandExists(cmdName);
	}
}