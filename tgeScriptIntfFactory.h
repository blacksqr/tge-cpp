#pragma once
#include "tgeCommon.h"
#include "tgeCmd.h"

namespace tge
{
	/** ScriptIntfFacotry objects cannot be created within scripts. They
		simply exist when script are evaluated.
	*/
	class ScriptIntfFactory : public Cmd
	{
	public:
		typedef std::map<String, ScriptIntf*> ScriptIntfInstanceMap;
		typedef std::vector<ScriptIntf*> ScriptIntfInstanceList;
	public:
		/** The mCapabilities list should be filled in at creation phase.
			TGE Engine will ask for capabilities to register the classes this
			factory could make.
		*/
		ScriptIntfFactory(void);
		virtual ~ScriptIntfFactory(void);
		void setInterp(Interpreter* interp);
		/** 'Create' will see to make new command, register new variable to point to 
			object and so on.  In the overrided _createImp we dont have to care about 
			these messy stuff. The situation is the same with destroy.
		*/
		ScriptIntf* create(const String& name, const String& className);
		bool		destroy(const String& name);

		/** List the names of all ScriptIntf instances of class className. */
		ScriptIntfInstanceList& listInstances(const String& className);
		
		/** @brief Returns string vector containing.
			names of classes this factory could handle. Later on, the class names could be
			used as commands in Tcl script.
			@note These class names will be dealt with in the following ways:
				  "[CLASSNAME] create" command will create an ScriptIntf object instance;
				  "[CLASSNAME] destroy" command will destroy an ScriptIntf object instance;
				  "[CLASSNAME] list" command will list all the instances.
		*/
		const StringVector& getCapabilities(void) const;

		virtual CmdStatus run(const String& cmdName, const Obj& arg1, const Obj& arg2, const Obj& argRest, size_t argRestC);
	
	protected:
		/** Must be overrided. */
		virtual ScriptIntf* _createImp(const String& name, const String& className) {return NULL;};
		/** Must be overrided. */
		virtual bool _destroyImp(ScriptIntf* si) {return true;};
	protected:
		StringVector	mClasses;
		StringVector	mCapabilities;
		Interpreter*	mInterp;
		ScriptIntfInstanceMap	mSIIMap;
	};

}