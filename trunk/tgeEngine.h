#pragma once
#include "tgeCommon.h"
#include <OgreSingleton.h>

namespace tge
{
	/** Scripting system engine
	*/
	class Engine : public Ogre::Singleton<Engine>
	{
	public:
		typedef std::map<String, Interpreter*> InterpreterMap;
	public:
		/** On construction, Engine will automatically create an Interpreter called "Default". 
			And set it as the default Obj context.
		*/
		Engine();
		~Engine();
		void init(void);

		bool createInterpreter(const String& name);
		Interpreter* getInterpreter(const String& name);

		/** Set the default interp for Obj, this will save the workload
			to call Obj::setInterp() everywhere. And also makes the subsequent
			Engine::eval calls bound to that interpreter.
		*/
		void setDefaultObjContext(const String& interpName);
		Interpreter*	getDefaultObjContext(void) const;

		bool registerScriptIntfFactory(const String& interpreterName, ScriptIntfFactory* factory);
		bool unregisterScriptIntfFactory(ScriptIntfFactory* factory);



		// Pure wrappers. 
		Result eval(const String& str) throw(tge::except);
		Result eval(Obj* obj) throw(tge::except);
		void setVar(const String& name, const Obj& obj);
		void removeVar(const String& name);
		Obj& getVar(const String& name) throw(tge::except);
		
		bool registerGlobalCmd(const String& cmdName, Cmd* cmd);
		bool unregisterGlobalCmd(const String& cmdName);
		
		bool commandExists(const String& cmdName);
		void deInit(void);


	protected:
		InterpreterMap		mInterpMap;
		Interpreter*		mDefContext;
	};
}