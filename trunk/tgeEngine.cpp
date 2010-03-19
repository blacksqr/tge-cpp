#include "tgeEngine.h"
#include "tgeScriptIntfFactory.h"
#include "tgeInterpreter.h"

tge::Engine* Ogre::Singleton<class tge::Engine>::ms_Singleton = NULL;
namespace tge
{
	Engine::Engine(void)
	{
		createInterpreter("Default");
		setDefaultObjContext("Default");
	}
	Engine::~Engine(void)
	{
	}
	void Engine::init(void)
	{
	}

	void Engine::deInit(void)
	{
	}

	bool Engine::createInterpreter(const String& name)
	{
		mInterpMap.insert(std::make_pair(name, new Interpreter));
		return true;
	}

	Interpreter* Engine::getInterpreter(const String& name)
	{
		return mInterpMap[name];
	}

	bool Engine::registerScriptIntfFactory(const String& interpreterName, ScriptIntfFactory* factory)
	{
		bool ret = true;
		Interpreter* interp = getInterpreter(interpreterName);

		if (interp==NULL)
		{
			return false;
		}

		factory->setInterp(interp);

		StringVector svec = factory->getCapabilities();

		for (StringVector::iterator i = svec.begin(); i!= svec.end(); ++i)
		{
			// String createCmd = (*i) + String("_create");
			// String destroyCmd = (*i) + String("_destroy");
			
			String cmd = (*i);
			interp->registerCmd(cmd, factory);

			// interp->registerCmd(createCmd, factory);
			// interp->registerCmd(destroyCmd, factory);
		}

		// Tcl_GetGlobalNamespace
		return ret;

	}

	Result Engine::eval(const String& str)
	{
		return mDefContext->eval(str);
	}
	Result Engine::eval(Obj* obj) 
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
	bool Engine::registerGlobalCmd(const String& cmdName, Cmd* cmd)
	{
		return mDefContext->registerCmd(cmdName, cmd);
	}
	bool Engine::unregisterGlobalCmd(const String& cmdName)
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

	bool Engine::unregisterScriptIntfFactory(ScriptIntfFactory* factory)
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

	bool Engine::commandExists(const String& cmdName)
	{
		return mDefContext->commandExists(cmdName);
	}
}