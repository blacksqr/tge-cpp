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
		typedef std::map<ScriptIntfFactory*, Interpreter*> SIPtrInterpPtrMap;
    public:
        /** @brief Constructor.

        	@details
        	Upon construction, Engine will automatically create an Interpreter called "Default".
        	And set it as the default Obj context. Note that "Obj context" is equivalent to
        	"Interpreter" instance, they only differ literally. I use these two terms interchangably.
        */
        Engine(void);
        /** Created interpreters, ScriptIntf's, ScriptIntfFactories and global commands will all
        	be cleaned up in destruction.
        */
        virtual ~Engine(void);
        /** @brief Creates an interpreter instance with a name, which could be used as its id.
			@details This method will throw exception if error occurs.
        	@param name The name given to the interpreter being created.
        	@return The pointer to the interpreter if succeeds.
        */
		Interpreter* createInterpreter(const String& name) throw (tge::except);
        /** @brief Retrieves the interpreter instance with the name.

        	@param name Name of the interpreter.
        	@return Pointer to the interpreter instance. Or NULL if not exists.
        */
        Interpreter* getInterpreter(const String& name) const;

        /** @brief Sets the default interp for Obj, this will save the workload
        	to call Obj::setInterp() everywhere. And also makes the subsequent
        	Engine::eval calls bound to that interpreter.

        	@param The name of the interpreter, which will be served as the default context for
        		later Obj manipulation.
        */
        void setDefaultObjContext(const String& interpName);
        /** Retrieves the interpreter used as default.
        	@return Pointer to the interpreter instance.
        */
        Interpreter*	getDefaultObjContext(void) const;

		/** @breif Registers a certain ScriptIntfFactory to a given interpreter. 
			@details Only one instance of a factory of certain type could be registered to one interpreter. 
					Attempts to register more than one instance of factory of same type to the same interpreter 
					will fail. However we could register the SAME ScriptIntfFactory to different interpreters.
			@param factory The pointer to the specific factory instance.
			@param 
		
		*/

        bool registerScriptIntfFactory(ScriptIntfFactory* factory, const String& interpreterName = String(""));
        bool unregisterScriptIntfFactory(ScriptIntfFactory* factory, const String& interpreterName = String(""));

        Result eval(const String& str, const String& interpreterName = String("")) throw(tge::except);
        Result eval(Obj* obj, const String& interpreterName = String("")) throw(tge::except);
        void setVar(const String& name, const Obj& obj);
        Obj& getVar(const String& name) throw(tge::except);
        void removeVar(const String& name);


        bool registerGlobalCmd(const String& cmdName, Cmd* cmd, const String& interpName = String(""));

        /**
        */
        bool unregisterGlobalCmd(const String& cmdName, const String& interpName = String(""));

        /** Judges if a command with cmdName exists in the given interp instance with interpName.
        	@param cmdName Name of the command.
        	@param interpName Name of the interpreter. If omitted, default context will be used.
        	@return True if the command exists otherwise no luck.
        */
        bool commandExists(const String& cmdName, const String& interpName = String(""));


    protected:
        /** Dummy for now. */
        void init(void);
        void deInit(void);

    protected:
        InterpreterMap		mInterpMap;
        Interpreter*		mDefContext;
		SIPtrInterpPtrMap	mSItoInterpMap;
    };
}
